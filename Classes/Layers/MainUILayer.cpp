#include "MainUILayer.h"
#include "Scenes/GameScene.h"

MainUILayer::MainUILayer()
{
}


MainUILayer::~MainUILayer()
{
}

bool MainUILayer::init()
{
  if (cocos2d::LayerColor::initWithColor(cocos2d::Color4B())) {
    this->constructMenu();
    return true;
  }

  return false;
}

void MainUILayer::constructMenu()
{
  cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
  cocos2d::Vec2 visibleOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();

  cocos2d::Size buttonSize = cocos2d::Size(320, 100);
  std::string fontName = "fonts/divlit.ttf";
  int fontSize = 44;

  cocos2d::ui::Text * title = cocos2d::ui::Text::create("PONG GAME", fontName, fontSize + 24);
  title->setTextColor(cocos2d::Color4B(0, 0, 0, 255));
  title->enableOutline(cocos2d::Color4B(255, 255, 255, 255), 4);
  title->setPosition(cocos2d::Vec2(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 0.75));
  this->addChild(title);

  cocos2d::ui::Button * greenButton = cocos2d::ui::Button::create("button_green.png");
  greenButton->setScale9Enabled(true);
  greenButton->setTouchEnabled(true);
  greenButton->setContentSize(buttonSize);
  greenButton->addTouchEventListener(CC_CALLBACK_2(MainUILayer::listenGreenButtonTouch, this));
  greenButton->setPosition(cocos2d::Vec2(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height / 2 + buttonSize.height / 2 - 20));
  this->addChild(greenButton);

  cocos2d::ui::Text * startText = cocos2d::ui::Text::create("START", fontName, fontSize);
  startText->setTextColor(cocos2d::Color4B(255, 255, 255, 255));
  startText->enableOutline(cocos2d::Color4B(0, 0, 0, 255), 4);
  startText->setPosition(cocos2d::Vec2(buttonSize.width / 2, buttonSize.height / 2));
  greenButton->addChild(startText);

  cocos2d::ui::Button * redButton = cocos2d::ui::Button::create("button_red.png");
  redButton->setScale9Enabled(true);
  redButton->setTouchEnabled(true);
  redButton->setContentSize(buttonSize);
  redButton->addTouchEventListener(CC_CALLBACK_2(MainUILayer::listenRedButtonTouch, this));
  redButton->setPosition(cocos2d::Vec2(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height / 2 - buttonSize.height / 2 - 40));
  this->addChild(redButton);

  cocos2d::ui::Text * exitText = cocos2d::ui::Text::create("EXIT", fontName, fontSize);
  exitText->setTextColor(cocos2d::Color4B(255, 255, 255, 255));
  exitText->enableOutline(cocos2d::Color4B(0, 0, 0, 255), 4);
  exitText->setPosition(cocos2d::Vec2(buttonSize.width / 2, buttonSize.height / 2));
  redButton->addChild(exitText);
}

void MainUILayer::listenGreenButtonTouch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType eventType)
{
  switch (eventType)
  {
  case cocos2d::ui::Widget::TouchEventType::ENDED: {
    auto scene = GameScene::createWithPhysics();
    cocos2d::Director::getInstance()->pushScene(scene);
  }
  default:
    break;
  }
}

void MainUILayer::listenRedButtonTouch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType eventType)
{
  switch (eventType)
  {
  case cocos2d::ui::Widget::TouchEventType::ENDED: {
    cocos2d::Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
  }
  default:
    break;
  }
}
