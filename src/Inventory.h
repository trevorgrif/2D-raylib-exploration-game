#ifndef INVENTORY_H
#define INVENTORY_H

#include "raylib.h"
#include "object.h"
#include <iostream>
#include <vector>

class Inventory{
 private:
  bool Open = false;
  int ActiveSlot{0};
  std::vector<Object*> Items;
  Object* EmptyItem;

  static int _counter;
  
 public:
  Inventory();
  ~Inventory();


  void Draw(Camera2D * Camera);
  void SetItemAtSlot(Object* NewItem, int Pos);
  void CreateSlot();
  void RemoveItem(int Pos);
  void SetActiveSlot(int Pos);
  void SwitchOpen();
  
  Object* GetItem(int Pos);
  Object* GetActiveItem();

  bool IsOpen();

  int AvailableSpace();
  
  int GetActiveSlot();
  int GetSize();
};

#endif //INVENTORY_H
