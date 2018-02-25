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
  BarDirection getBarDirection() const;
  void setBarDirection(BarDirection direction);
  void restrictDirection(BarDirection direction);

protected:
  BarDirection _barDirection;
  BarDirection _restriction;

  bool init(const std::string & filename, double width, double height);
};

#endif