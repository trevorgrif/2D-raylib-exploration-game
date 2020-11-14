#include "Block.h"
#include <random>
#include <iostream>

Camera2D* Block::camera;

float blockLength = 16;

float modBlockLength(float x){
  if(x >= 0)
    return (float)((int)x - (int)x % (int)blockLength);
  else{
    if(((int)x % (int)blockLength) == 0)
      return (int)x;
    else
      return (float)((int)x - ((int)x % (int)blockLength) - blockLength);
  }
}

void Block::SetCamera(Camera2D* newCamera){
  camera = newCamera; 
}
//ITemTable is being loaded hundreds of times...
//Creating New Block Constructor
Block::Block(Rectangle body,float TerrNoiseValue,float BlueNoiseValue){
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(-4, 4);
  std::uniform_real_distribution<float> dist2(-6, 2);
  ShiftX = dist(mt);
  ShiftY = dist2(mt);

  this->body = body;
  this->TerrNoiseValue = TerrNoiseValue;
  this->BlueNoiseValue = BlueNoiseValue;
  EvaluateNoise();
}

// Loading Block Data Constructor
Block::Block(Rectangle body,float TerrNoiseValue,float BlueNoiseValue, float ShiftX, float ShiftY){
  this->body = body;
  this->TerrNoiseValue = TerrNoiseValue;
  this->BlueNoiseValue = BlueNoiseValue;
  this->ShiftX = ShiftX;
  this->ShiftY = ShiftY;
  EvaluateNoise();
}

void Block::EvaluateNoise(){
  if(TerrNoiseValue >= 0.25){
    setBlockType(global_enums::OBJECTS::Forest);
    return;
  }
  else if(TerrNoiseValue >= 0.01){
    setBlockType(global_enums::OBJECTS::Grass);
    return;
  }
  else if(TerrNoiseValue >= -0.05){
    setBlockType(global_enums::OBJECTS::SandyGrass);
    return;
  }
  else if(TerrNoiseValue >= -0.1){
    setBlockType(global_enums::OBJECTS::Sand);
    return;
  }
  else if(TerrNoiseValue >= -0.2){
    setBlockType(global_enums::OBJECTS::WetSand);
    return;
  }
  else if(TerrNoiseValue >= -0.4){
    setBlockType(global_enums::OBJECTS::ShallowWater);
    return;
  }
  else if(TerrNoiseValue >= -0.5){
    setBlockType(global_enums::OBJECTS::Water);
    return;
  }
  else if(TerrNoiseValue >= -1.0){
    setBlockType(global_enums::OBJECTS::DeepWater);
    return;
  }
  
}

void Block::setBlockType(global_enums::OBJECTS newType){
  this->block_type = newType;
  switch(block_type){
  case global_enums::OBJECTS::Forest:
    this->blocked = false;
    break;
  case global_enums::OBJECTS::Grass:
    this->blocked = false;
    break;
  case global_enums::OBJECTS::SandyGrass:
    this->blocked = false;
    break;
  case global_enums::OBJECTS::Sand:
    this->blocked = false;
    break;
  case global_enums::OBJECTS::WetSand:
    this->blocked = false;
    break;
  case global_enums::OBJECTS::ShallowWater:
    this->blocked = true;
    break;
  case global_enums::OBJECTS::Water:
    this->blocked = true;
    break;
  case global_enums::OBJECTS::DeepWater:
    this->blocked = true;
    break;
  }
}

void Block::drawBlock(){ //Only Draw Block if it's inside the view
  Vector2 ScreenUppLeft = GetScreenToWorld2D({0,0}, *camera);
  Vector2 ScreenBotRight = GetScreenToWorld2D({GetScreenWidth(),GetScreenHeight()},*camera);
  Rectangle view = {ScreenUppLeft.x - blockLength,ScreenUppLeft.y-blockLength,(ScreenBotRight.x - ScreenUppLeft.x)+blockLength, (ScreenBotRight.y - ScreenUppLeft.y)+blockLength};
  if(CheckCollisionPointRec(Vector2{body.x,body.y},view)){
    DrawTexture(Textures.GetTexture(block_type),this->body.x,this->body.y, RAYWHITE);
  }
  
}

global_enums::OBJECTS Block::getBlockType(){return block_type;}
float Block::GetBlueNoise(){return BlueNoiseValue;}

float Block::GetShiftX(){return ShiftX;}
float Block::GetShiftY(){return ShiftY;}
