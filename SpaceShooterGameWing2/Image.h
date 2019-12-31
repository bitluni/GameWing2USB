#pragma once
#include "Graphics.h"

class Image
{
  public:
  int xres;
  int yres;
  const unsigned short *pixels;
  
  void init(int xres, int yres, const unsigned short *pixels)
  {
    this->xres = xres;
    this->yres = yres;
    this->pixels = pixels;    
  }

  Image(){};
 
  Image(int xres, int yres, const unsigned short *pixels)
  {
    init(xres, yres, pixels);
  }

  ~Image()
  {
  }

  void draw(Graphics &g, int x, int y)
  {
    int i = 0;
    for(int py = 0; py < yres; py++)
      for(int px = 0; px < xres; px++)
        g.dot(px + x, py + y, pixels[i++]);
  }

  void draw(Graphics &g, int x, int y, int srcX, int srcY, int srcXres, int srcYres)
  {
    for(int py = 0; py < srcYres; py++)
    {
      int i = srcX + (py + srcY) * xres;
      for(int px = 0; px < srcXres; px++)
        g.dot(px + x, py + y, pixels[i++]);
    }
  }
  
  void drawAdd(Graphics &g, int x, int y)
  {
    int i = 0;
    for(int py = 0; py < yres; py++)
      for(int px = 0; px < xres; px++)
        g.dotAdd(px + x, py + y, pixels[i++]);
  }
  
  void drawMix(Graphics &g, int x, int y)
  {
    int i = 0;
    for(int py = 0; py < yres; py++)
      for(int px = 0; px < xres; px++)
        g.dotMix(px + x, py + y, pixels[i++]);
  }
  
  unsigned short get(int x, int y) const
  {
    return pixels[y * xres + x];
  }
};

