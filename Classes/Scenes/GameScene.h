#ifndef _GAME_SCENE_
#define _GAME_SCENE_

#include "cocos2d.h"

class GameScene : public cocos2d::Scene
{
CC_CONSTRUCTOR_ACCESS:
  GameScene();
  ~GameScene();

public:
  bool init() override;

  CREATE_FUNC(GameScene);
};

#endif