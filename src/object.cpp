#include "object.h"
#include <iostream>

int Object::_count = 0;

//Default Constructor
Object::Object(global_enums::OBJECTS ItemName,float dura, float weight){
  this->Durability = dura;
  this->Weight = weight;
  ObjectType = ItemName;
  Texture = Textures.GetTexture(ItemName);
  _count++;
  //std::cout << "Object Added " << _count << std::endl;
}

Object::~Object(){
  _count--;
  //std::cout << "Object Removed " << _count << std::endl;
}

void Object::SetPosX(float x){Position.x = x;}

void Object::SetPosY(float y){Position.y = y;}

Vector2 Object::GetPosition(){return Position;}

void Object::SetHealth(int NewHealth){Health = NewHealth;}

int Object::GetHealth(){return Health;}
int Object::GetWeight(){return Weight;}
int Object::GetDurability(){return Durability;}  

global_enums::OBJECTS Object::GetType(){ return ObjectType;}

void Object::DrawAbove(){
  DrawTextureRec(Texture, {0,0,16,16},{Position.x, Position.y}, RAYWHITE);
  //DrawRectangleLinesEx(GetHitBox(),1,BLACK);
}

void Object::DrawBelow(){
  DrawTextureRec(Texture, {0,0,16,16},{Position.x, Position.y}, RAYWHITE);
  //DrawRectangleLinesEx(GetHitBox(),1,BLACK);
}

void Object::DrawAbove(Vector2 Pos, int Dir){
  DrawTextureRec(Texture, {(float)Dir*16,0,16,16},{Pos.x, Pos.y}, RAYWHITE);
  //DrawRectangleLinesEx(GetHitBox(),1,BLACK);
}

void Object::DrawBelow(Vector2 Pos, int Dir){
  DrawTextureRec(Texture, {(float)Dir*16,0,16,16},{Pos.x, Pos.y}, RAYWHITE);
  //DrawRectangleLinesEx(GetHitBox(),1,BLACK);
}

std::vector<global_enums::OBJECT_ACTIONS> Object::HitByItem(Object* ItemPtr){
  std::vector<global_enums::OBJECT_ACTIONS> ActionList;
  ActionList.push_back(global_enums::OBJECT_ACTIONS::null);
  return ActionList;
}

/************************************************/
/*                TREE                          */
/************************************************/
Tree::Tree() : Object(global_enums::OBJECTS::Tree,100,100){
}

void Tree::DrawBelow(){
  DrawTextureRec(Texture,{0,21,32,11}, {Position.x,Position.y+21},RAYWHITE);
}

void Tree::DrawAbove(){
  DrawTextureRec(Texture,{0,0,32,21}, {Position.x,Position.y},RAYWHITE);
}

void Tree::DrawHitBox(){
  DrawRectangleLinesEx(GetHitBox(),2,BLACK);
}

Rectangle Tree::GetHitBox(){
  return {Position.x+12, Position.y + 16, 6, 16};
}

std::vector<global_enums::OBJECT_ACTIONS> Tree::HitByItem(Object* ItemPtr){
  std::vector<global_enums::OBJECT_ACTIONS> ActionList;
  switch(ItemPtr->GetType()){
  case global_enums::OBJECTS::Wood_Axe:
    Position.x = Position.x + 8;
    Position.y = Position.y + 16;
    ActionList.push_back(global_enums::OBJECT_ACTIONS::CREATE_LOG);
    ActionList.push_back(global_enums::OBJECT_ACTIONS::DEL);
    break;
  default:
    break;
  }
  return ActionList;
}

/************************************************/
/*                   Log                        */
/************************************************/

Log::Log() : Object(global_enums::OBJECTS::Log,100,90){
}

void Log::DrawBelow(){
  DrawTextureRec(Texture,{0,0,16,16}, {Position.x,Position.y},RAYWHITE);
}

void Log::DrawAbove(){}

void Log::DrawHitBox(){
  DrawRectangleLinesEx(GetHitBox(),2,BLACK);
}

Rectangle Log::GetHitBox(){
  return {Position.x, Position.y, 16, 16};
}

std::vector<global_enums::OBJECT_ACTIONS> Log::HitByItem(Object* ItemPtr){
  std::vector<global_enums::OBJECT_ACTIONS> ActionList;
  switch(ItemPtr->GetType()){
  case global_enums::OBJECTS::Wood_Axe:
    ActionList.push_back(global_enums::OBJECT_ACTIONS::CREATE_STICK);
    ActionList.push_back(global_enums::OBJECT_ACTIONS::DEL);
    break;
  default:
    break;
  }
  return ActionList;
}

/************************************************/
/*                   Stick                      */
/************************************************/

Stick::Stick() : Object(global_enums::OBJECTS::Stick,2,1){
}

void Stick::DrawBelow(){
  DrawTextureRec(Texture,{0,0,16,16}, {Position.x,Position.y},RAYWHITE);
}

void Stick::DrawAbove(){}

void Stick::DrawHitBox(){
  DrawRectangleLinesEx(GetHitBox(),2,BLACK);
}

Rectangle Stick::GetHitBox(){
  return {Position.x, Position.y, 16, 16};
}

std::vector<global_enums::OBJECT_ACTIONS> Stick::HitByItem(Object* ItemPtr){
  std::vector<global_enums::OBJECT_ACTIONS> ActionList;
  switch(ItemPtr->GetType()){
  case global_enums::OBJECTS::Wood_Axe:
    ActionList.push_back(global_enums::OBJECT_ACTIONS::DEL);
    break;
  default:
    break;
  }
  return ActionList;
}
