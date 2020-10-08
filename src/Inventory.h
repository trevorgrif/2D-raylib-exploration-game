#ifndef INVENTORY_H
#define INVENTORY_H

#include "raylib.h"
#include "Item.h"
#include <iostream>
#include <vector>

class Inventory{
 private:
  bool DisplayInv = true;
  int ActiveSlot{0};
  std::vector<Item*> Items;
  Item* EmptyItem;

  static int _counter;
  
 public:
  Inventory();
  ~Inventory();


  void Draw();
  void SetItemAtSlot(Item* NewItem, int Pos);
  void CreateSlot();
  void RemoveItem(int Pos);
  void SetActiveSlot(int Pos);
  
  Item* GetItem(int Pos);
  Item* GetActiveItem();
  
  int GetActiveSlot();
  int GetSize();
};

#endif //INVENTORY_H
