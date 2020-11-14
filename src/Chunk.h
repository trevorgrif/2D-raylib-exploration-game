#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include "Block.h"
#include "object.h"

class Chunk{
 private:
  Vector2 ChunkPos;
  std::vector<Block*> BlockList;
  std::vector<Object*> ObjList;
  int ChunkLength = 16;

  static int _count;
  
 public:
  Chunk(Vector2 Pos, float TerrNoiseValues[256], float BlueNoiseArr[256]);
  Chunk(Vector2 Pos, float TerrNoiseValues[256], float BlueNoiseArr[256], std::vector<Object*>& ObjList);
  ~Chunk();
  
  void DrawChunk();

  int GetChunkLength();
  int GetTreeListSize();

  Vector2 GetTreeCoor(int i);

  Object* GetObjectByIndex(int i);
  
  void AddTree(Block* block);
  void RemoveObjectByPointer(Object* ObjectPtr);
  void DeleteTreeByArrPosition(int i);
  void DeleteTreeByMapPosition(Vector2 Pos);
  void DrawChunkObjectsAbove();
  void DrawChunkObjectsBelow();
  void GenerateChunkObjects();
  void CheckObjectInteraction(Object* ItemPtr);

  template <class T> void CreateObjectAtLocation(Vector2 Position);

  void AnalyzeActionList(std::vector<global_enums::OBJECT_ACTIONS> ActionList, int ObjIdx);
  
  Block* GetBlock(Vector2 Pos);
  Block* GetBlockByIndex(int i);

  Object* CheckObjectPickUp(Object* ItemPtr);
};



#endif //CHUNK_H
