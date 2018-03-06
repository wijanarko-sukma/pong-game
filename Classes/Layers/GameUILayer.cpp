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
  _scoreTexts.insert(std::make_pair(PlayerSide::RightPlayer, rightScoreText));
  
  _timerText = cocos2d::ui::Text::create("0", "fonts/divlit.ttf", 120);
  _timerText->setTextColor(cocos2d::Color4B(0, 0, 0, 255));
  _timerText->enableOutline(cocos2d::Color4B(255, 255, 255, 255), 4);
  _timerText->setPosition(cocos2d::Vec2(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.9f));
  this->addChild(_timerText);

  cocos2d::ui::Button * redButton = cocos2d::ui::Button::create("button_red.png");
  redButton->setScale9Enabled(true);
  redButton->setTouchEnabled(true);
  redButton->setContentSize(cocos2d::Size(220, 60));
  redButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::listenRedButtonTouch, this));
  redButton->setPosition(cocos2d::Vec2(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.9f - redButton->getContentSize().height * 0.5f - 60));
  this->addChild(redButton);

  cocos2d::ui::Text * exitText = cocos2d::ui::Text::create("MAIN MENU", "fonts/divlit.ttf", 44);
  exitText->setTextColor(cocos2d::Color4B(255, 255, 255, 255));
  exitText->enableOutline(cocos2d::Color4B(0, 0, 0, 255), 4);
  exitText->setPosition(cocos2d::Vec2(redButton->getContentSize().width / 2, redButton->getContentSize().height / 2));
  redButton->addChild(exitText);
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

  _eventDispatcher->addCustomEventListener("update_score", CC_CALLBACK_0(GameUILayer::updateScoreCallback, this));
}

void GameUILayer::updateTimer(float dt)
{
  this->setTimerText((int)std::ceilf(_gameManager->getTimer()));
}

void GameUILayer::listenRedButtonTouch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType eventType)
{
  switch (eventType)
  {
  case cocos2d::ui::Widget::TouchEventType::ENDED: {
    cocos2d::Director::getInstance()->popScene();
  }
  default:
    break;
  }
}

void GameUILayer::updateScoreCallback()
{
  this->updateScoreForPlayer(PlayerSide::LeftPlayer);
  this->updateScoreForPlayer(PlayerSide::RightPlayer);
}

void GameUILayer::updateScoreForPlayer(PlayerSide side)
{
  int score = _gameManager->getPlayerScore(side);
  std::stringstream ss;

  ss << score;
  _scoreTexts.at(side)->setString(ss.str());
}
