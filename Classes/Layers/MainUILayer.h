#ifndef _MAIN_UI_LAYER_
#define _MAIN_UI_LAYER_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MainUILayer : public cocos2d::LayerColor
{
CC_CONSTRUCTOR_ACCESS:
  MainUILayer();
  ~MainUILayer();

public:
  virtual bool init();

  CREATE_FUNC(MainUILayer);

private:
  void constructMenu();
  void listenGreenButtonTouch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType eventType);
  void listenRedButtonTouch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType eventType);
};

#endif