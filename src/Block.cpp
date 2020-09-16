#include "Block.h"
#include <iostream>

std::map<int,Texture2D>* Block::BlockTextures = new std::map<int,Texture2D>;
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

Block::Block(Rectangle body,float NoiseValue){
  this->body = body;
  this->NoiseValue = NoiseValue;
  EvaluateNoise();
  LoadTextures();
  _counter++;
}

Block::Block(Rectangle body,BlockType btype){
  this->body = body;
  setBlockType(btype);
  LoadTextures();
  _counter++;
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
    DrawTexture(BlockTextures->find(4)->second,this->body.x,this->body.y,RAYWHITE);
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

void Block::HitBy(Item ActiveItem){
  if(ActiveItem.Name == "Wood Sword"){
    switch(block_type){
    case DarkForest:
      setBlockType(Grass);
      NoiseValue = 0.011;
    }
  }
}

BlockType Block::getBlockType(){return block_type;}


