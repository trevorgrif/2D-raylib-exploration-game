#include "Chunk.h"

std::map<std::string, Item*>* Chunk::itemTable = new std::map<std::string,Item*>;

Chunk::Chunk(Vector2 Pos, float NoiseValues[256], std::map<std::string,Item*>* itemTable){
  //Allocated Memory for BlockList
  this->ChunkPos = Pos;
  BlockList.resize(256);
  for(int i = 0; i < 256; i++){
    int col = i % 16;
    int row = (i-(i%16))/16;
    BlockList[i] = new Block({Pos.x + col*blockLength,Pos.y + row*blockLength,blockLength,blockLength},NoiseValues[i], itemTable);
  }
  
}

Chunk::Chunk(Vector2 Pos,  float NoiseValues[256], float ShiftX[256], float ShiftY[256],  std::map<std::string,Item*>* itemTable){
  //Allocated Memory for BlockList
  this->ChunkPos = Pos;
  BlockList.resize(256);
  for(int i = 0; i < 256; i++){
    int col = i % 16;
    int row = (i-(i%16))/16;
    BlockList[i] = new Block({Pos.x + col*blockLength,Pos.y + row*blockLength,blockLength,blockLength},NoiseValues[i],ShiftX[i], ShiftY[i], itemTable);
  }
  
}

Chunk::~Chunk(){
  for(int i = 0; i < 256; i++){
    delete BlockList[i];
  }
  BlockList.clear();
}

void Chunk::DrawChunk(){
  for(int i = 0; i < 256; i++){
    BlockList[i]->drawBlock();
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

int Chunk::GetChunkLength(){return ChunkLength;}


