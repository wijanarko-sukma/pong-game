#ifndef _BALL_OBJECT_
#define _BALL_OBJECT_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameObjects/DynamicObject.h"

class BallObject : public DynamicObject
{
CC_CONSTRUCTOR_ACCESS:
  BallObject();
  ~BallObject();

public:
  static BallObject * create(const std::string & filename);
};

#endif