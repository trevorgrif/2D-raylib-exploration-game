#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "raylib.h"
#include <map>
#include "Chunk.h"
#include <iostream>

class Structure{
private:
  Rectangle body;
  Color color{BLUE};
  int health{100};
  bool fill{true};
  std::map<Vector2,Chunk*,Vec2Compare>* map;
public:
  Structure(){}; //Needed to make template LL work
  Structure(Rectangle body,std::map<Vector2,Chunk*,Vec2Compare>* map);
  Structure(Rectangle body,std::map<Vector2,Chunk*,Vec2Compare>* map,Color color);
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
