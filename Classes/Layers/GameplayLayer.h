#ifndef _GAMEPLAY_LAYER_
#define _GAMEPLAY_LAYER_

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
  std::shared_ptr<GameManager> _gameManager;
  BarObject * _leftBar;
  BarObject * _rightBar;
  BallObject * _ball;

  void constructArena();
  BarObject * createBar(const std::string & filename, const cocos2d::Size & barSize);
  BallObject * createBall(const std::string & filename, float radius);
  cocos2d::Node * createGoal(float length, int type);
  void startGame();
  void changeBarDirection(BarObject * bar, BarDirection dir);
  void restrictPreviousBarDirection(BarObject * bar);

  void onEnter() override;
  void update(float dt) override;
  bool onContactBegin(cocos2d::PhysicsContact & contact);
  void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);
};

#endif