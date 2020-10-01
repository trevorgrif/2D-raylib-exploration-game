#ifndef ITEM_H
#define ITEM_H

#include "raylib.h"
#include <iostream>
#include <string>

class Item{
 private:
  //std::string Name;
  float Durability;
  float Weight;

  Texture2D ItemTexture;

  static int _counter;
  
 public:
  Item();
  ~Item();
  Item(std::string ItemName,std::string TexturePath, float dure, float weight);
  std::string Name;
  void Draw(Vector2 Pos, int Dir);
  Texture2D GetItemTexture();
};

#endif //ITEM_H
