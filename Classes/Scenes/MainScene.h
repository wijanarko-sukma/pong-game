#ifndef _MAIN_SCENE_
#define _MAIN_SCENE_

#include "cocos2d.h"

class MainScene : public cocos2d::Scene
{
public:
  MainScene();
  ~MainScene();

  virtual bool init();

  CREATE_FUNC(MainScene);
};

#endif