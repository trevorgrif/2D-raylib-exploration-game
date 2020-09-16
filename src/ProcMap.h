//Procedural Map using Perlin Noise

#ifndef PROCMAP_H
#define PROCMAP_H

#include "Block.h"
#include "FastNoise.h"
#include <map>

class ProcMap{
 private:
  FastNoise MapNoise;
  int ChunkLength = 16;
  
 public:
  ProcMap();
  ProcMap(float NewSeed);
  
  float GetNoise(int x,int y);

  float getSeed();
  
  void setSeed(float seed);
  void GetRanSeed();
  void DrawChunk(Vector2 v);
  void DrawCloseChunks(Vector2 v,int radius);
  void ClearMap();
  
  std::map<Vector2,Block*,Vec2Compare>* map = new std::map<Vector2,Block*,Vec2Compare>; // Map Object
};

#endif //PROCMAP_H
