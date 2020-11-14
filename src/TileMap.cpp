#include "TileMap.h"
#include <iostream>
#include <stdlib.h>
#include <random>
#include <math.h>

#include <thread>

TileMap::TileMap(){
  TerrainNoise.SetNoiseType(FastNoise::Simplex);
  BlueNoise.SetNoiseType(FastNoise::WhiteNoise);

  ChunkCount = ChunkRegWidth*ChunkRegHeight;

  ChunkListCoor.resize(ChunkCount);
  ChunkList.resize(ChunkCount);

  Vector2 Center{0,0};
  int LowCol, UppCol, LowRow, UppRow, i{0};
  ChunkLength = 16;//ChunkList[0]->GetChunkLength();
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
}

TileMap::~TileMap(){
  for(auto i : ChunkList){
    delete i;
  }
  ChunkList.clear();
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
    TerrainNoise.SetSeed(x);
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
  TerrainNoise.SetSeed(seed);

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

void TileMap::UpdateChunkList(Vector2 Center){
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

void TileMap::DrawChunkListBelow(){
  for(auto it = ChunkList.begin(); it != ChunkList.end(); ++it){
    (*it)->DrawChunk();
  }
  for(auto it = ChunkList.begin(); it != ChunkList.end(); ++it){
    (*it)->DrawChunkObjectsBelow();
  }
}

void TileMap::DrawChunkListAbove(){
  for(auto it = ChunkList.begin(); it != ChunkList.end(); ++it){
    (*it)->DrawChunkObjectsAbove();
  }
}

void TileMap::LoadChunkList(){ //First Checks for ChunkSaveFile and either loads the file or creates one
  for(int i = 0; i < ChunkCount; i++){
    float TerrNoiseArr[256];
    float BlueNoiseArr[256];
    //Determine if ChunkSave exist
    if(SaveFileExist(Vector2{ChunkListCoor[i].x,ChunkListCoor[i].y})){
      //Load the Data into TerrNoiseArr and ShiftX/ShiftY
      std::ifstream inStream;
      inStream.open(ActiveChunkPath);
      //Inputting TerrNoiseValues
      for(int j = 0; j < 256; j++){
    	inStream >> TerrNoiseArr[j];
      }
      for(int j = 0; j < 256; j++){
    	inStream >> BlueNoiseArr[j];
      }
      ChunkList[i] = new Chunk({ChunkListCoor[i].x,ChunkListCoor[i].y}, TerrNoiseArr, BlueNoiseArr);
      while(inStream.peek() != EOF){
	float tx, ty;
        int ObjectType;
	inStream >> tx >> ty >> ObjectType;
	switch(ObjectType){
	case static_cast<std::underlying_type<global_enums::OBJECTS>::type>(global_enums::OBJECTS::Tree):
	  ChunkList[i]->CreateObjectAtLocation<Tree>({tx,ty});
	  break;
	case static_cast<std::underlying_type<global_enums::OBJECTS>::type>(global_enums::OBJECTS::Log):
	  ChunkList[i]->CreateObjectAtLocation<Log>({tx,ty});
	  break;
	  case static_cast<std::underlying_type<global_enums::OBJECTS>::type>(global_enums::OBJECTS::Stick):
	  ChunkList[i]->CreateObjectAtLocation<Stick>({tx,ty});
	  break;
	default:
	  break;
	}
      }
      inStream.close();
    }
    else{
      for(int j = 0; j < 256; j++){
	int col = j % 16;
	int row = (j-(j%16))/16;
	TerrNoiseArr[j] = GetNoise(ChunkListCoor[i].x + col*blockLength,ChunkListCoor[i].y + row*blockLength);
	BlueNoiseArr[j] = BlueNoise.GetNoise(ChunkListCoor[i].x + col*blockLength,ChunkListCoor[i].y + row*blockLength);
      }
      ChunkList[i] =  new Chunk({ChunkListCoor[i].x,ChunkListCoor[i].y}, TerrNoiseArr, BlueNoiseArr);
      ChunkList[i]->GenerateChunkObjects();
    }
  }
}

void TileMap::UnloadChunkList(){ //Updates all ChunkSaveFiles and then deallocates the memory
  for(int i = 0; i < ChunkCount; i++){
    StoreChunkData(Vector2{ChunkListCoor[i].x,ChunkListCoor[i].y});
  }
  for(auto i : ChunkList){
    delete i;
  }
  //ChunkList.clear();
}

void TileMap::StoreChunkData(Vector2 Coor){ // Stores info on ActiveChunkPath
  SetChunkSavePath(Coor);
  std::ofstream outStream;
  outStream.open(ActiveChunkPath, std::ofstream::out | std::ofstream::trunc); //Clears files data for reinputting

  for(int i = 0; i < ChunkCount; i++){
    if(ChunkListCoor[i].x == Coor.x && ChunkListCoor[i].y == Coor.y){
      //Store all block noise values
      for(int j = 0; j < 256; j++){
	Block* tempBlock = ChunkList[i]->GetBlockByIndex(j);
	outStream <<  tempBlock->GetNoiseValue() << " ";
      }
      //Store all block blue noise values
      for(int j = 0; j < 256; j++){
	Block* tempBlock = ChunkList[i]->GetBlockByIndex(j);
	outStream <<  tempBlock->GetBlueNoise() << " ";
      }
      //Store Object Positions and Types
      for(int j = 0; j < ChunkList[i]->GetTreeListSize(); j++){
	Object* tempV = ChunkList[i]->GetObjectByIndex(j);
	outStream << tempV->GetPosition().x << " " << tempV->GetPosition().y << " " << static_cast<std::underlying_type<global_enums::OBJECTS>::type>(tempV->GetType()) << " ";
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

float TileMap::getSeed(){return TerrainNoise.GetSeed();}
float TileMap::GetNoise(int x,int y){return TerrainNoise.GetNoise(x,y);}

Vector2 TileMap::GetChunkCoor(Vector2 Pos){
  float x0,y0;
  Pos.x = Pos.x;
  Pos.y = Pos.y;
  
  if(Pos.x >= 0)
    x0 = (float)((int)Pos.x - (int)Pos.x % (int)(ChunkLength*blockLength));
  else{
    if(((int)Pos.x % (int)(ChunkLength*blockLength)) == 0)
      x0 = (int)Pos.x;
    else
      x0 = (float)((int)Pos.x - ((int)Pos.x % (int)(ChunkLength*blockLength)) - (ChunkLength*blockLength));
  }
  if(Pos.y >= 0)
    y0 = (float)((int)Pos.y - (int)Pos.y % (int)(ChunkLength*blockLength));
  else{
    if(((int)Pos.y % (int)(ChunkLength*blockLength)) == 0)
      y0 = (int)Pos.y;
    else
      y0 = (float)((int)Pos.y - ((int)Pos.y % (int)(ChunkLength*blockLength)) - (ChunkLength*blockLength));
  }
  
  return {x0,y0};
}

void TileMap::CheckItemInteraction(Object* ItemPtr){
  for(int i = 0; i < ChunkCount; i++){
    ChunkList[i]->CheckObjectInteraction(ItemPtr);
  }
}

Object * TileMap::CheckItemPickUp(Object * ItemPtr){
  for(int i = 0; i < ChunkCount; i++){
    Object * RecievedObject = ChunkList[i]->CheckObjectPickUp(ItemPtr);
    if(RecievedObject != NULL)
      return RecievedObject;
  }
  return NULL;
}

void TileMap::RemoveObjectByPointer(Object * ObjectPtr){
  for(int i = 0; i < ChunkCount; i++){
    ChunkList[i]->RemoveObjectByPointer(ObjectPtr);
  }
}
