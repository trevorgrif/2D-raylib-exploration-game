#include "Chunk.h"
float chunkLength = 20;
float modChunkLength(float x){return (float)((int)x - (int)x % (int)chunkLength);}

void Chunk::setChunkType(ChunkType newType){
  this->chunk_type = newType;
  if(this->chunk_type != freeSpace){
    this->blocked = true;
  }
  else{
    this->blocked = false;
  };
}

void Chunk::draw(){
  switch(this->chunk_type){
  case ChunkType::unitSpace:
    DrawRectangleLinesEx(this->body, 1, PURPLE);
    break;
  case ChunkType::freeSpace:
    DrawRectangleLinesEx(this->body, 1, GREEN);
    break;
  case ChunkType::structSpace:
    DrawRectangleLinesEx(this->body, 1, RED);
    break;
  case ChunkType::borderSpace:
    this->setColor(BLACK);
    DrawRectangleLinesEx(this->body,1, BLACK);
    break;
  case ChunkType::null:
    DrawRectangleLinesEx(this->body, 1, BLACK);
    break;
  };
}
