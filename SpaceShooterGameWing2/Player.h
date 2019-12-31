#pragma once
#include "GameEntity.h"


class Player : public GameEntity
{
  public:
  int cooldown[2];
  int gunPosition[2];
  Weapon *weapons[4];
  bool fire;
  Player(Sprites &sprites, int x, int y)
  {
    this->sprites = &sprites;
    this->x = x << 16;
    this->y = y << 16;
    vx = 0;
    vy = 0;
    cooldown[0] = cooldown[1] = 0;
    gunPosition[0] = gunPosition[1] = 0;
    for(int i = 0; i < 4; i++)
      weapons[i] = 0;
  }

  bool act(int dt)
  {
    const int topSpeed = 10000;
    const int cooldownValues[2] = {50, 500};

    if(controllers.pressed(0, USBControllers::R) && !weapons[0])
    {
      weapons[0] = new LaserBlaster(x >> 16, y >> 16, this, -20, 0, 0);
    }
    if(controllers.down(0, USBControllers::LEFT))
      vx -= dt * 100;
    if(controllers.down(0, USBControllers::RIGHT))
      vx += dt * 100;
    vx -= (vx / 10);
    vx = min(topSpeed, max(-topSpeed, vx));
    if(controllers.down(0, USBControllers::UP))
      vy -= dt * 100;
    if(controllers.down(0, USBControllers::DOWN))
      vy += dt * 100;
    vy -= (vy / 10);
    vy = min(topSpeed, max(-topSpeed, vy));
    
    cooldown[0] -= dt;
    cooldown[1] -= dt;
    
    if(controllers.down(0, USBControllers::A) && cooldown[0] <= 0)
    {
      cooldown[0] = cooldownValues[0];
      emitProjectile(new Shot((x >> 16) + gunPosition[0], (y >> 16) + gunPosition[1], 0, -20000));
      sounds.play(audioSystem, 6, 0.3f, 1);
    }
    fire = controllers.down(0, USBControllers::B);
    if(controllers.down(0, USBControllers::B) && cooldown[1] <= 0)
    {
      cooldown[1] = cooldownValues[1];
      emitProjectile(new Rocket((x >> 16), (y >> 16), -90, getClosestEnemy(x >> 16, y >> 16)));
      sounds.play(audioSystem, 4, 0.3, 1.5f);
    }
    x += vx * dt;
    y += vy * dt;

    for(int i = 0; i < 4; i++)
      if(weapons[i])
          weapons[i]->act(dt);
    return true;
  }

  void draw(Graphics &g)
  {
    int dx = 0, dy = 0;
    if(vx > 4000) dx = 1;
    if(vx > 9000) dx = 2;
    if(vx < -4000) dx = 3;
    if(vx < -9000) dx = 4;
    if(vy > 5000) dy = 5;
    if(vy < -5000) dy = 10;
    sprites->drawMix(g, dx + dy, x >> 16, y >> 16);
    gunPosition[0] = sprites->point(dx + dy, 1)[0] - sprites->point(dx + dy, 0)[0];
    gunPosition[1] = sprites->point(dx + dy, 1)[1] - sprites->point(dx + dy, 0)[1];
    for(int i = 0; i < 4; i++)
      if(weapons[i])
        weapons[i]->draw(g);
  }

  void targetKilled(GameEntity *e)
  {
    for(int i = 0; i < 4; i++)
      if(weapons[i])
        weapons[i]->targetKilled(e);
  }

};
