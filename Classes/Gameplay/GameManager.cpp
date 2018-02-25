#include "GameManager.h"

GameManager::GameManager() :
  _timer(0.0)
{
}

GameManager::~GameManager()
{
}

int GameManager::getPlayerScore(PlayerSide side) const
{
  int result = 0;
  if (_playerScore.count(side) > 0) {
    result = _playerScore.at(side);
  }
  return result;
}

void GameManager::setPlayerScore(PlayerSide side, int score)
{
  if (_playerScore.count(side) == 0) {
    _playerScore.insert(std::make_pair(side, 0));
  }
  _playerScore.at(side) = score;
}

float GameManager::getTimer() const
{
  return _timer;
}

void GameManager::setTimer(float timer)
{
  _timer = timer;
}

void GameManager::addTimer(float delta)
{
  _timer += delta;
}
