#include "TileMap.h"
#include <iostream>
#include <stdlib.h>
#include <random>
#include <math.h>

TileMap::TileMap(std::vector<Item*>* itemTable){
  MapNoise.SetNoiseType(FastNoise::Simplex);
  this->itemTable = itemTable;

  ChunkCount = ChunkRegWidth*ChunkRegHeight;

  ChunkListCoor.resize(ChunkCount);
  ChunkList.resize(ChunkCount);

  Vector2 Center{0,0};
  
  ChunkLength = 16;//ChunkList[0]->GetChunkLength();
}

void TileMap::SetWorldName(std::string NewName){
  this->WorldName = NewName;
}

void TileMap::ClearMap(){
  
}

void TileMap::CreateSeed(){
  //Check for existing seed
  std::ifstream ifStream;
  ifStream.open("data/worlds/" + WorldName + "/tilemapinfo.txt");
  if(!ifStream.fail()){
    //Get Seed and store
    float x;
    ifStream >> x;
    std::cout << "Got the seed from file: " << x <<std::endl;
    MapNoise.SetSeed(x);
  }
  else{
    GetRanSeed();
  }
  return;
}

void TileMap::GetRanSeed(){
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(-1000000, 1000000);
  setSeed(dist(mt));
}

void TileMap::setSeed(float seed){
  MapNoise.SetSeed(seed);

  //Store seed value in file
  std::ofstream ifStream;
  ifStream.open("data/worlds/" + WorldName + "/tilemapinfo.txt", std::ofstream::out | std::ofstream::trunc);
  ifStream << getSeed();
  ifStream.close();
  
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



void TileMap::LoadChunkList(){ //First Checks for ChunkSaveFile and either loads the file or creates one
  for(int i = 0; i < ChunkCount; i++){
    float NoiseArr[256];
    //Determine if ChunkSave exist
    if(SaveFileExist(Vector2{ChunkListCoor[i].x,ChunkListCoor[i].y})){
      //Load the Data into NoiseArr and ShiftX/ShiftY
      std::ifstream inStream;
      inStream.open(ActiveChunkPath);
      float sx[256],sy[256];
      for(int j = 0; j < 256; j++){
    	inStream >> sx[j] >> sy[j] >> NoiseArr[j];
      }
      inStream.close();
      ChunkList[i] = new Chunk({ChunkListCoor[i].x,ChunkListCoor[i].y}, NoiseArr, sx, sy, itemTable);
    }
    else{
      for(int j = 0; j < 256; j++){
	int col = j % 16;
	int row = (j-(j%16))/16;
	NoiseArr[j] = GetNoise(ChunkListCoor[i].x + col*blockLength,ChunkListCoor[i].y + row*blockLength);
      }
      ChunkList[i] = new Chunk({ChunkListCoor[i].x,ChunkListCoor[i].y}, NoiseArr, itemTable);
    }
  }
}

void TileMap::UnloadChunkList(){ //Updates all ChunkSaveFiles and then deallocates the memory
  for(int i = 0; i < ChunkCount; i++){
    StoreChunkData(Vector2{ChunkListCoor[i].x,ChunkListCoor[i].y});
  }
    for(int i = 0; i < ChunkCount; i++){
    delete ChunkList[i];
  }
}

void TileMap::StoreChunkData(Vector2 Coor){ // Stores info on ActiveChunkPath
  SetChunkSavePath(Coor);
  std::ofstream outStream;
  outStream.open(ActiveChunkPath, std::ofstream::out | std::ofstream::trunc); //Clears files data for reinputting

  for(int i = 0; i < ChunkCount; i++){
    if(ChunkListCoor[i].x == Coor.x && ChunkListCoor[i].y == Coor.y){
      for(int j = 0; j < 256; j++){
	Block* tempBlock = ChunkList[i]->GetBlockByIndex(j);
	outStream <<  tempBlock->GetShiftX() << " " << tempBlock->GetShiftY() << " " << tempBlock->GetNoiseValue() << " ";
      }
    }
  }
  outStream.close();
}

bool TileMap::SaveFileExist(Vector2 Coor){ //Takes any GameRelative coors
  SetChunkSavePath(Coor);
  std::ifstream inStream;
  inStream.open(ActiveChunkPath);
  if(inStream.fail()){
    inStream.close();
    return false;
  }
  inStream.close();
  return true;
}

void TileMap::SetChunkSavePath(Vector2 Coor){
  Coor = GetChunkCoor(Coor);
  ActiveChunkPath = "data/worlds/";
  ActiveChunkPath.append(WorldName);
  ActiveChunkPath.append("/chunks/");
  ActiveChunkPath.append(std::to_string((int)Coor.x));
  ActiveChunkPath.append(std::to_string((int)Coor.y));
  ActiveChunkPath.append(".txt");
}

void TileMap::CreateWorldSaveDir(){
  std::string TempPath;
  TempPath = "data/worlds/";
  TempPath.append(WorldName);
  std::filesystem::create_directories(TempPath);
  TempPath.append("/chunks");
  std::filesystem::create_directories(TempPath);
}

float TileMap::getSeed(){return MapNoise.GetSeed();}
float TileMap::GetNoise(int x,int y){return MapNoise.GetNoise(x,y);}


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
