#include "Inventory.h"

int Inventory::_counter{0};

Inventory::Inventory(){
  _counter++;
}

void Inventory::Draw(){

}

void Inventory::SetItemAtSlot(Item* NewItem, int Pos){
  Items[Pos] = NewItem;
}

void Inventory::CreateSlot(){
  Items.push_back(new Item);
}

void Inventory::RemoveItem(Item* OldItem){

}

Item* Inventory::GetItem(int Pos){return Items[Pos];}

Item* Inventory::GetActiveItem(){return Items[ActiveSlot];}

int Inventory::GetActiveSlot(){return ActiveSlot;}

void Inventory::SetActiveSlot(int Pos){ActiveSlot = Pos;}

int Inventory::GetSize(){
  return Items.size();
}

Inventory::~Inventory(){
  _counter--;
  //for(auto p : Items)
  // delete p;
  //Items.clear();
}
