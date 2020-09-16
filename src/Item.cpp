#include "Item.h"

Item::Item(std::string name,std::string TexturePath, float dura, float weight){
  
  this->Name = name;
  this->Durability = dura;
  this->Weight = weight;
  this->ItemTexture = LoadTexture(TexturePath.c_str());
  
}

void Item::Draw(){

}

Texture2D Item::GetItemTexture(){return ItemTexture;}
