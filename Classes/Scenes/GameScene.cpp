#include "GameScene.h"

GameScene::GameScene()
{
}


GameScene::~GameScene()
{
}

bool GameScene::init()
{
  if (cocos2d::Scene::init()) {
    
    return true;
  }

  return false;
}