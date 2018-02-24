#include "DynamicObject.h"

DynamicObject::DynamicObject() :
  _speed(0.0)
  , _direction(cocos2d::Vec2::ZERO)
{
}


DynamicObject::~DynamicObject()
{
}

DynamicObject * DynamicObject::create(const std::string & filename)
{
  DynamicObject * obj = new DynamicObject();

  if (obj && obj->initWithFile(filename)) {
    obj->autorelease();
    return obj;
  }

  CC_SAFE_DELETE(obj);
  return nullptr;
}

double DynamicObject::getSpeed() const
{
  return _speed;
}

void DynamicObject::setSpeed(double speed)
{
  _speed = speed;
}

const cocos2d::Vec2 & DynamicObject::getDirection() const
{
  return _direction;
}

void DynamicObject::setDirection(const cocos2d::Vec2 & direction)
{
  _direction = direction;
}

void DynamicObject::setDirection(double x, double y)
{
  _direction.x = x;
  _direction.y = y;
}
