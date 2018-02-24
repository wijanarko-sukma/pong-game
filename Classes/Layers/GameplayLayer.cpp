#include "GameplayLayer.h"
#include "Global.h"

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
    this->scheduleUpdate();
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

  cocos2d::EventListenerPhysicsContact * contactListener = cocos2d::EventListenerPhysicsContact::create();
  contactListener->onContactBegin = CC_CALLBACK_1(GameplayLayer::onContactBegin, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
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
  double speed = 150.0;
  _leftBar->setSpeed(speed);
  _rightBar->setSpeed(speed);

  _ball->setSpeed(2 * speed);

  cocos2d::Vec2 ballDirection = cocos2d::Vec2(cocos2d::RandomHelper::random_real<float>(-100.0, 100.0), cocos2d::RandomHelper::random_real<float>(-100.0, 100.0));
  ballDirection.normalize();
  _ball->setDirection(ballDirection);
}

bool GameplayLayer::onContactBegin(cocos2d::PhysicsContact & contact)
{
  
  return true;
}

void GameplayLayer::onEnter()
{
  cocos2d::Layer::onEnter();
}

void GameplayLayer::update(float dt)
{
  cocos2d::Layer::update(dt);

}


