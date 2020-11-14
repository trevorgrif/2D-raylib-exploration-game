//Procedural Map using Perlin Noise
#ifndef TILEMAP_H
#define TILEMAP_H

#include "Chunk.h"
#include "FastNoise.h"
#include "Inventory.h"
#include "object.h"
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

class TileMap{
 private:
  FastNoise TerrainNoise;
  FastNoise BlueNoise;
  int ChunkLength;
  int ChunkCount;
  int ChunkRegWidth{5};
  int ChunkRegHeight{3};

  std::string ActiveChunkPath;
  std::string WorldName;

  std::vector<Vector2> ChunkListCoor;
  std::vector<Chunk*> ChunkList;
  
 public:
  TileMap();
  ~TileMap();

  void CheckItemInteraction(Object* ItemPtr);
  void RemoveObjectByPointer(Object * ObjectPtr);
  void UpdateChunkList(Vector2 Center); //Updates chunks visible to player (read: Loaded Chunks)
  void SetChunkSavePath(Vector2 Coor);
  void StoreChunkData(Vector2 Coor);
  void SetWorldName(std::string NewName);
  void CreateWorldSaveDir();
  void CreateSeed();
  void setSeed(float seed);
  void UnloadChunkList();
  void DrawChunkListBelow();
  void DrawChunkListAbove();
  void LoadChunkList();
  void GetRanSeed();
  void ClearMap();

  float GetNoise(int x,int y);
  float getSeed();

  bool SaveFileExist(Vector2 Coor);

  Vector2 GetChunkCoor(Vector2 Pos); //Returns Upper left Coor of the Chunk

  Block* GetBlock(Vector2 v);
  Object* CheckItemPickUp(Object * ItemPtr);
  
};

#endif //TILEMAP_H
