#include "DynamicObject.h"

DynamicObject::DynamicObject() :
  _velocity(0.0)
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

double DynamicObject::getVelocity() const
{
  return _velocity;
}

void DynamicObject::setVelocity(double velocity)
{
  _velocity = velocity;
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
