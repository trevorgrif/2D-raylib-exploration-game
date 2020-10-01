//Procedural Map using Perlin Noise

#ifndef PROCMAP_H
#define PROCMAP_H

#include "Block.h"
#include "FastNoise.h"
#include "Inventory.h"
#include <map>

class ProcMap{
 private:
  FastNoise MapNoise;
  int ChunkLength = 16;
  std::map<std::string,Item*>* itemTable;
  
 public:
  std::map<Vector2,Block*,Vec2Compare>* map = new std::map<Vector2,Block*,Vec2Compare>; // Map Object
  ProcMap(std::map<std::string,Item*>* itemTable);
  ProcMap(float NewSeed, std::map<std::string,Item*>* itemTable);
  
  float GetNoise(int x,int y);
  float getSeed();

  int Index(Vector2 v);
  
  void setSeed(float seed);
  void GetRanSeed();
  void DrawChunk(Vector2 v);
  void DrawCloseChunks(Vector2 v,int radius);
  void DrawChunkItems(Vector2 v);
  void DrawCloseChunksItems(Vector2 v,int radius);
  void ClearMap();
  
  
};

#endif //PROCMAP_H
