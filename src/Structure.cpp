#include "Structure.h"

Structure::Structure(Rectangle body,std::map<Vector2,Chunk*,Vec2Compare>* map,ChunkType type_oh_chunk){
  this->body = body;
  this->map = map;
  
  for(int i = modChunkLength(body.x); i < modChunkLength(body.x+body.width); i = i+chunkLength){
    for(int j = modChunkLength(body.y); j < modChunkLength(body.y + body.height); j = j+chunkLength){
      map->find(Vector2{(float)i,(float)j})->second->setChunkType(type_oh_chunk);
    }
  }
}

Structure::Structure(Rectangle body, std::map<Vector2,Chunk*,Vec2Compare>* map,ChunkType type_oh_chunk, Color color){
  this->body = body;
  this->color = color;
  this->map = map;
  for(int i = modChunkLength(body.x); i < modChunkLength(body.x+body.width); i = i+chunkLength){
    for(int j = modChunkLength(body.y); j < modChunkLength(body.y + body.height); j = j+chunkLength){
      map->find(Vector2{(float)i,(float)j})->second->setChunkType(type_oh_chunk);
    }
  }
}

void Structure::draw(){
  if(this->fill == true){
    DrawRectanglePro(this->body, Vector2{0,0},0.0f,this->color);
  }
  else
    DrawRectangleLinesEx(this->body, 5, this->color); 
}

void Structure::setFill(bool value){this->fill = value;}
void Structure::setColor(Color color){this->color = color;}
Rectangle Structure::getBody(){return this->body;}
int Structure::getHealth(){return this->health;}
float Structure::getX(){return this->body.x;}
float Structure::getY(){return this->body.y;}
float Structure::getWidth(){return this->body.width;}
float Structure::getHeight(){return this->body.height;}
