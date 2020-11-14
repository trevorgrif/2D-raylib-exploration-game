#include "Inventory.h"

int Inventory::_counter{0};

Inventory::Inventory(){
  Items.resize(9);
  for(int i = 0; i < 7; i++){
    Items[i] = new Empty();
  }
  Items[7] = new Wood_Sword();
  Items[8] = new Wood_Axe();
}

Inventory::~Inventory(){
  for(auto i : Items){
    delete i;
  }
  Items.clear();
}

void Inventory::Draw(Camera2D * Camera){
  Vector2 Dimension = GetScreenToWorld2D({GetScreenWidth()*78.5/80.0,GetScreenHeight()*8.0/10.0},*Camera);
  Vector2 Position = GetScreenToWorld2D({GetScreenWidth()*19.0/20.0,GetScreenHeight()/10.0},*Camera);
  Dimension.x = Dimension.x - Position.x;
  Dimension.y = Dimension.y - Position.y;
  Vector2 Spacing = {Dimension.x/10.0,Dimension.y/50.0};

  
  DrawRectangleV(Position,Dimension,CLITERAL(Color){0,0,0,200});
  for(int i = 0; i < 9; i++){ 
    DrawRectangleV({Position.x+Spacing.x,Position.y+(i*Dimension.y/9.0)+Spacing.y},{Dimension.x-(2*Spacing.x),Dimension.y/9.0 - (2*Spacing.y)},CLITERAL(Color){100,100,100,200});
    Items[i]->DrawAbove({Position.x+Spacing.x,Position.y+(i*Dimension.y/9.0)+Spacing.y},0);
  }
}

void Inventory::SetItemAtSlot(Object* NewItem, int Pos){
  Items[Pos] = NewItem;
}

void Inventory::CreateSlot(){

}

void Inventory::RemoveItem(int Pos){
  delete Items[Pos];
}

bool Inventory::IsOpen(){
  return Open;
}

Object* Inventory::GetItem(int Pos){return Items[Pos];}

Object* Inventory::GetActiveItem(){return Items[ActiveSlot];}

int Inventory::GetActiveSlot(){return ActiveSlot;}

int Inventory::AvailableSpace(){ //Return position of free slot plus 1 so that 0 means not found
  for(int i = 0; i < Items.size(); i++){
    if(Items[i]->GetType() == global_enums::OBJECTS::Empty){
      return i+1;
    }
  }
  return 0;
}
void Inventory::SetActiveSlot(int Pos){ActiveSlot = Pos;}

int Inventory::GetSize(){
  return Items.size();
}
void Inventory::SwitchOpen(){
  Open = !Open;
}


