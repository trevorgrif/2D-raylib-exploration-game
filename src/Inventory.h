#ifndef INVENTORY_H
#define INVENTORY_H

#include "raylib.h"
#include "Item.h"
#include "Block.h"
#include <iostream>

class Inventory{
 private:
  bool DisplayInv = true;
  int ActiveSlot{0};
  Item Items[10];
  
 public:
  Inventory();

  void Draw();
  void AddItem(Item NewItem, int Pos);
  void RemoveItem(Item OldItem);
  void DrawActiveItem(Vector2 pos,int row);
  void SetActiveSlot(int Pos);
  
  Item GetItem(int Pos);
  int GetActiveSlot();
};

#endif //INVENTORY_H
