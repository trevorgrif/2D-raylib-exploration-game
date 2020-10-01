#include "Item.h"

int Item::_counter{0};

Item::Item(){
    _counter++;
  std::cout << _counter << "<-- Item Counter\n";
}

Item::Item(std::string name,std::string TexturePath, float dura, float weight){
  
  this->Name = name;
  this->Durability = dura;
  this->Weight = weight;
  this->ItemTexture = LoadTexture(TexturePath.c_str());
  _counter++;
  std::cout << _counter << "<-- Item Counter\n";
}

void Item::Draw(Vector2 Pos, int Dir){
  DrawTextureRec(ItemTexture, {Dir*16,0,16,16},{Pos.x, Pos.y}, RAYWHITE);
}

Texture2D Item::GetItemTexture(){return ItemTexture;}

Item::~Item(){
  _counter--;
  std::cout << _counter << "<-- Item Counter\n";
}
