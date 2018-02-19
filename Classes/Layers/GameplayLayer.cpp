#include "GameplayLayer.h"
#include "Global.h"

enum PhysicsCategory
{
  NonePhysics = 0,
  ArenaPhysics = (1 << 0),
  BarPhysics = (1 << 1),
  BallPhysics = (1 << 2),
  AllPhysics = ArenaPhysics | BarPhysics | BallPhysics
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
  cocos2d::Size playSize = cocos2d::Size(GAME_WIDTH_HD, GAME_HEIGHT_HD - 160);

  cocos2d::PhysicsBody * testBounding = cocos2d::PhysicsBody::createEdgeBox(gameSize);
  testBounding->setDynamic(false);
  this->setPhysicsBody(testBounding);

  cocos2d::ui::Widget * arena = cocos2d::ui::Widget::create();
  arena->setContentSize(playSize);
  arena->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
  arena->setPosition(cocos2d::Vec2(visibleOrigin.x + visibleSize.width / 2, (screenSize.height - gameSize.height) / 2 + playSize.height / 2));
  this->addChild(arena);

  cocos2d::PhysicsBody * arenaBounding = cocos2d::PhysicsBody::createEdgeBox(playSize);
  arenaBounding->setDynamic(false);
  arenaBounding->setCategoryBitmask(PhysicsCategory::ArenaPhysics);
  arenaBounding->setCollisionBitmask(PhysicsCategory::NonePhysics);
  arenaBounding->setContactTestBitmask(PhysicsCategory::BarPhysics | PhysicsCategory::BallPhysics);
  arena->setPhysicsBody(arenaBounding);

  cocos2d::Size barSize = cocos2d::Size(30, 100);
  cocos2d::Size barBodySize = cocos2d::Size(barSize.width - 4, barSize.height - 4);

  _leftBar = BarObject::create("bar_green.png", barSize.width, barSize.height);
  _leftBar->setPosition(cocos2d::Vec2(barSize.width / 2 + 20, playSize.height / 2));
  arena->addChild(_leftBar);

  cocos2d::PhysicsBody * leftBarBounding = cocos2d::PhysicsBody::createBox(barBodySize);
  leftBarBounding->setDynamic(false);
  leftBarBounding->setCategoryBitmask(PhysicsCategory::BarPhysics);
  leftBarBounding->setCollisionBitmask(PhysicsCategory::ArenaPhysics | PhysicsCategory::BallPhysics);
  leftBarBounding->setContactTestBitmask(PhysicsCategory::NonePhysics);
  _leftBar->setPhysicsBody(leftBarBounding);

  _rightBar = BarObject::create("bar_orange.png", barSize.width, barSize.height);
  _rightBar->setPosition(cocos2d::Vec2(playSize.width - barSize.width / 2 - 20, playSize.height / 2));
  arena->addChild(_rightBar);

  cocos2d::PhysicsBody * rightBarBounding = cocos2d::PhysicsBody::createBox(barBodySize);
  rightBarBounding->setDynamic(false);
  rightBarBounding->setCategoryBitmask(PhysicsCategory::BarPhysics);
  rightBarBounding->setCollisionBitmask(PhysicsCategory::ArenaPhysics | PhysicsCategory::BallPhysics);
  rightBarBounding->setContactTestBitmask(PhysicsCategory::NonePhysics);
  _rightBar->setPhysicsBody(rightBarBounding);

  _ball = BallObject::create("ball.png");
  _ball->setPosition(cocos2d::Vec2(playSize.width / 2, playSize.height / 2));
  arena->addChild(_ball);

  cocos2d::PhysicsBody * ballBounding = cocos2d::PhysicsBody::createCircle(15);
  ballBounding->setDynamic(false);
  ballBounding->setCategoryBitmask(PhysicsCategory::BallPhysics);
  ballBounding->setCollisionBitmask(PhysicsCategory::ArenaPhysics | PhysicsCategory::BarPhysics);
  ballBounding->setContactTestBitmask(PhysicsCategory::ArenaPhysics);
  _ball->setPhysicsBody(ballBounding);
}