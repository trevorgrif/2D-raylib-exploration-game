#include "Block.h"
#include <random>
#include <iostream>

std::map<int,Texture2D>* Block::BlockTextures = new std::map<int,Texture2D>;
std::map<std::string, Item*>* Block::itemTable = new std::map<std::string,Item*>;
int Block::_counter = 0;

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
//ITemTable is being loaded hundreds of times...
//Creating New Block Constructor
Block::Block(Rectangle body,float NoiseValue,std::map<std::string,Item*>* itemTable){
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(-4, 4);
  std::uniform_real_distribution<float> dist2(-6, 2);
  ShiftX = dist(mt);
  ShiftY = dist2(mt);
  
  this->body = body;
  this->NoiseValue = NoiseValue;
  this->itemTable = itemTable;
  this->BlockInventory->CreateSlot();
  EvaluateNoise();
  LoadTextures();
  _counter++;
}

// Loading Block Data COnstructor
Block::Block(Rectangle body,float NoiseValue, float ShiftX, float ShiftY, std::map<std::string,Item*>* itemTable){
  this->body = body;
  this->NoiseValue = NoiseValue;
  this->itemTable = itemTable;
  this->ShiftX = ShiftX;
  this->ShiftY = ShiftY;
  this->BlockInventory->CreateSlot();
  EvaluateNoise();
  LoadTextures();
  _counter++;
}

Block::~Block(){
  delete BlockInventory;
}

void Block::LoadTextures(){
  if(_counter == 1){
    BlockTextures->insert({1,LoadTexture("textures/blocks/water.png")});
    BlockTextures->insert({2,LoadTexture("textures/blocks/sand.png")});
    BlockTextures->insert({3,LoadTexture("textures/blocks/grass.png")});
    BlockTextures->insert({4,LoadTexture("textures/blocks/forest.png")});
    BlockTextures->insert({5,LoadTexture("textures/blocks/wetsand.png")});
    BlockTextures->insert({6,LoadTexture("textures/blocks/sandygrass.png")});
  }
}

void Block::EvaluateNoise(){
  if(NoiseValue >= 0.2){
    setBlockType(DarkForest);
    BlockInventory->SetItemAtSlot(itemTable->find("Tree")->second,0);
    return;
  }
  else if(NoiseValue >= 0.01){
    setBlockType(Grass);
    return;
  }
  else if(NoiseValue >= -0.025){
    setBlockType(SandyGrass);
    return;
  }
  else if(NoiseValue >= -0.045){
    setBlockType(Sand);
    return;
  }
  else if(NoiseValue >= -0.07){
    setBlockType(WetSand);
    return;
  }
  else if(NoiseValue >= -0.1){
    setBlockType(ShallowWater);
    return;
  }
  else if(NoiseValue >= -0.2){
    setBlockType(Water);
    return;
  }
  else if(NoiseValue >= -2.0){
    setBlockType(DeepWater);
    return;
  }
  else{
    setBlockType(Undefined);
    return;
  }
}

void Block::setBlockType(BlockType newType){
  this->block_type = newType;
  switch(block_type){
  case DarkForest:
    this->blocked = false;
    break;
  case Grass:
    this->blocked = false;
    break;
  case SandyGrass:
    this->blocked = false;
    break;
  case Sand:
    this->blocked = false;
    break;
  case WetSand:
    this->blocked = false;
    break;
  case ShallowWater:
    this->blocked = true;
    break;
  case Water:
    this->blocked = true;
    break;
  case DeepWater:
    this->blocked = true;
    break;
  case Undefined:
    this->blocked = true;
    break;
  }
}

void Block::drawBlock(){
  switch(block_type){
  case DarkForest:
    DrawTexture(BlockTextures->find(3)->second,this->body.x,this->body.y,RAYWHITE);
    break;
  case Grass:
    DrawTexture(BlockTextures->find(3)->second,this->body.x,this->body.y,RAYWHITE);
    break;
  case SandyGrass:
    DrawTexture(BlockTextures->find(6)->second,this->body.x,this->body.y,RAYWHITE);
    break;
  case Sand:
    DrawTexture(BlockTextures->find(2)->second,this->body.x,this->body.y,RAYWHITE);
    break;
  case WetSand:
    DrawTexture(BlockTextures->find(5)->second,this->body.x,this->body.y,RAYWHITE);
    break;
  case ShallowWater:
    DrawTexture(BlockTextures->find(1)->second,this->body.x,this->body.y,RAYWHITE);
    break;
  case Water:
    DrawTexture(BlockTextures->find(1)->second,this->body.x,this->body.y,SKYBLUE);
    break;
  case DeepWater:
    DrawTexture(BlockTextures->find(1)->second,this->body.x,this->body.y,BLUE);
    break;
  case Undefined:
    DrawRectanglePro(this->body, Vector2{0,0}, 0.0f, YELLOW);
    std::cout << NoiseValue << std::endl;
    break;
  }
}

void Block::drawItem(){
  switch(block_type){
  case DarkForest:
    BlockInventory->GetActiveItem()->Draw({body.x+ ShiftX,body.y+ShiftY}, 1);
    break;
  }
}

void Block::HitBy(Item* ActiveItem){
  if(ActiveItem->Name == "Wood Sword"){
    switch(block_type){
    case DarkForest:
      setBlockType(Grass);
      NoiseValue = 0.011;
    }
  }
}

BlockType Block::getBlockType(){return block_type;}

float Block::GetShiftX(){return ShiftX;}
float Block::GetShiftY(){return ShiftY;}
