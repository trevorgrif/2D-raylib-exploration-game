#ifndef OBJECT_H
#define OBJECT_H

#include "raylib.h"
#include "textures.h"
#include <string>
#include <vector>

//Object is the base class of all 'items' in the game. tools, trees, shrubbery, food, ect. . . 

class Object{
protected:
  global_enums::OBJECTS ObjectType;
  float Durability;
  float Weight;
  Vector2 Position{0,0};
  Texture2D Texture;
  int Health{0};
  static int _count;
  
public:
  Object(global_enums::OBJECTS ItemName, float dure, float weight);
  ~Object();
  
  virtual void DrawBelow();
  virtual void DrawAbove();
  virtual void DrawAbove(Vector2 Pos, int Dir);
  virtual void DrawBelow(Vector2 Pos, int Dir);
  virtual Rectangle GetHitBox() = 0;
  virtual std::vector<global_enums::OBJECT_ACTIONS> HitByItem(Object* ItemPtr); // Add item here
  
  void SetPosX(float x);
  void SetPosY(float y);
  void SetHealth(int NewHealth);

  global_enums::OBJECTS GetType();

  Vector2 GetPosition();
  int GetHealth();
  int GetWeight();
  int GetDurability();
};

class Empty : public Object{
public:
  Empty() : Object(global_enums::OBJECTS::Empty,0,0){};
  Rectangle GetHitBox(){return {Position.x,Position.y,8,6};};
};

class Wood_Sword : public Object{
public:
  Wood_Sword() : Object(global_enums::OBJECTS::Wood_Sword,10,5){};
  Rectangle GetHitBox(){return {Position.x,Position.y,8,6};};
};

class Wood_Axe : public Object{
public:
  Wood_Axe() : Object(global_enums::OBJECTS::Wood_Axe,10,5){};
  Rectangle GetHitBox(){return {Position.x,Position.y,8,6};};
};

class Tree : public Object{
 private:
  std::string Path;
  
 public:
  Tree();
  
  void DrawBelow();
  void DrawAbove();
  Rectangle GetHitBox();
  void DrawHitBox();
  std::vector<global_enums::OBJECT_ACTIONS> HitByItem(Object* ItemPtr);
  
};

class Log : public Object{
 private:
  std::string Path;
  
 public:
  Log();
  
  void DrawBelow();
  void DrawAbove();
  Rectangle GetHitBox();
  void DrawHitBox();
  std::vector<global_enums::OBJECT_ACTIONS> HitByItem(Object* ItemPtr);
  
};

class Stick : public Object{
 private:
  std::string Path;
  
 public:
  Stick();
  
  void DrawBelow();
  void DrawAbove();
  Rectangle GetHitBox();
  void DrawHitBox();
  std::vector<global_enums::OBJECT_ACTIONS> HitByItem(Object* ItemPtr);
  
};

#endif //OBJECT_H
