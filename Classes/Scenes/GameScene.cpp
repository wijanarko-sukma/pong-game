#include "GameScene.h"
#include "Gameplay/GameManager.h"
#include "Layers/GameplayLayer.h"
#include "Layers/GameUILayer.h"

constexpr float GAME_TIMER = 60.0;

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
    std::shared_ptr<GameManager> gameManager = std::make_shared<GameManager>();
    gameManager->setTimer(GAME_TIMER);
    auto gameplayLayer = GameplayLayer::create(gameManager);
    this->addChild(gameplayLayer);
    return true;
  }

  return false;
}