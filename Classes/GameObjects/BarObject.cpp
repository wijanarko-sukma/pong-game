#include "BarObject.h"

BarObject::BarObject() :
  DynamicObject()
{
}

BarObject::~BarObject()
{
}

BarObject * BarObject::create(const std::string & filename, double width, double height)
{
  BarObject * obj = new BarObject();
  
  if (obj && obj->init(filename, width, height)) {
    obj->autorelease();
    return obj;
  }

  CC_SAFE_DELETE(obj);

  return nullptr;
}

bool BarObject::init(const std::string & filename, double width, double height)
{
  if (DynamicObject::initWithFile(filename)) {
    this->setScale9Enabled(true);
    this->setContentSize(cocos2d::Size(width, height));
    return true;
  }

  return false;
}

void BarObject::setBarDirection(BarDirection direction)
{
  switch (direction)
  {
  case None:
    _direction.y = 0;
    break;
  case Up:
    _direction.y = 1;
    break;
  case Down:
    _direction.y = -1;
    break;
  default:
    break;
  }
}
