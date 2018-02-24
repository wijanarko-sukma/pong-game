#include "GameScene.h"
#include "Layers/GameplayLayer.h"

GameScene::GameScene()
{
}


GameScene::~GameScene()
{
}

GameScene * GameScene::createWithPhysics()
{
  GameScene * obj = new GameScene();

  if (obj && obj->initWithPhysics()) {
    obj->autorelease();
    return obj;
  }

  CC_SAFE_DELETE(obj);
  return nullptr;
}

bool GameScene::initWithPhysics()
{
  if (cocos2d::Scene::initWithPhysics()) {
    this->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    this->getPhysicsWorld()->setGravity(cocos2d::Vec2::ZERO);
    this->getPhysicsWorld()->setFixedUpdateRate(1 / 60.0);
    auto gameplayLayer = GameplayLayer::create();
    this->addChild(gameplayLayer);
    return true;
  }

  return false;
}