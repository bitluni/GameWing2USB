#pragma once
#include "Graphics.h"

class GameEntity
{
  public:
  int x, y;
  int vx, vy;
  int life;
  int faction;
  Sprites *sprites;
  virtual bool act(int dt);
  virtual void draw(Graphics &g);
};

