#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "raylib.h"
#include <iostream>

class Structure{
private:
  Rectangle body;
  Color color{BLUE};
  int health{100};
  bool fill{true};
public:
  Structure(){}; //Needed to make template LL work
  Structure(Rectangle body);
  Structure(Rectangle body,Color color);
  void draw();
  Rectangle getBody();
  int getHealth();
  void setFill(bool value);
  void setColor(Color color);
  float getX();
  float getY();
  float getWidth();
  float getHeight();
};

#endif //STRUCTURE_H