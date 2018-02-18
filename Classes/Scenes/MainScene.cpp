#include "MainScene.h"
#include "Layers/MainUILayer.h"

MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

bool MainScene::init()
{
  if (cocos2d::Scene::init())
  {
    auto mainLayer = MainUILayer::create();
    this->addChild(mainLayer);
    return true;
  }

  return false;
}

