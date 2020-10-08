//Procedural Map using Perlin Noise
#ifndef TILEMAP_H
#define TILEMAP_H

#include "Chunk.h"
#include "FastNoise.h"
#include "Inventory.h"
#include <map>
#include <vector>

class TileMap{
 private:
  FastNoise MapNoise;
  int ChunkLength;
  int ChunkCount;
  int ChunkRegWidth{5};
  int ChunkRegHeight{3};
  
  std::map<std::string,Item*>* itemTable;

  std::vector<Vector2> ChunkListCoor;
  std::vector<Chunk*> ChunkList;
  
 public:
  TileMap(std::map<std::string,Item*>* itemTable);

  void UpdateChunkList(Vector2 Center);
  void setSeed(float seed);
  void UnloadChunkList();
  void DrawChunkList();
  void LoadChunkList();
  void GetRanSeed();
  void ClearMap();

  float GetNoise(int x,int y);
  float getSeed();

  Vector2 GetChunkCoor(Vector2 Pos); //Returns Upper left Coor of the Chunk

  Block* GetBlock(Vector2 v);
  
};

#endif //TILEMAP_H