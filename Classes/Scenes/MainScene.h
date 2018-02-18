#ifndef _MAIN_SCENE_
#define _MAIN_SCENE_

#include "cocos2d.h"

class MainScene : public cocos2d::Scene
{
CC_CONSTRUCTOR_ACCESS:
  MainScene();
  ~MainScene();

  bool init() override;

  CREATE_FUNC(MainScene);
};

#endif