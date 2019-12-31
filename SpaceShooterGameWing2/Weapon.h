#pragma once
#include "GameEntity.h"

class Weapon : public GameEntity
{
  public:
  int cooldown;
  int dx, dy;
  GameEntity *parent;
  GameEntity *target;
  int angle;
  Weapon(Sprites &sprites, int x, int y, GameEntity *parent, int dx, int dy, int angle)
  {
    this->parent = parent;
    this->sprites = &sprites;
    this->angle = angle << 16;
    this->x = x << 16;
    this->y = y << 16;
    this->vx = 0;
    this->vy = 0;
    this->dx = dx << 16;
    this->dy = dy << 16;
    cooldown = 0;
    target = 0;
  }
  void targetKilled(GameEntity *e)
  {
    if(e == target)
      target = 0;
  }
};

class LaserBlaster : public Weapon
{
  public:
  int soundId;
  int active;
  int power;
  int cooldown2;
  LaserBlaster(int x, int y, GameEntity *parent, int dx, int dy, int angle)
    :Weapon(blaster1, x, y, parent, dx, dy, angle)
  {
    soundId = -1;
    active = 0;
    power = 800;
    cooldown2 = 0;
  }

  bool act(int dt)
  {
    cooldown -= dt;
    cooldown2 -= dt;
    active += dt;
    x = parent->x + dx;
    y = parent->y + dy;
    if(!target)
    {
      target = getClosestEnemy(x, y);
    }
    if(target)
    {
      angle += trackAngle( target->x - x, target->y - y, angle >> 16) * dt * 5000;
    }
    if(controllers.down(0, USBControllers::B) && cooldown2 <= 0)
    {
      active = true;
      if(cooldown <= 0)
      {
        cooldown += 100;
        emitProjectile(new Laser((x >> 16), (y >> 16), angle >> 16));
        if(soundId == -1)
          soundId = sounds.play(audioSystem, 0, 1, 1, true);
        power -= dt;
        if(power <= 0)
        {
          cooldown2 = 1000;
          power = 800;
        }
      }
    }
    else
    {
      active = false;
      if(soundId > -1)
      {
        sounds.stop(audioSystem, soundId);
        soundId = -1;
      }
    }
/*    x += vx * dt;
    y += vy * dt;*/
    true;
  }

  void draw(Graphics &g)
  {
    if(active)
    {
      float a = (this->angle >> 16) * M_PI / 180;
      int x0 = (x >> 16) - 1;
      int y0 = (y >> 16) - 1;
      int x1 = x0 + (int)(cos(a) * 320);
      int y1 = y0 + (int)(sin(a) * 320);
      g.line(x0 + 0, y0 + 0, x1 + 0, y1 + 0, g.rgb(200, 200, 255));
      g.line(x0 + 1, y0 + 0, x1 + 1, y1 + 0, g.rgb(200, 200, 255));
      g.line(x0 + 0, y0 + 1, x1 + 0, y1 + 1, g.rgb(200, 200, 255));
      g.line(x0 + 1, y0 + 1, x1 + 1, y1 + 1, g.rgb(200, 200, 255));
    }
    sprites->drawMix(g, ((((angle >> 14)-25) / 90 + 4) & 15) + (active?16:0), x >> 16, y >> 16);
  }
};
