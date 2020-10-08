#include "Inventory.h"

int Inventory::_counter{0};

Inventory::Inventory(){
  Items.resize(9);
  for(int i = 0; i < 9; i++)
    Items[i] = new Item();
  _counter++;
}

void Inventory::Draw(){

}

void Inventory::SetItemAtSlot(Item* NewItem, int Pos){
  Items[Pos] = NewItem;
}

void Inventory::CreateSlot(){

}

void Inventory::RemoveItem(int Pos){
  delete Items[Pos];
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
  //for(int i = 0; i < Items.size(); i++){
  //  delete Items[i];
  //}
  std::cout << "\t\t Inventory Count --> " << _counter <<std::endl;
  //  Items.clear();
}
