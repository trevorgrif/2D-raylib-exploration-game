#include "Chunk.h"
#include <random>

int Chunk::_count{0};

//Constuctor for Chunk when no chunk file found
Chunk::Chunk(Vector2 Pos, float TerrNoiseValues[256], float BlueNoiseArr[256]){
  //Allocated Memory for BlockList
  this->ChunkPos = Pos;
  BlockList.resize(256);
  for(int i = 0; i < 256; i++){
    int col = i % 16;
    int row = (i-(i%16))/16;
    BlockList[i] = new Block({Pos.x + col*blockLength,Pos.y + row*blockLength,blockLength,blockLength},TerrNoiseValues[i],BlueNoiseArr[i]);
  }
  _count++;
  //std::cout << "Chunk Added: " << _count << std::endl;
}

Chunk::~Chunk(){
  for(auto i : BlockList){
    delete i;
  }
  BlockList.clear();
  for(auto p : ObjList){
    delete p;
  }
  ObjList.clear();

  _count--;
  //std::cout << "Chunk Deleted: " << _count << std::endl;
}

void Chunk::GenerateChunkObjects(){
  for(int i = 0; i < 256; i++){
    int col = i % 16;
    int row = (i-(i%16))/16;
    AddTree(BlockList[i]);
  }
}



void Chunk::DrawChunk(){
  for(int i = 0; i < 256; i++){
    BlockList[i]->drawBlock();
  }
}

void Chunk::DrawChunkObjectsAbove(){
  for(int i = 0; i < ObjList.size(); i++){
    ObjList[i]->DrawAbove();
  }
}

void Chunk::DrawChunkObjectsBelow(){
  for(int i = 0; i < ObjList.size(); i++){
    ObjList[i]->DrawBelow();
  }
}

Block* Chunk::GetBlockByIndex(int i){
  return BlockList[i];
}

Block* Chunk::GetBlock(Vector2 Pos){
  int row = (modBlockLength(Pos.y) - ChunkPos.y)/blockLength;
  int col = (modBlockLength(Pos.x) - ChunkPos.x)/blockLength;
  return BlockList[row*16+col];
}

void Chunk::AddTree(Block* block){
  if(block->getBlockType() == global_enums::OBJECTS::Forest && block->GetBlueNoise() > 0.5){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-8, 8);
    float ShiftX = dist(mt);
    float ShiftY = dist(mt);
    
    Tree * temp = new Tree; //Is this deleted?
    temp->SetPosX(block->getRect().x+ShiftX);
    temp->SetPosY(block->getRect().y+ShiftY);
    ObjList.push_back(temp);
  }
}

void Chunk::DeleteTreeByArrPosition(int i){
  delete ObjList[i];
  ObjList.erase(ObjList.begin()+i);
}

void Chunk::RemoveObjectByPointer(Object* ObjectPtr){
   for(int i = 0; i < ObjList.size(); i++){
     if(ObjList[i] == ObjectPtr){
       ObjList.erase(ObjList.begin()+i); //no delete since object is passed to another list
     }
   }
}

void Chunk::DeleteTreeByMapPosition(Vector2 Pos){
  Vector2 tempPos{Pos.x,Pos.y};
  for(int i = 0; i < ObjList.size(); i++){
    Vector2 ObjPos = ObjList[i]->GetPosition();
    if((ObjPos.x == tempPos.x) && (ObjPos.y == tempPos.y)){
      delete ObjList[i];
      ObjList.erase(ObjList.begin()+i);
    }
  }
}

void Chunk::CheckObjectInteraction(Object* ItemPtr){
  std::vector<int> IdxList;;
  Rectangle ItemHitBox = ItemPtr->GetHitBox();
  for(int i = 0; i < ObjList.size(); i++){
    if(CheckCollisionRecs(ObjList[i]->GetHitBox(), ItemHitBox)){
      //Mark Index of each colliding object (Necessary since AnalzyeActionList changes ObjList)
      IdxList.push_back(i);
    }
  }
  for(int i = IdxList.size() - 1; i >= 0; i--){
    AnalyzeActionList(ObjList[IdxList[i]]->HitByItem(ItemPtr),IdxList[i]);
  }
}

Object* Chunk::CheckObjectPickUp(Object* ItemPtr){
  int Idx;
  Rectangle ItemHitBox = ItemPtr->GetHitBox();
  for(int i = 0; i < ObjList.size(); i++){
    if(CheckCollisionRecs(ObjList[i]->GetHitBox(), ItemHitBox)){
      return ObjList[i];
    }
  }
  return NULL;
}

int Chunk::GetTreeListSize(){return ObjList.size();}

Vector2 Chunk::GetTreeCoor(int i){
  return ObjList[i]->GetPosition();
}

int Chunk::GetChunkLength(){return ChunkLength;}

void Chunk::AnalyzeActionList(std::vector<global_enums::OBJECT_ACTIONS> ActionList, int ObjIdx){
  for(int i = 0; i < ActionList.size(); i++){
    switch(ActionList[i]){
    case global_enums::OBJECT_ACTIONS::DEL:
      DeleteTreeByArrPosition(ObjIdx);
      break;
    case global_enums::OBJECT_ACTIONS::null:
      break;
    case global_enums::OBJECT_ACTIONS::CREATE_LOG:
      CreateObjectAtLocation<Log>(ObjList[ObjIdx]->GetPosition());
      break;
    case global_enums::OBJECT_ACTIONS::CREATE_STICK:
      CreateObjectAtLocation<Stick>(ObjList[ObjIdx]->GetPosition());
      break;
    case global_enums::OBJECT_ACTIONS::CREATE_TREE:
      CreateObjectAtLocation<Tree>(ObjList[i]->GetPosition());
      break;
    default:
      break;
    }
  }
}

template <class T> void Chunk::CreateObjectAtLocation(Vector2 Position){
  T * temp = new T;
  temp->SetPosX(Position.x);
  temp->SetPosY(Position.y);
  ObjList.push_back(temp);
}

Object* Chunk::GetObjectByIndex(int i){ return ObjList[i];}


