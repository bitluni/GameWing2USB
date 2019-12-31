#pragma once
#include "GameEntity.h"

class Enemy : public GameEntity
{
  public:
  int time;
  Enemy(Sprites &sprites, int x, int y, int vx, int vy)
  {
    this->sprites = &sprites;
    this->x = x << 16;
    this->y = y << 16;
    this->vx = vx;
    this->vy = vy;
    time = 0;
    life = 3;
  }
};

class Enemy1 : public Enemy
{
  public:
  int p[2];
  int ox, oy;
  Enemy1(int x, int y, int vx, int vy)
    :Enemy(enemy1, x, y, vx, vy)
  {
    p[0] = rand() & 64;
    p[1] = rand() & 31;
    ox = this->x;
    oy = this->y;
  }

  bool act(int dt)
  {
    time += dt;
    ox += vx * dt;
    oy += vy * dt;
    x = ox + ((int)(sin(time * 0.005f) * p[0])<< 16);
    y = oy + ((int)(cos(time * 0.005f) * p[1])<< 16);
    if(life <= 0)
    {
      sounds.play(audioSystem, 1, 0.5f);
      animationsEmit(new Animation(explosion, x >> 16, y >> 16, 0, 17, 20, 0));
    }
    return life > 0 && time < 5000;
  }

  void draw(Graphics &g)
  {
    sprites->drawMix(g, (time / 50) & 7, x >> 16, y >> 16);
  }
};

class Enemy2 : public Enemy
{
  public:
  Enemy2(int x, int y, int vx, int vy)
    :Enemy(rock, x, y, vx, vy)
  {
    life = 10;
  }

  bool act(int dt)
  {
    time += dt;
    x += vx * dt;
    y += vy * dt;
    if(life <= 0)
    {
      sounds.play(audioSystem, 1, 0.5f);
      animationsEmit(new Animation(explosion, x >> 16, y >> 16, 0, 17, 20, 0));
    }
    return life > 0 && time < 10000;
  }

  void draw(Graphics &g)
  {
    sprites->drawMix(g, (time / 50) & 15, x >> 16, y >> 16);
  }
};

Enemy *enemies[10] = {0};

void enemiesDraw()
{
    for(int i = 0; i < 10; i++)
    if(enemies[i])
      if(enemies[i])
        enemies[i]->draw(graphics);
}

void enemiesEmit(Enemy *e)
{
  for(int i = 0; i < 10; i++)
  {
    if(!enemies[i])
    {
      enemies[i] = e;
      return;
    }
  }
  delete e;
}

Enemy *getClosestEnemy(int x, int y)
{
  Enemy *e = 0;
  int d = 0x7fffffff;
  for(int i = 0; i < 10; i++)
  {
    if(enemies[i])
    {
      int dx = (enemies[i]->x >> 16) - x;
      int dy = (enemies[i]->y >> 16) - y;
      int cd = dx * dx + dy * dy;
      if(cd < d)
      {
        e = enemies[i];
        d = cd;
      }
    }
  }
  return e;
}


