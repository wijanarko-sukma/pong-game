#ifndef _DYNAMIC_OBJECT_
#define _DYNAMIC_OBJECT_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class DynamicObject : public cocos2d::ui::Scale9Sprite
{
CC_CONSTRUCTOR_ACCESS:
  DynamicObject();
  ~DynamicObject();

public:
  static DynamicObject * create(const std::string & filename);

  double getSpeed() const;
  virtual void setSpeed(double speed);
  const cocos2d::Vec2 & getDirection() const;
  virtual void setDirection(const cocos2d::Vec2 & direction);
  virtual void setDirection(double x, double y);

protected:
  double _speed;
  cocos2d::Vec2 _direction;
};

#endif