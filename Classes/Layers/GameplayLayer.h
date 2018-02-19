#ifndef _GAMEPLAY_LAYER_
#define _GAMEPLAY_LAYER_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameObjects/BallObject.h"
#include "GameObjects/BarObject.h"

class GameplayLayer : public cocos2d::Layer
{
CC_CONSTRUCTOR_ACCESS:
  GameplayLayer();
  ~GameplayLayer();

public:
  bool init() override;

  CREATE_FUNC(GameplayLayer);
private:
  BarObject * _leftBar;
  BarObject * _rightBar;
  BallObject * _ball;

  void constructObjects();
};

#endif