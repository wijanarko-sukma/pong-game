#ifndef _GAME_MANAGER_
#define _GAME_MANAGER_

#include <iostream>
#include <map>

enum PlayerSide
{
  LeftPlayer = 0,
  RightPlayer
};

class GameManager
{
public:
  GameManager();
  ~GameManager();

  int getPlayerScore(PlayerSide side) const;
  void setPlayerScore(PlayerSide side, int score);
  float getTimer() const;
  void setTimer(float timer);
  void addTimer(float delta);
protected:
  std::map<PlayerSide, int> _playerScore;
  float _timer;
};

#endif