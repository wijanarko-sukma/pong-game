#ifndef _GAME_SCENE_
#define _GAME_SCENE_

#include "cocos2d.h"

class GameScene : public cocos2d::Scene
{
public:
  GameScene();
  ~GameScene();

  virtual bool init();

  CREATE_FUNC(GameScene);
};

#endif