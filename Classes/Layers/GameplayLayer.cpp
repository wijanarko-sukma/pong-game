#include "GameplayLayer.h"
#include "Global.h"

constexpr float SPEED = 200.0;
constexpr float SPEED_LIMIT = 600.0;
constexpr float SPEED_ADDITION = 10.0;

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

GameplayLayer::GameplayLayer() :
  _leftBar(nullptr)
  , _rightBar(nullptr)
  , _ball(nullptr)
{
}

GameplayLayer::~GameplayLayer()
{
}

bool GameplayLayer::init()
{
  if (Layer::init()) {
    this->constructObjects();
    this->startGame();
    return true;
  }
  return false;
}

void GameplayLayer::constructObjects()
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
  this->addChild(arena);

  cocos2d::PhysicsBody * arenaBounding = cocos2d::PhysicsBody::createEdgeBox(playSize, cocos2d::PhysicsMaterial(0.1f, 1.0f, 0.0f));
  arenaBounding->setDynamic(false);
  arenaBounding->setCategoryBitmask(PhysicsCategory::ArenaBitmask);
  arenaBounding->setCollisionBitmask(PhysicsCategory::BarBitmask | PhysicsCategory::BallBitmask);
  arenaBounding->setContactTestBitmask(PhysicsCategory::BarBitmask | PhysicsCategory::BallBitmask);
  arena->setTag(ObjectTag::ArenaTag);
  arena->setPhysicsBody(arenaBounding);

  cocos2d::Node * leftGoal = this->createGoal(playSize.height, PhysicsCategory::LeftGoalBitmask);
  leftGoal->setPosition(cocos2d::Vec2(20, playSize.height / 2));
  arena->addChild(leftGoal);

  cocos2d::Node * rightGoal = this->createGoal(playSize.height, PhysicsCategory::RightGoalBitmask);
  rightGoal->setPosition(cocos2d::Vec2(playSize.width - 20, playSize.height / 2));
  arena->addChild(rightGoal);

  cocos2d::Size barSize = cocos2d::Size(30, 120);
  cocos2d::Size barBodySize = cocos2d::Size(barSize.width - 4, barSize.height - 4);

  _leftBar = this->createBar("bar_green.png", barSize);
  _leftBar->setPosition(cocos2d::Vec2(barSize.width / 2 + 60, playSize.height / 2));
  _leftBar->setTag(ObjectTag::LeftBarTag);
  arena->addChild(_leftBar);

  _rightBar = this->createBar("bar_orange.png", barSize);
  _rightBar->setPosition(cocos2d::Vec2(playSize.width - barSize.width / 2 - 60, playSize.height / 2));
  _rightBar->setTag(ObjectTag::RightBarTag);
  arena->addChild(_rightBar);

  _ball = this->createBall("ball.png", 15.0);
  _ball->setPosition(cocos2d::Vec2(playSize.width / 2, playSize.height / 2));
  _ball->setTag(ObjectTag::BallTag);
  arena->addChild(_ball);
}

BarObject * GameplayLayer::createBar(const std::string & filename, const cocos2d::Size & barSize)
{
  BarObject * bar = BarObject::create(filename, barSize.width, barSize.height);

  cocos2d::PhysicsBody * boundingBox = cocos2d::PhysicsBody::createBox(barSize, cocos2d::PhysicsMaterial(1.0f, 1.0f, 0.0f));
  boundingBox->setDynamic(false);
  boundingBox->setCategoryBitmask(PhysicsCategory::BarBitmask);
  boundingBox->setCollisionBitmask(PhysicsCategory::ArenaBitmask | PhysicsCategory::BallBitmask);
  boundingBox->setContactTestBitmask(PhysicsCategory::ArenaBitmask | PhysicsCategory::BallBitmask);
  boundingBox->setRotationEnable(false);
  bar->setPhysicsBody(boundingBox);

  return bar;
}

BallObject * GameplayLayer::createBall(const std::string & filename, float radius)
{
  BallObject * ball = BallObject::create(filename);

  cocos2d::PhysicsBody * boundingCircle = cocos2d::PhysicsBody::createCircle(radius, cocos2d::PhysicsMaterial(0.1f, 1.0f, 0.0f));
  boundingCircle->setDynamic(true);
  boundingCircle->setGravityEnable(false);
  boundingCircle->setRotationEnable(false);
  boundingCircle->setCategoryBitmask(PhysicsCategory::BallBitmask);
  boundingCircle->setCollisionBitmask(PhysicsCategory::ArenaBitmask | PhysicsCategory::BarBitmask);
  boundingCircle->setContactTestBitmask(PhysicsCategory::ArenaBitmask | PhysicsCategory::BarBitmask);
  ball->setPhysicsBody(boundingCircle);

  return ball;
}

cocos2d::Node * GameplayLayer::createGoal(float length, int type)
{
  cocos2d::Node * goal = cocos2d::Node::create();

  cocos2d::PhysicsBody * goalBounding = cocos2d::PhysicsBody::createEdgeSegment(cocos2d::Vec2(0, length / 2), cocos2d::Vec2(0, -length / 2), cocos2d::PhysicsMaterial(0.1f, 1.0f, 0.0f));
  goalBounding->setDynamic(false);
  goalBounding->setCategoryBitmask(type);
  goalBounding->setCollisionBitmask(PhysicsCategory::NoneBitmask);
  goalBounding->setContactTestBitmask(PhysicsCategory::BallBitmask);
  goal->setPhysicsBody(goalBounding);

  return goal;
}

void GameplayLayer::startGame()
{
  _leftBar->setSpeed(SPEED);
  _rightBar->setSpeed(SPEED);

  _ball->setSpeed(1.5 * SPEED);

  cocos2d::Vec2 ballDirection = cocos2d::Vec2(cocos2d::RandomHelper::random_real<float>(-100.0, 100.0), cocos2d::RandomHelper::random_real<float>(-100.0, 100.0));
  ballDirection.normalize();
  _ball->setDirection(ballDirection);

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
  _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameplayLayer::update(float dt)
{
  cocos2d::Layer::update(dt);

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
  else if ((shapeA->getCategoryBitmask() | shapeB->getCategoryBitmask()) == (PhysicsCategory::BarBitmask | PhysicsCategory::BallBitmask)) {
    DynamicObject * ball = nullptr;
    if (shapeA->getCategoryBitmask() == PhysicsCategory::BallBitmask) {
      ball = dynamic_cast<DynamicObject*>(shapeA->getBody()->getNode());
    }
    else {
      ball = dynamic_cast<DynamicObject*> (shapeB->getBody()->getNode());
    }
    float speed = ball->getSpeed() + SPEED_ADDITION;
    if (speed <= SPEED_LIMIT) {
      ball->setSpeed(speed);
    }
  }
  return true;
}

void GameplayLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
  switch (keyCode) {
  case cocos2d::EventKeyboard::KeyCode::KEY_W:
    this->changeBarDirection(_leftBar, BarDirection::Up);
    break;
  case cocos2d::EventKeyboard::KeyCode::KEY_S:
    this->changeBarDirection(_leftBar, BarDirection::Down);
    break;
  case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    this->changeBarDirection(_rightBar, BarDirection::Up);
    break;
  case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    this->changeBarDirection(_rightBar, BarDirection::Down);
    break;
  default:
    break;
  }
}

void GameplayLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
  switch (keyCode) {
  case cocos2d::EventKeyboard::KeyCode::KEY_W:
  case cocos2d::EventKeyboard::KeyCode::KEY_S:
    this->changeBarDirection(_leftBar, BarDirection::None);
    break;
  case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
  case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    this->changeBarDirection(_rightBar, BarDirection::None);
    break;
  default:
    break;
  }
}
