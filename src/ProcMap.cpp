#include "ProcMap.h"
#include <iostream>
#include <stdlib.h>
#include <random>
#include <math.h>

ProcMap::ProcMap(std::map<std::string,Item*>* itemTable){
  MapNoise.SetNoiseType(FastNoise::Perlin);
  GetRanSeed();
  this->itemTable = itemTable;
}

void ProcMap::ClearMap(){
  for(auto it = map->begin(); it != map->cend();){
    delete it->second;
    it = map->erase(it);
  }
}

float ProcMap::GetNoise(int x,int y){
  return MapNoise.GetNoise(x,y);
}

void ProcMap::GetRanSeed(){
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(-1000000, 1000000);
  MapNoise.SetSeed(dist(mt));
}

float ProcMap::getSeed(){return MapNoise.GetSeed();}
void ProcMap::setSeed(float seed){MapNoise.SetSeed(seed);}

void ProcMap::DrawChunk(Vector2 v){ //Takes any coor of a block in the chunk and draws the whole chunk
  float temp;
  float x1,y1;
  v.x = v.x+(0.5*blockLength);
  v.y = v.y+(0.5*blockLength);
  //Determine the coors of the upper left and bottom right corners
  float x0 = v.x - ((int)v.x % (int)(ChunkLength*blockLength));
  float y0 = v.y - ((int)v.y % (int)(ChunkLength*blockLength));
  if(v.x >= 0 && v.y >= 0){ //Quad IV
    x1 = x0 + (ChunkLength*blockLength);
    y1 = y0 + (ChunkLength*blockLength);
    // Adding info to map
    for(int i =x0; i <= x1; i = i+blockLength){
      for(int j = y0; j <= y1; j = j+blockLength){
	if(!(map->count(Vector2{(float)i,(float)j})))
	  map->insert(std::pair<Vector2, Block*>{Vector2{(float)i,(float)j}, new Block(Rectangle{(float)i,(float)j,blockLength,blockLength},GetNoise(i,j),itemTable)});
      }
    }
    for(int i = x0; i <= x1; i = i+blockLength){
      for(int j = y0; j <= y1; j = j+blockLength){
	map->find(Vector2{(float)i,(float)j})->second->drawBlock();
      }
    }
  }
  else if(v.x >= 0 && v.y <= 0){ //Quad I
    x1 = x0 + (ChunkLength*blockLength);
    y1 = y0 - (ChunkLength*blockLength);
    temp = y0;
    y0 = y1;
    y1 = temp;
    // Adding info to map
    for(int i =x0; i <= x1; i = i+blockLength){
      for(int j = y0; j < y1; j = j+blockLength){
	if(!(map->count(Vector2{(float)i,(float)j})))
	  map->insert(std::pair<Vector2, Block*>{Vector2{(float)i,(float)j}, new Block(Rectangle{(float)i,(float)j,blockLength,blockLength},GetNoise(i,j),itemTable)});
      }
    }
    for(int i = x0; i <= x1; i = i+blockLength){
      for(int j = y0; j < y1; j = j+blockLength){
	map->find(Vector2{(float)i,(float)j})->second->drawBlock();
      }
    }
  }
  else if(v.x <= 0 && v.y >= 0){ //Quad III
    x1 = x0 - (ChunkLength*blockLength);
    y1 = y0 + (ChunkLength*blockLength);
    temp = x0;
    x0 = x1;
    x1 = temp;
    // Adding info to map
    for(int i =x0; i < x1; i = i+blockLength){
      for(int j = y0; j <= y1; j = j+blockLength){
	if(!(map->count(Vector2{(float)i,(float)j})))
	  map->insert(std::pair<Vector2, Block*>{Vector2{(float)i,(float)j}, new Block(Rectangle{(float)i,(float)j,blockLength,blockLength},GetNoise(i,j),itemTable)});
      }
    }
    for(int i = x0; i < x1; i = i+blockLength){
      for(int j = y0; j <= y1; j = j+blockLength){
	map->find(Vector2{(float)i,(float)j})->second->drawBlock();
      }
    }
  }
  else{ //Quad II
    x1 = x0 - (ChunkLength*blockLength);
    y1 = y0 - (ChunkLength*blockLength);
    temp = y0;
    y0 = y1;
    y1 = temp;
    temp = x0;
    x0 = x1;
    x1 = temp;
    // Adding info to map
    for(int i =x0; i < x1; i = i+blockLength){
      for(int j = y0; j < y1; j = j+blockLength){
	if(!(map->count(Vector2{(float)i,(float)j})))
	  map->insert(std::pair<Vector2, Block*>{Vector2{(float)i,(float)j}, new Block(Rectangle{(float)i,(float)j,blockLength,blockLength},GetNoise(i,j),itemTable)});
      }
    }
    for(int i = x0; i < x1; i = i+blockLength){
      for(int j = y0; j < y1; j = j+blockLength){
	map->find(Vector2{(float)i,(float)j})->second->drawBlock();
      }
    }
  }
  
}

void ProcMap::DrawChunkItems(Vector2 v){ //Takes any coor of a block in the chunk and draws the whole chunk
  float temp;
  float x1,y1;
  v.x = v.x+(0.5*blockLength);
  v.y = v.y+(0.5*blockLength);
  //Determine the coors of the upper left and bottom right corners
  float x0 = v.x - ((int)v.x % (int)(ChunkLength*blockLength));
  float y0 = v.y - ((int)v.y % (int)(ChunkLength*blockLength));
  if(v.x >= 0 && v.y >= 0){ //Quad IV
    x1 = x0 + (ChunkLength*blockLength);
    y1 = y0 + (ChunkLength*blockLength);
    // Adding info to map
    for(int i = x0; i <= x1; i = i+blockLength){
      for(int j = y0; j <= y1; j = j+blockLength){
	map->find(Vector2{(float)i,(float)j})->second->drawItem();
      }
    }
  }
  else if(v.x >= 0 && v.y <= 0){ //Quad I
    x1 = x0 + (ChunkLength*blockLength);
    y1 = y0 - (ChunkLength*blockLength);
    temp = y0;
    y0 = y1;
    y1 = temp;

    for(int i = x0; i <= x1; i = i+blockLength){
      for(int j = y0; j < y1; j = j+blockLength){
	map->find(Vector2{(float)i,(float)j})->second->drawItem();
      }
    }
  }
  else if(v.x <= 0 && v.y >= 0){ //Quad III
    x1 = x0 - (ChunkLength*blockLength);
    y1 = y0 + (ChunkLength*blockLength);
    temp = x0;
    x0 = x1;
    x1 = temp;
    for(int i = x0; i < x1; i = i+blockLength){
      for(int j = y0; j <= y1; j = j+blockLength){
	map->find(Vector2{(float)i,(float)j})->second->drawItem();
      }
    }
  }
  else{ //Quad II
    x1 = x0 - (ChunkLength*blockLength);
    y1 = y0 - (ChunkLength*blockLength);
    temp = y0;
    y0 = y1;
    y1 = temp;
    temp = x0;
    x0 = x1;
    x1 = temp;
    
    for(int i = x0; i < x1; i = i+blockLength){
      for(int j = y0; j < y1; j = j+blockLength){
	map->find(Vector2{(float)i,(float)j})->second->drawItem();
      }
    }
  }
}

void ProcMap::DrawCloseChunks(Vector2 v, int radius){ //Draws a region around the character should be adjustable
  float regLen = ChunkLength*blockLength;

  for(int r = 0; r <= radius; r++){
    for(int x = -1*r; x <= r; x++){ //Draws positive y
      int y = r - abs(x);
      DrawChunk(Vector2{ v.x+(x*regLen) , v.y+(y*regLen) });
    }
    for(int x = -1*r; x <= r; x++){ //Draws positive y
      int y = abs(x) - r;
      DrawChunk(Vector2{ v.x+(x*regLen) , v.y+(y*regLen) });
    }
  }
}

void ProcMap::DrawCloseChunksItems(Vector2 v, int radius){ //Draws a region around the character should be adjustable
  float regLen = ChunkLength*blockLength;

  for(int r = 0; r <= radius; r++){
    for(int x = -1*r; x <= r; x++){ //Draws positive y
      int y = r - abs(x);
      DrawChunkItems(Vector2{ v.x+(x*regLen) , v.y+(y*regLen) });
    }
    for(int x = -1*r; x <= r; x++){ //Draws positive y
      int y = abs(x) - r;
      DrawChunkItems(Vector2{ v.x+(x*regLen) , v.y+(y*regLen) });
    }
  }
}

int ProcMap::Index(Vector2 v){
  if(v.x >= 0)
    v.x = v.x*2;
  else
    v.x = -2*v.x-1;
  if(v.y >= 0)
    v.y = v.y*2;
  else
    v.y = -2*v.y-1;
  return ((0.5)*(v.x+v.y-1)*(v.x+v.y-2))+v.x;
}
