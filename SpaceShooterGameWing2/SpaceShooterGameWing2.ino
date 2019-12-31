//code by bitluni give me a shout-out if you like it

#include <soc/rtc.h>
#include "AudioSystem.h"
#include "AudioOutput.h"
#include "Graphics.h"
#include "Image.h"
#include "SimplePALOutput.h"
#include "USBControllers.h"
#include "Sprites.h"
#include "Font.h"

//lincude graphics and sounds
namespace font88
{
#include "src/gfx/font.h"
}
Font font(8, 8, font88::pixels);
#include "src/gfx/ship.h"
#include "src/gfx/enemy1.h"
#include "src/gfx/blaster1.h"
#include "src/gfx/laser.h"
#include "src/gfx/rock.h"
#include "src/gfx/rocket.h"
#include "src/gfx/shot.h"
#include "src/gfx/explosion.h"
#include "src/gfx/exhaust.h"
#include "src/sfx/sounds.h"

#include "GameEntity.h"

////////////////////////////
//usb host pins
const int USB_HOST_RX = 22;
const int USB_HOST_TX = 21;
USBControllers controllers;

////////////////////////////
//audio configuration
const int SAMPLING_RATE = 10000;
const int BUFFER_SIZE = 4000;
AudioSystem audioSystem(SAMPLING_RATE, BUFFER_SIZE);
AudioOutput audioOutput;

///////////////////////////
//Video configuration
const int XRES = 320;
const int YRES = 240;
Graphics graphics(XRES, YRES);
SimplePALOutput composite;

void compositeCore(void *data)
{
  while (true)
  {
    composite.sendFrame(&graphics.frame);
  }
}

int stars[256][2] = {1000000};

int starsDraw()
{
  for(int i = 0; i < 256; i++)
  {
    stars[i][1] += i + 1;
    int y = stars[i][1] >> 6;
    if(y >= 240)
    {
      stars[i][0] = rand() % 320;
      stars[i][1] = 0;
      y = 0;
    }
    int c = graphics.rgb(32 + (i >> 3), 32 + (i >> 3), 32 + (i >> 3));
    for(int l = 0; l < 1 + i >> 6; l++)
    {
      graphics.dot(stars[i][0], y + l, c);
    }
  }
}

int trackAngle(int dx, int dy, int angle)
{
  float c = dx * sin(angle * M_PI / 180) - dy * cos(angle * M_PI / 180);
  if(c < 0) return 1;
  if(c > 0) return -1;
  return 0;
}

#include "Animation.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Player.h"
Player player(ship, 160, 200);

void setup()
{
  Serial.begin(115200);
  //rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);              //highest cpu frequency

  //initialize composite output and graphics
  composite.init();
  graphics.init();
  graphics.setFont(font);
  xTaskCreatePinnedToCore(compositeCore, "c", 1024, NULL, 1, NULL, 0);

  //initialize audio output
  audioOutput.init(audioSystem);

  //initialize controllers
  controllers.init(USB_HOST_RX, USB_HOST_TX);
  controllers.setController(0); //first controller

  //Play first sound in loop (music)
  //sounds.play(audioSystem, 1, 1, 1, true);
}

void enemiesAct(int dt)
{
   for(int i = 0; i < 10; i++)
  {
    if(enemies[i])
      if(!enemies[i]->act(dt))
      {
        for(int j = 0; j < 100; j++)
          if(projectiles[j])
            projectiles[j]->targetKilled(enemies[i]);
        player.targetKilled(enemies[i]);
        delete enemies[i];
        enemies[i] = 0;
      }
  }
}

void hitTest()
{
  for(int i = 0; i < 100; i++)
    if(projectiles[i])
    {
      for(int j = 0; j < 10; j++)
      if(enemies[j])
      {
        if(abs((projectiles[i]->x >> 16) - (enemies[j]->x >> 16)) < projectiles[i]->size &&
          abs((projectiles[i]->y >> 16) - (enemies[j]->y >> 16)) < projectiles[i]->size
        )
        {
          animationsEmit(new Animation(exhaust, projectiles[i]->x >> 16, projectiles[i]->y >> 16, 0, 7, 20, 0));
          sounds.play(audioSystem, 6, 0.3f, 2);
          enemies[j]->life -= projectiles[i]->power;
          delete projectiles[i];
          projectiles[i] = 0;
          break;
        }
      }
    }  
}

void loop()
{
  static int time = 0;
  int t = millis();
  int dt = t - time;

  if((time / 1000) != (t / 1000))
    enemiesEmit(new Enemy1((rand() & 127) + 100, -20, (rand() & 1023) - 512, 3000));
  if((time / 1000) != (t / 1000))
    enemiesEmit(new Enemy2((rand() & 63) + 130, -20, (rand() & 2047) - 1024, 2500));
  
  time = t;

  //fill audio buffer
  audioSystem.calcSamples();
  controllers.poll();

  projectilesAct(dt);
  enemiesAct(dt);
  player.act(dt);
  animationsAct(dt);
  hitTest();
  graphics.begin(0);
  static int xpos = 0;
  xpos = (xpos +1 ) & 255;

  starsDraw();

  enemiesDraw();
  projectilesDraw();
  player.draw(graphics);
  animationsDraw();
  graphics.setCursor(16, 16);
  /*graphics.fillRect(0,0,2,240,graphics.rgb(255, 255, 255));
  graphics.fillRect(318,0,2,240,graphics.rgb(255, 255, 255));*/
  //graphics.setTextColor(graphics.rgb(255, 255, 255));
  //graphics.print("Hello!\n");
  /*graphics.print(player.gunPosition[0]);
  graphics.print("\n");
  graphics.print(player.gunPosition[1]);*/
  graphics.end();
}
