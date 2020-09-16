#include "Inventory.h"


Inventory::Inventory(){

}

void Inventory::Draw(){

}

void Inventory::AddItem(Item NewItem, int Pos){
  Items[Pos] = NewItem;
}

void Inventory::RemoveItem(Item OldItem){

}

void Inventory::DrawActiveItem(Vector2 pos, int col){
  DrawTextureRec(Items[GetActiveSlot()].GetItemTexture(), {col*blockLength,0,blockLength,blockLength},{pos.x, pos.y}, RAYWHITE);
}

Item Inventory::GetItem(int Pos){return Items[Pos];}

int Inventory::GetActiveSlot(){return ActiveSlot;}

void Inventory::SetActiveSlot(int Pos){ActiveSlot = Pos;}
