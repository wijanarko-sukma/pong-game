#include "GameUILayer.h"

GameUILayer::GameUILayer(std::shared_ptr<GameManager> gameManager) :
  _gameManager(gameManager)
  , _timerText(nullptr)
{
}

GameUILayer::~GameUILayer()
{
}

GameUILayer * GameUILayer::create(std::shared_ptr<GameManager> gameManager)
{
  GameUILayer * layer = new GameUILayer(gameManager);

  if (layer && layer->init()) {
    layer->autorelease();
    return layer;
  }

  CC_SAFE_DELETE(layer);
  return nullptr;
}

bool GameUILayer::init()
{
  if (cocos2d::LayerColor::initWithColor(cocos2d::Color4B(255, 255, 255, 0))) {
    this->constructMenu();
    return true;
  }
  return false;
}

void GameUILayer::constructMenu()
{
  cocos2d::Director * director = cocos2d::Director::getInstance();
  cocos2d::Vec2 visibleOrigin = director->getVisibleOrigin();
  cocos2d::Size visibleSize = director->getVisibleSize();

  cocos2d::ui::Text * leftScoreText = cocos2d::ui::Text::create("0", "fonts/divlit.ttf", 120);
  leftScoreText->setTextColor(cocos2d::Color4B(0, 0, 0, 255));
  leftScoreText->enableOutline(cocos2d::Color4B(255, 255, 255, 255), 4);
  leftScoreText->setPosition(cocos2d::Vec2(visibleOrigin.x + visibleSize.width * 0.2f, visibleOrigin.y + visibleSize.height * 0.9f));
  this->addChild(leftScoreText);
  _scoreTexts.insert(std::make_pair(PlayerSide::LeftPlayer, leftScoreText));

  cocos2d::ui::Text * rightScoreText = cocos2d::ui::Text::create("0", "fonts/divlit.ttf", 120);
  rightScoreText->setTextColor(cocos2d::Color4B(0, 0, 0, 255));
  rightScoreText->enableOutline(cocos2d::Color4B(255, 255, 255, 255), 4);
  rightScoreText->setPosition(cocos2d::Vec2(visibleOrigin.x + visibleSize.width * 0.8f, visibleOrigin.y + visibleSize.height * 0.9f));
  this->addChild(rightScoreText);
  _scoreTexts.insert(std::make_pair(PlayerSide::LeftPlayer, leftScoreText));
  
  _timerText = cocos2d::ui::Text::create("0", "fonts/divlit.ttf", 120);
  _timerText->setTextColor(cocos2d::Color4B(0, 0, 0, 255));
  _timerText->enableOutline(cocos2d::Color4B(255, 255, 255, 255), 4);
  _timerText->setPosition(cocos2d::Vec2(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.9f));
  this->addChild(_timerText);
}

void GameUILayer::setTimerText(int time)
{
  std::stringstream ss;
  ss << time;

  _timerText->setString(ss.str());
}

void GameUILayer::onEnterTransitionDidFinish()
{
  cocos2d::LayerColor::onEnterTransitionDidFinish();
  this->setTimerText((int)std::ceilf(_gameManager->getTimer()));
  this->schedule(CC_CALLBACK_1(GameUILayer::updateTimer, this), 1.0f, "update_timer");
}

void GameUILayer::updateTimer(float dt)
{
  this->setTimerText((int)std::ceilf(_gameManager->getTimer()));
}
