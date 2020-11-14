#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include "Inventory.h"
#include "raylib.h"
#include "textures.h"
#include <time.h>
#include <stdlib.h>
#include <map>
#include <vector>

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

  global_enums::OBJECTS block_type;

  float TerrNoiseValue;
  float BlueNoiseValue;
  float ShiftX, ShiftY;

  Vector2 GradValue;

  int chunkLength = 20;

  static Camera2D* camera;
		  
public:
  Block(Rectangle body){this->body = body;};
  Block(Rectangle body, float TerrNoiseValue, float BlueNoiseValue);
  Block(Rectangle body, float TerrNoiseValue, float BlueNoiseValue, float ShiftX, float ShiftY);
  Block(){};
  
  Rectangle getRect(){return this->body;};
  bool isBlocked(){return blocked;};
  void setX(float x){this->body.x = x;};
  void setY(float y){this->body.y = y;};
  void setColor(Color color){this->color = color;};
  float GetBlueNoise();
  Color getColor(){return this->color;};
  global_enums::OBJECTS getBlockType();
  void setBlockType(global_enums::OBJECTS newType);
  void drawBlock();
  void EvaluateNoise();
  void SetCamera(Camera2D* newCamera);
  float GetNoiseValue(){return TerrNoiseValue;};
  float GetShiftX();
  float GetShiftY();
  
};

#endif //BLOCK_H
