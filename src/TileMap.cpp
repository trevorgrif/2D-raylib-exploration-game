#include "TileMap.h"
#include <iostream>
#include <stdlib.h>
#include <random>
#include <math.h>

TileMap::TileMap(std::map<std::string,Item*>* itemTable){
  MapNoise.SetNoiseType(FastNoise::Perlin);
  MapNoise.SetFractalType(FastNoise::RigidMulti); //Need to understand this better
  MapNoise.SetFractalOctaves(30);
  GetRanSeed();
  this->itemTable = itemTable;

  ChunkCount = ChunkRegWidth*ChunkRegHeight;

  ChunkListCoor.resize(ChunkCount);
  ChunkList.resize(ChunkCount);

  Vector2 Center{0,0};
  
  int LowCol, UppCol, LowRow, UppRow, i = 0;
  LowCol = -1*(ChunkRegWidth/2);
  UppCol = (-1*LowCol) - ((ChunkRegWidth+1)%2);
  LowRow = -1*(ChunkRegHeight/2);
  UppRow = (-1*LowRow) - ((ChunkRegHeight+1)%2);   
  for(int row = LowRow; row <= UppRow ; row++){
    for(int col = LowCol; col <= UppCol; col++){
      ChunkListCoor[i] = {Center.x + (col*ChunkLength*blockLength), Center.y + (row*ChunkLength*blockLength)};
      i++;
    }
  }
  LoadChunkList();
  ChunkLength = ChunkList[0]->GetChunkLength();
 
}

void TileMap::ClearMap(){
  /*for(auto it = map->begin(); it != map->cend();){
    delete it->second;
    it = map->erase(it);
    }*/
}



void TileMap::GetRanSeed(){
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(-1000000, 1000000);
  MapNoise.SetSeed(dist(mt));
}

Block* TileMap::GetBlock(Vector2 v){ //Expecting v to be actual game space coords
  Vector2 v2 = GetChunkCoor(v);
  for(int i = 0; i < ChunkCount; i++){
    if(ChunkListCoor[i].x == v2.x && ChunkListCoor[i].y == v2.y){
      return ChunkList[i]->GetBlock(v);
    }
  }
  return nullptr;
}

void TileMap::UpdateChunkList(Vector2 Center){ //Non-dynamic size rn
  //Get the Coords of the Chunk
  Center = GetChunkCoor(Center);
  //Delete all old chunks (a bit wasteful rn, should only update new chunks)
  UnloadChunkList();
  //Reset List
  int LowCol, UppCol, LowRow, UppRow, i{0};
  LowCol = -1*(ChunkRegWidth/2);
  UppCol = (-1*LowCol) - ((ChunkRegWidth+1)%2);
  LowRow = -1*(ChunkRegHeight/2);
  UppRow = (-1*LowRow) - ((ChunkRegHeight+1)%2);
  for(int row = LowRow; row <= UppRow ; row++){
    for(int col = LowCol; col <= UppCol; col++){
      ChunkListCoor[i] = {Center.x + (col*ChunkLength*blockLength), Center.y + (row*ChunkLength*blockLength)};
      i++;
    }
  }
  //Load New Chunks (i.e. allocated memory)
  LoadChunkList();
}

void TileMap::DrawChunkList(){
  for(auto it = ChunkList.begin(); it != ChunkList.end(); ++it){
    (*it)->DrawChunk();
  }
}



void TileMap::LoadChunkList(){
  for(int i = 0; i < ChunkCount; i++){
    float NoiseArr[256];
    for(int j = 0; j < 256; j++){
      int col = j % 16;
      int row = (j-(j%16))/16;
      NoiseArr[j] = GetNoise(ChunkListCoor[i].x + col*blockLength,ChunkListCoor[i].y + row*blockLength);
    }
    ChunkList[i] = new Chunk({ChunkListCoor[i].x,ChunkListCoor[i].y}, NoiseArr, itemTable);
  }
}

void TileMap::UnloadChunkList(){
  for(int i = 0; i < 9; i++){
    delete ChunkList[i];
  }
}

float TileMap::getSeed(){return MapNoise.GetSeed();}
float TileMap::GetNoise(int x,int y){return MapNoise.GetNoise(x,y);}
void TileMap::setSeed(float seed){MapNoise.SetSeed(seed);}

Vector2 TileMap::GetChunkCoor(Vector2 Pos){
  float x0,y0;
  Pos.x = Pos.x+(0.5*blockLength);
  Pos.y = Pos.y+(0.5*blockLength);
  
  if(Pos.x >= 0)
    x0 = (int)Pos.x - ((int)Pos.x % (int)(ChunkLength*blockLength));
  else
    x0 = (int)Pos.x - ((int)(ChunkLength*blockLength) + (int)Pos.x % (int)(ChunkLength*blockLength));
  if(Pos.y >= 0)
    y0 = (int)Pos.y - ((int)Pos.y % (int)(ChunkLength*blockLength));
  else
    y0 = (int)Pos.y - ((int)(ChunkLength*blockLength) + (int)Pos.y % (int)(ChunkLength*blockLength));
  return {x0,y0};
}
