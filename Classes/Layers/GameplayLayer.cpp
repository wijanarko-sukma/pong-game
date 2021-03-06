#include "GameplayLayer.h"
#include "Global.h"

constexpr float SPEED = 200.0;
constexpr float SPEED_LIMIT = 600.0;
constexpr float SPEED_ADDITION = 10.0;
constexpr float COUNTDOWN_START_TIME = 3.0;

enum PhysicsCategory
{
  NoneBitmask = 0,
  ArenaBitmask = (1 << 0),
  BarBitmask = (1 << 1),
  BallBitmask = (1 << 2),
  LeftGoalBitmask = (1 << 3),
  RightGoalBitmask = (1 << 4),
  AllBitmask = ArenaBitmask | BarBitmask | BallBitmask | LeftGoalBitmask | RightGoalBitmask
};

enum ObjectTag
{
  NoneTag = 0,
  ArenaTag,
  LeftBarTag,
  RightBarTag,
  BallTag
};

GameplayLayer::GameplayLayer(std::shared_ptr<GameManager> gameManager) :
  _isPaused(false)
  , _disableInput(false)
  , _gameManager(gameManager)
  , _ball(nullptr)
  , _countdownTime(0.0)
  , _countdownText(nullptr)
{
}

GameplayLayer::~GameplayLayer()
{
}

GameplayLayer * GameplayLayer::create(std::shared_ptr<GameManager> gameManager)
{
  GameplayLayer * obj = new GameplayLayer(gameManager);

  if (obj && obj->init()) {
    obj->autorelease();
    return obj;
  }

  CC_SAFE_DELETE(obj);
  return nullptr;
}

bool GameplayLayer::init()
{
  if (Layer::init()) {
    this->constructArena();
    this->prepareGame();
    return true;
  }
  return false;
}

void GameplayLayer::constructArena()
{
  cocos2d::Director * director = cocos2d::Director::getInstance();
  cocos2d::Size visibleSize = director->getVisibleSize();
  cocos2d::Vec2 visibleOrigin = director->getVisibleOrigin();
  cocos2d::Size screenSize = cocos2d::Size(SCREEN_WIDTH_HD, SCREEN_HEIGHT_HD);
  cocos2d::Size gameSize = cocos2d::Size(GAME_WIDTH_HD, GAME_HEIGHT_HD);
  cocos2d::Size playSize = cocos2d::Size(GAME_WIDTH_HD + 40, GAME_HEIGHT_HD - 160);

  cocos2d::ui::Widget * arena = cocos2d::ui::Widget::create();
  arena->setContentSize(playSize);
  arena->setPosition(cocos2d::Vec2(visibleOrigin.x + visibleSize.width / 2, (screenSize.height - gameSize.height) / 2 + playSize.height / 2));
  arena->setTag(ObjectTag::ArenaTag);
  this->addChild(arena, 0, "arena");

  cocos2d::PhysicsBody * arenaBounding = cocos2d::PhysicsBody::createEdgeBox(playSize, cocos2d::PhysicsMaterial(0.1f, 1.0f, 0.0f));
  arenaBounding->setDynamic(false);
  arenaBounding->setCategoryBitmask(PhysicsCategory::ArenaBitmask);
  arenaBounding->setCollisionBitmask(PhysicsCategory::BarBitmask | PhysicsCategory::BallBitmask);
  arenaBounding->setContactTestBitmask(PhysicsCategory::BarBitmask | PhysicsCategory::BallBitmask);
  arena->setPhysicsBody(arenaBounding);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  cocos2d::Size buttonSize = playSize / 4;

  cocos2d::ui::Widget * leftUpperButton = this->createInvisibleButton(buttonSize, CC_CALLBACK_2(GameplayLayer::onUpperButtonTouched, this));
  leftUpperButton->setPosition(cocos2d::Vec2(buttonSize.width / 2 + 20, playSize.height / 2 + buttonSize.height / 2));
  leftUpperButton->setTag(PlayerSide::LeftPlayer);
  arena->addChild(leftUpperButton);

  cocos2d::ui::Widget * leftBottomButton = this->createInvisibleButton(buttonSize, CC_CALLBACK_2(GameplayLayer::onBottomButtonTouched, this));
  leftBottomButton->setPosition(cocos2d::Vec2(buttonSize.width / 2 + 20, buttonSize.height / 2));
  leftBottomButton->setTag(PlayerSide::LeftPlayer);
  arena->addChild(leftBottomButton);

  cocos2d::ui::Widget * rightUpperButton = this->createInvisibleButton(buttonSize, CC_CALLBACK_2(GameplayLayer::onUpperButtonTouched, this));
  rightUpperButton->setPosition(cocos2d::Vec2(playSize.width - buttonSize.width / 2 - 20, playSize.height / 2 + buttonSize.height / 2));
  rightUpperButton->setTag(PlayerSide::RightPlayer);
  arena->addChild(rightUpperButton);

  cocos2d::ui::Widget * rightBottomButton = this->createInvisibleButton(buttonSize, CC_CALLBACK_2(GameplayLayer::onBottomButtonTouched, this));
  rightBottomButton->setPosition(cocos2d::Vec2(playSize.width - buttonSize.width / 2 - 20, buttonSize.height / 2));
  rightBottomButton->setTag(PlayerSide::RightPlayer);
  arena->addChild(rightBottomButton);
#endif

  _countdownText = cocos2d::ui::Text::create(" ", "fonts/divlit.ttf", 120);
  _countdownText->setTextColor(cocos2d::Color4B(0, 0, 0, 255));
  _countdownText->enableOutline(cocos2d::Color4B(255, 255, 255, 255), 4);
  _countdownText->setPosition(cocos2d::Vec2(playSize.width * 0.5f, playSize.height * 0.75f));
  arena->addChild(_countdownText, 99);

  cocos2d::Node * leftGoal = this->createGoal(playSize.height, PhysicsCategory::LeftGoalBitmask);
  leftGoal->setPosition(cocos2d::Vec2(20, playSize.height / 2));
  arena->addChild(leftGoal);

  cocos2d::Node * rightGoal = this->createGoal(playSize.height, PhysicsCategory::RightGoalBitmask);
  rightGoal->setPosition(cocos2d::Vec2(playSize.width - 20, playSize.height / 2));
  arena->addChild(rightGoal);

  cocos2d::Size barSize = cocos2d::Size(30, 120);
  cocos2d::Size barBodySize = cocos2d::Size(barSize.width - 4, barSize.height - 4);

  BarObject * leftBar = this->createBar("bar_green.png", barSize);
  leftBar->setTag(ObjectTag::LeftBarTag);
  arena->addChild(leftBar);
  _bars.insert(std::make_pair(PlayerSide::LeftPlayer, leftBar));

  BarObject * rightBar  = this->createBar("bar_orange.png", barSize);
  rightBar->setTag(ObjectTag::RightBarTag);
  arena->addChild(rightBar);
  _bars.insert(std::make_pair(PlayerSide::RightPlayer, rightBar));
}

BarObject * GameplayLayer::createBar(const std::string & filename, const cocos2d::Size & barSize)
{
  BarObject * bar = BarObject::create(filename, barSize.width, barSize.height);

  cocos2d::PhysicsBody * boundingBox = cocos2d::PhysicsBody::createBox(barSize, cocos2d::PhysicsMaterial(1.0f, 1.0f, 0.0f));
  boundingBox->setDynamic(false);
  boundingBox->setCategoryBitmask(PhysicsCategory::BarBitmask);
  boundingBox->setCollisionBitmask(PhysicsCategory::ArenaBitmask | PhysicsCategory::BallBitmask);
  boundingBox->setContactTestBitmask(PhysicsCategory::ArenaBitmask | PhysicsCategory::BallBitmask);
  bar->setPhysicsBody(boundingBox);

  return bar;
}

BallObject * GameplayLayer::createBall(const std::string & filename)
{
  BallObject * ball = BallObject::create(filename);

  return ball;
}

cocos2d::PhysicsBody * GameplayLayer::createBallBounding(float radius)
{
  cocos2d::PhysicsBody * boundingCircle = cocos2d::PhysicsBody::createCircle(radius, cocos2d::PhysicsMaterial(0.1f, 1.0f, 0.0f));
  boundingCircle->setDynamic(true);
  boundingCircle->setCategoryBitmask(PhysicsCategory::BallBitmask);
  boundingCircle->setCollisionBitmask(PhysicsCategory::ArenaBitmask | PhysicsCategory::BarBitmask | PhysicsCategory::LeftGoalBitmask | PhysicsCategory::RightGoalBitmask);
  boundingCircle->setContactTestBitmask(PhysicsCategory::ArenaBitmask | PhysicsCategory::BarBitmask | PhysicsCategory::LeftGoalBitmask | PhysicsCategory::RightGoalBitmask);

  return boundingCircle;
}

cocos2d::Node * GameplayLayer::createGoal(float length, int type)
{
  cocos2d::Node * goal = cocos2d::Node::create();

  cocos2d::PhysicsBody * goalBounding = cocos2d::PhysicsBody::createEdgeSegment(cocos2d::Vec2(0, length / 2), cocos2d::Vec2(0, -length / 2), cocos2d::PhysicsMaterial(0.1f, 1.0f, 0.0f));
  goalBounding->setDynamic(false);
  goalBounding->setCategoryBitmask(type);
  goalBounding->setCollisionBitmask(PhysicsCategory::BallBitmask);
  goalBounding->setContactTestBitmask(PhysicsCategory::BallBitmask);
  goal->setPhysicsBody(goalBounding);

  return goal;
}

cocos2d::ui::Widget * GameplayLayer::createInvisibleButton(const cocos2d::Size & contentSize, const std::function<void(cocos2d::Ref *, cocos2d::ui::Widget::TouchEventType)> & callback)
{
  cocos2d::ui::Widget * invisibleButton = cocos2d::ui::Widget::create();
  invisibleButton->setContentSize(contentSize);
  invisibleButton->addTouchEventListener(callback);

  return invisibleButton;
}

void GameplayLayer::prepareGame()
{
  this->unscheduleUpdate();
  _disableInput = true;

  auto arena = this->getChildByName("arena");
  cocos2d::Size playSize = arena->getContentSize();

  auto leftBar = _bars.at(PlayerSide::LeftPlayer);
  leftBar->setBarDirection(BarDirection::None);
  leftBar->restrictDirection(BarDirection::None);
  leftBar->setPosition(cocos2d::Vec2(leftBar->getContentSize().width / 2 + 60, playSize.height / 2));

  auto rightBar = _bars.at(PlayerSide::RightPlayer);
  rightBar->setBarDirection(BarDirection::None);
  rightBar->restrictDirection(BarDirection::None);
  rightBar->setPosition(cocos2d::Vec2(playSize.width - rightBar->getContentSize().width / 2 - 60, playSize.height / 2));

  if (_ball) {
    _ball->removeFromParentAndCleanup(true);
  }

  _ball = this->createBall("ball.png");
  _ball->setTag(ObjectTag::BallTag);
  _ball->setSpeed(0.0);
  _ball->setDirection(cocos2d::Vec2::ZERO);
  _ball->setPosition(cocos2d::Vec2(playSize.width / 2, playSize.height / 2));
  arena->addChild(_ball);

  _countdownTime = COUNTDOWN_START_TIME;
  this->setCountdownText((int)std::ceilf(_countdownTime));

  _countdownText->runAction(cocos2d::FadeIn::create(0.0f));
  this->schedule(CC_CALLBACK_1(GameplayLayer::countdownUpdate, this), 1.0f, "countdown_update");
}

void GameplayLayer::startGame()
{
  cocos2d::Vec2 ballDirection = cocos2d::Vec2(cocos2d::RandomHelper::random_real<float>(-100.0, 100.0), cocos2d::RandomHelper::random_real<float>(-100.0, 100.0));
  ballDirection.normalize();

  _bars.at(PlayerSide::LeftPlayer)->setSpeed(SPEED);
  _bars.at(PlayerSide::RightPlayer)->setSpeed(SPEED);

  cocos2d::PhysicsBody * boundingCircle = this->createBallBounding(15.0);
  _ball->setPhysicsBody(boundingCircle);
  _ball->setSpeed(1.5 * SPEED);
  _ball->setDirection(ballDirection);

  _disableInput = false;

  this->scheduleUpdate();
}

void GameplayLayer::changeBarDirection(BarObject * bar, BarDirection dir)
{
  if (bar != nullptr) {
    bar->setBarDirection(dir);
  }
}

void GameplayLayer::restrictPreviousBarDirection(BarObject * bar)
{
  if (bar != nullptr) {
    BarDirection prevDirection = bar->getBarDirection();
    bar->restrictDirection(prevDirection);
    this->changeBarDirection(bar, BarDirection::None);
  }
}

void GameplayLayer::setCountdownText(int time)
{
  std::stringstream ss;
  ss << time;

  _countdownText->setString(ss.str());
}

void GameplayLayer::scoreGoal(PlayerSide side)
{
  int score = _gameManager->getPlayerScore(side) + 1;
  _gameManager->setPlayerScore(side, score);
  this->prepareGame();
  _eventDispatcher->dispatchCustomEvent("update_score");
}

void GameplayLayer::onEnter()
{
  cocos2d::Layer::onEnter();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
  auto listener = cocos2d::EventListenerKeyboard::create();
  listener->onKeyPressed = CC_CALLBACK_2(GameplayLayer::onKeyPressed, this);
  listener->onKeyReleased = CC_CALLBACK_2(GameplayLayer::onKeyReleased, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#endif

  cocos2d::EventListenerPhysicsContact * contactListener = cocos2d::EventListenerPhysicsContact::create();
  contactListener->onContactBegin = CC_CALLBACK_1(GameplayLayer::onContactBegin, this);
  contactListener->onContactPostSolve = CC_CALLBACK_1(GameplayLayer::onContactPostSolve, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameplayLayer::countdownUpdate(float dt)
{
  if (!_isPaused) {
    _countdownTime -= dt;
    this->setCountdownText((int)std::ceilf(_countdownTime));
    if (_countdownTime <= 0.0f) {
      _countdownText->runAction(cocos2d::FadeOut::create(0.2f));
      this->unschedule("countdown_update");
      this->startGame();
    }
  }
}

void GameplayLayer::update(float dt)
{
  cocos2d::Layer::update(dt);

  if (!_isPaused) {
    _gameManager->addTimer(-dt);
  }
}

bool GameplayLayer::onContactBegin(cocos2d::PhysicsContact & contact)
{
  auto shapeA = contact.getShapeA();
  auto shapeB = contact.getShapeB();
  
  if ((shapeA->getCategoryBitmask() | shapeB->getCategoryBitmask()) == (PhysicsCategory::ArenaBitmask | PhysicsCategory::BarBitmask)) {
    BarObject * bar = nullptr;
    if (shapeA->getCategoryBitmask() == PhysicsCategory::BarBitmask) {
      bar = dynamic_cast<BarObject*>(shapeA->getBody()->getNode());
    }
    else {
      bar = dynamic_cast<BarObject*>(shapeB->getBody()->getNode());
    }
    this->restrictPreviousBarDirection(bar);
  }
  return true;
}

void GameplayLayer::onContactPostSolve(cocos2d::PhysicsContact & contact)
{
  auto shapeA = contact.getShapeA();
  auto shapeB = contact.getShapeB();

  if ((shapeA->getCategoryBitmask() | shapeB->getCategoryBitmask()) == (PhysicsCategory::BallBitmask | PhysicsCategory::LeftGoalBitmask)) {
    this->scoreGoal(PlayerSide::RightPlayer);
  }
  else if ((shapeA->getCategoryBitmask() | shapeB->getCategoryBitmask()) == (PhysicsCategory::BallBitmask | PhysicsCategory::RightGoalBitmask)) {
    this->scoreGoal(PlayerSide::LeftPlayer);
  }
}

void GameplayLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
  if (_disableInput) {
    return;
  }
  switch (keyCode) {
  case cocos2d::EventKeyboard::KeyCode::KEY_W:
    this->changeBarDirection(_bars.at(PlayerSide::LeftPlayer), BarDirection::Up);
    break;
  case cocos2d::EventKeyboard::KeyCode::KEY_S:
    this->changeBarDirection(_bars.at(PlayerSide::LeftPlayer), BarDirection::Down);
    break;
  case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    this->changeBarDirection(_bars.at(PlayerSide::RightPlayer), BarDirection::Up);
    break;
  case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    this->changeBarDirection(_bars.at(PlayerSide::RightPlayer), BarDirection::Down);
    break;
  default:
    break;
  }
}

void GameplayLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
  if (_disableInput) {
    return;
  }
  switch (keyCode) {
  case cocos2d::EventKeyboard::KeyCode::KEY_W:
  case cocos2d::EventKeyboard::KeyCode::KEY_S:
    this->changeBarDirection(_bars.at(PlayerSide::LeftPlayer), BarDirection::None);
    break;
  case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
  case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    this->changeBarDirection(_bars.at(PlayerSide::RightPlayer), BarDirection::None);
    break;
  default:
    break;
  }
}

void GameplayLayer::onUpperButtonTouched(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType eventType)
{
  if (_disableInput) {
    return;
  }
  cocos2d::Node * obj = dynamic_cast<cocos2d::Node *>(ref);
  PlayerSide side = (PlayerSide)obj->getTag();
  switch (eventType) {
  case cocos2d::ui::Widget::TouchEventType::BEGAN:
    this->changeBarDirection(_bars.at(side), BarDirection::Up);
    break;
  case cocos2d::ui::Widget::TouchEventType::CANCELED:
  case cocos2d::ui::Widget::TouchEventType::ENDED:
    this->changeBarDirection(_bars.at(side), BarDirection::None);
    break;
  default:
    break;
  }
}

void GameplayLayer::onBottomButtonTouched(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType eventType)
{
  if (_disableInput) {
    return;
  }
  cocos2d::Node * obj = dynamic_cast<cocos2d::Node *>(ref);
  PlayerSide side = (PlayerSide)obj->getTag();
  switch (eventType) {
  case cocos2d::ui::Widget::TouchEventType::BEGAN:
    this->changeBarDirection(_bars.at(side), BarDirection::Down);
    break;
  case cocos2d::ui::Widget::TouchEventType::CANCELED:
  case cocos2d::ui::Widget::TouchEventType::ENDED:
    this->changeBarDirection(_bars.at(side), BarDirection::None);
    break;
  default:
    break;
  }
}
