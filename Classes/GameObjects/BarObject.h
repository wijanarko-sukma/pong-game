#ifndef _BOX_OBJECT_
#define _BOX_OBJECT_

#include <string>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameObjects/DynamicObject.h"

enum BarDirection
{
  None = 0,
  Up,
  Down
};

class BarObject : public DynamicObject
{
CC_CONSTRUCTOR_ACCESS:
  BarObject();
  ~BarObject();

public:
  static BarObject * create(const std::string & filename, double width, double height);
  void setBarDirection(BarDirection direction);

protected:
  bool init(const std::string & filename, double width, double height);
};

#endif