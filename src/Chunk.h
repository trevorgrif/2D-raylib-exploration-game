#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include "Block.h"

class Chunk{
 private:
  Vector2 ChunkPos;
  std::vector<Block*> BlockList;
  int ChunkLength = 16;

  static std::map<std::string,Item*>* itemTable;
  
 public:
  Chunk(Vector2 Pos, float NoiseValues[256], std::map<std::string,Item*>* itemTable);
  Chunk(Vector2 Pos,  float NoiseValues[256], float ShiftX[256], float ShiftY[256],  std::map<std::string,Item*>* itemTable);
  ~Chunk();
  
  void DrawChunk();

  int GetChunkLength();
  
  Block* GetBlock(Vector2 Pos);
  Block* GetBlockByIndex(int i);
};

#endif //CHUNK_H
