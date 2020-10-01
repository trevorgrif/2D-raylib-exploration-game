#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include "Inventory.h"
#include "raylib.h"
#include <time.h>
#include <stdlib.h>
#include <map>

enum BlockType{
	       DarkForest,
	       Grass,
	       Sand,
	       ShallowWater,
	       Water,
	       DeepWater,
	       WetSand,
	       SandyGrass,
	       Undefined
};

extern float blockLength;
extern float modBlockLength(float x);

class Vec2Compare{
public:
  bool operator()(const Vector2& a,const Vector2& b) const {
    if( a.y < b.y)
      return true;
    else if(a.y > b.y)
      return false;
    else if(a.x < b.x)
      return true;
    else
      return false;
  }
};



class Block{
private:
  Rectangle body = Rectangle{0,0,0,0};
  bool blocked{false};
  Color color{GREEN};
  BlockType block_type;
  float NoiseValue;
  Vector2 GradValue;
  int chunkLength = 20;

  float ShiftX, ShiftY;

  Inventory* BlockInventory = new Inventory();

  static std::map<int,Texture2D>* BlockTextures;
  static std::map<std::string, Item*>* itemTable;
  static int _counter;
		  
public:
  Block(Rectangle body){this->body = body; this->block_type = Undefined;};
  Block(Rectangle body,float NoiseValue,std::map<std::string,Item*>* itemTable);
  Block(Rectangle body,float NoiseValue, float ShiftX, float ShiftY, std::map<std::string,Item*>* itemTable);
  ~Block();
  
  Rectangle getRect(){return this->body;};
  bool isBlocked(){return blocked;};
  void setX(float x){this->body.x = x;};
  void setY(float y){this->body.y = y;};
  void setColor(Color color){this->color = color;};
  Color getColor(){return this->color;};
  BlockType getBlockType();
  void setBlockType(BlockType newType);
  void drawBlock();
  void drawItem();
  void EvaluateNoise();
  void LoadTextures();
  void HitBy(Item* ActiveItem);
  float GetNoiseValue(){return NoiseValue;};
  float GetShiftX();
  float GetShiftY();
  
};

#endif //BLOCK_H
