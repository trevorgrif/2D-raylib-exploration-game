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
  
 public:
  Item(){};
  Item(std::string ItemName,std::string TexturePath, float dure, float weight);
  std::string Name;
  void Draw();
  Texture2D GetItemTexture();
  
};

#endif //ITEM_H
