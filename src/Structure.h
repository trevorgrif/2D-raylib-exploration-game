#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "raylib.h"
#include "Block.h"
#include <map>
#include <iostream>

class Structure{
private:
  Rectangle body;
  Color color{BLUE};
  float health{100};
  bool fill{true};
  std::map<Vector2,Block*,Vec2Compare>* map;
public:
  Structure(){}; //Needed to make template LL work
  Structure(Rectangle body,std::map<Vector2,Block*,Vec2Compare>* map,BlockType type_oh_block);
  Structure(Rectangle body,std::map<Vector2,Block*,Vec2Compare>* map,BlockType type_oh_block, Color color);
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
