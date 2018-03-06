#ifndef _GAME_UI_LAYER_
#define _GAME_UI_LAYER_

#include <map>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Gameplay/GameManager.h"

class GameUILayer : public cocos2d::LayerColor
{
CC_CONSTRUCTOR_ACCESS:
  GameUILayer(std::shared_ptr<GameManager> gameManager);
  ~GameUILayer();

public:
  static GameUILayer * create(std::shared_ptr<GameManager> gameManager);


protected:
  std::shared_ptr<GameManager> _gameManager;
  std::map<PlayerSide, cocos2d::ui::Text *> _scoreTexts;
  cocos2d::ui::Text * _timerText;

  void constructMenu();
  void setTimerText(int time);

  bool init() override;
  void onEnterTransitionDidFinish() override;
  void updateTimer(float dt);

  void listenRedButtonTouch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType eventType);
  void updateScoreCallback();
  void updateScoreForPlayer(PlayerSide side);
};

#endif