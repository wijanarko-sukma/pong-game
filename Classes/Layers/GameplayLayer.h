#ifndef _GAMEPLAY_LAYER_
#define _GAMEPLAY_LAYER_

#include <map>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameObjects/BallObject.h"
#include "GameObjects/BarObject.h"
#include "Gameplay/GameManager.h"

class GameplayLayer : public cocos2d::Layer
{
CC_CONSTRUCTOR_ACCESS:
  GameplayLayer(std::shared_ptr<GameManager> gameManager);
  ~GameplayLayer();

public:
  static GameplayLayer * create(std::shared_ptr<GameManager> gameManager);
  bool init() override;

private:
  bool _isPaused;
  bool _disableInput;
  std::shared_ptr<GameManager> _gameManager;
  std::map<PlayerSide, BarObject *> _bars;
  BallObject * _ball;
  float _countdownTime;
  cocos2d::ui::Text * _countdownText;

  void constructArena();
  BarObject * createBar(const std::string & filename, const cocos2d::Size & barSize);
  BallObject * createBall(const std::string & filename, float radius);
  cocos2d::Node * createGoal(float length, int type);
  cocos2d::ui::Widget * createInvisibleButton(const cocos2d::Size & contentSize, const std::function<void(cocos2d::Ref *, cocos2d::ui::Widget::TouchEventType eventType)> & callback);

  void prepareGame();
  void startGame();
  void changeBarDirection(BarObject * bar, BarDirection dir);
  void restrictPreviousBarDirection(BarObject * bar);
  void countdownUpdate(float dt);
  void setCountdownText(int time);

  void onEnter() override;
  void update(float dt) override;
  bool onContactBegin(cocos2d::PhysicsContact & contact);
  void onContactPostSolve(cocos2d::PhysicsContact & contact);
  void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);

  void onUpperButtonTouched(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType eventType);
  void onBottomButtonTouched(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType eventType);
};

#endif