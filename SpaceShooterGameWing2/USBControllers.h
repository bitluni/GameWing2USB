//code by bitluni. It's public domain but I'm happy about shout outs nevertheless :-)

#pragma once
const int MAX_CONTROLLERS = 2;

const int PS3_UP =      0x0010;
const int PS3_DOWN =    0x0040;
const int PS3_LEFT =    0x0080;
const int PS3_RIGHT =   0x0020;
const int PS3_START =   0x0008;
const int PS3_SELECT =  0x0001;
const int PS3_A =       0x2000;
const int PS3_B =       0x4000;
const int PS3_X =       0x1000;
const int PS3_Y =       0x8000;
const int PS3_L =       0x0400;
const int PS3_R =       0x0800;

const int SNES_START =   0x2000;
const int SNES_SELECT =  0x1000;
const int SNES_A =       0x0020;
const int SNES_B =       0x0040;
const int SNES_X =       0x0010;
const int SNES_Y =       0x0080;
const int SNES_L =       0x0100;
const int SNES_R =       0x0200;

class USBControllers
{
  public:

  enum Type
  {
    SNES = 0,
    PS3 = 1,
  };
  
  enum Button
  {
    B = 0,
    Y = 1,
    SELECT = 2,
    START = 3,
    UP = 4,
    DOWN = 5,
    LEFT = 6,
    RIGHT = 7,
    A = 8,
    X = 9,
    L = 10,
    R = 11,
  };

  Type types[MAX_CONTROLLERS];
  int buttons[MAX_CONTROLLERS][12];

  ///This has to be initialized once for the shared pins latch and clock
  void init(int rx, int tx)
  {
    Serial1.begin(100000, SERIAL_8N1, rx, tx);    
    for(int c = 0; c < MAX_CONTROLLERS; c++)
    {
      for(int i = 0; i < 12; i++)
        buttons[c][i] = -1;
      types[c] = SNES;
    }
  }

  ///This sets the controller type and initializes its individual data pin
  void setController(int controller)
  {
    types[controller] = SNES;
  }

  void message(unsigned char *buffer, int len)
  {
    //ps3 
    //unsigned short b = *(unsigned short*)(&buffer[6]);
    //SNES 
    unsigned char h = buffer[7];
    unsigned char v = buffer[8];
    unsigned short b = *(unsigned short*)(&buffer[9]);
    int id = buffer[2];
    buttons[id][A] = (b & SNES_A) ? buttons[id][A] + 1 : -1;
    buttons[id][B] = (b & SNES_B) ? buttons[id][B] + 1 : -1;
    buttons[id][X] = (b & SNES_X) ? buttons[id][X] + 1 : -1;
    buttons[id][Y] = (b & SNES_Y) ? buttons[id][Y] + 1 : -1;
    buttons[id][L] = (b & SNES_L) ? buttons[id][L] + 1 : -1;
    buttons[id][R] = (b & SNES_R) ? buttons[id][R] + 1 : -1;
    buttons[id][START] = (b & SNES_START) ? buttons[id][START] + 1 : -1;
    buttons[id][SELECT] = (b & SNES_SELECT) ? buttons[id][SELECT] + 1 : -1;

    //Serial.println(h);
    buttons[id][LEFT] = (h < 127) ? buttons[id][LEFT] + 1 : -1;
    buttons[id][RIGHT] = (h > 127) ? buttons[id][RIGHT] + 1 : -1;
    buttons[id][UP] = (v < 127) ? buttons[id][UP] + 1 : -1;
    buttons[id][DOWN] = (v > 127) ? buttons[id][DOWN] + 1 : -1;
  }

  void poll()
  {
    static unsigned long n = 0;
    static unsigned long command = 0;
    static unsigned long length = 0;
    static unsigned char buffer[1024];
    static unsigned long signature;
    while(Serial1.available()) 
    {
      unsigned char d = Serial1.read();
      signature = (signature << 8) | d;
      if(signature == 0x6c756e69)
        n = 0;
      else
      if(n == 0)
      {
        length = d;
        n++;
      }
      else if(n == 1)
      {
        length |= ((unsigned long)d) << 8;    
        n++;
      }
      else if(n < length + 6 && n < 1024 - 2)
      {
        buffer[n - 2] = d;
        n++;
        if(n >= length + 6)
        {
          message(buffer, n - 2);
          n = 0xffff;
        }
      }
    }
  }

  ///button will be unpressed until released again
  void clear(int controller, Button b)
  {
    buttons[controller][b] = 0x80000000;
  }

  ///returns if button is currently down
  bool down(int controller, Button b) const
  {
    return buttons[controller][b] >= 0;
  }

  ///returns true if button state changed to down since previous poll. repeatAfterTics can be used to repeat after button was hold down for sme time
  bool pressed(int controller, Button b, int repeatAfterTics = 0x7fffffff) const
  {
    return buttons[controller][b] == 0 || (buttons[controller][b] >= repeatAfterTics);
  }

  ///returns the type of controller configured
  Type getType(int controller)
  {
    return types[controller];
  }
};
