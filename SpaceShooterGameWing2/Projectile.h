#pragma once
#include "GameEntity.h"
#include "Animation.h"

class Projectile : public GameEntity
{
  public:
  int lifeTime;
  GameEntity *target;
  int power;
  int size;
  Projectile(Sprites &sprites, int x, int y, int power, int vx, int vy, int lifeTime, GameEntity *target = 0)
  {
    this->sprites = &sprites;
    this->x = x << 16;
    this->y = y << 16;
    this->vx = vx;
    this->vy = vy;
    this->lifeTime = lifeTime;
    this->target = target;
    this->power = power;
    size = 10;
  }
  
  void targetKilled(GameEntity *e)
  {
    if(e == target)
      target = 0;
  }
};

class Shot : public Projectile
{
  public:
  Shot(int x, int y, int vx, int vy)
    :Projectile(shot, x, y, 1, vx, vy, 1000)
  {
  }

  virtual bool act(int dt)
  {
    lifeTime -= dt;
    x += vx * dt;
    y += vy * dt;
    return lifeTime >= 0;
  }

  virtual void draw(Graphics &g)
  {
    sprites->drawMix(g, 0, x >> 16, y >> 16);
  }  
};

class Rocket : public Projectile
{
  public:
  int angle;
  int v;
  int smokeCooldown;
  Rocket(int x, int y, int angle, GameEntity *target)
    :Projectile(rocket, x, y, 3, 0, 0, 3000, target)
  {
    this->angle = angle << 16;
    v = 1000;
    smokeCooldown = 0;
  }

  virtual bool act(int dt)
  {
    lifeTime -= dt;
    smokeCooldown -= dt;
    if(smokeCooldown < 0)
    {
      smokeCooldown = 50;
      animationsEmit(new Animation(exhaust, x >> 16, y >> 16, 3, 7, 80, 0));
    }
    v += dt * 20;
    if(v > 20000) v = 20000;
    if(target)
    {
      angle += trackAngle( target->x - x, target->y - y, angle >> 16) * dt * 15000;
    }
    float a = (angle >> 16) * M_PI / 180;
    x += (cos(a) * v * dt);
    y += (sin(a) * v * dt);
    angle = angle % (360 << 16);
    return lifeTime >= 0;
  }

  virtual void draw(Graphics &g)
  {
    sprites->drawMix(g, (((angle >> 14) - 45) / 90 + 4) & 15, x >> 16, y >> 16);
  }  
};

class Laser : public Projectile
{
  public:
  int angle;
  int v;
  Laser(int x, int y, int angle)
    :Projectile(laser, x, y, 10, 0, 0, 500, 0)
  {
    this->angle = angle << 16;
    v = 80000;
    float a = (this->angle >> 16) * M_PI / 180;
    vx = (int)(cos(a) * v);
    vy = (int)(sin(a) * v);
    size = 30;
  }

  virtual bool act(int dt)
  {
    lifeTime -= dt;
    x += vx * dt;
    y += vy * dt;
    return lifeTime >= 0;
  }

  virtual void draw(Graphics &g)
  {
    //sprites->drawMix(g, ((angle >> 14) / 45 + 8) & 15, x >> 16, y >> 16);
  }  
};

Projectile *projectiles[100] = {0};

void emitProjectile(Projectile *p)
{
  for(int i = 0; i < 100; i++)
  {
    if(!projectiles[i])
    {
      projectiles[i] = p;
      return;
    }
  }
  //change to oldest
  delete p;
}

void projectilesAct(int dt)
{
  for(int i = 0; i < 100; i++)
  {
    if(projectiles[i])
      if(!projectiles[i]->act(dt))
      {
        delete projectiles[i];
        projectiles[i] = 0;
      }
  }
}

void projectilesDraw()
{
  for(int i = 0; i < 100; i++)
    if(projectiles[i])
      if(projectiles[i])
        projectiles[i]->draw(graphics);
}

