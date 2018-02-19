#include "BallObject.h"

BallObject::BallObject() :
  DynamicObject()
{
}

BallObject::~BallObject()
{
}

BallObject * BallObject::create(const std::string & filename)
{
  BallObject * obj = new BallObject();

  if (obj && obj->initWithFile(filename)) {
    obj->autorelease();
    return obj;
  }

  CC_SAFE_DELETE(obj);
  return nullptr;
}
