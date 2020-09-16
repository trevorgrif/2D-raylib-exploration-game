#include "Structure.h"

Structure::Structure(Rectangle body,std::map<Vector2,Block*,Vec2Compare>* map,BlockType type_oh_block){
  this->body = body;
  this->map = map;
  
  for(int i = modBlockLength(body.x); i < modBlockLength(body.x+body.width); i = i+blockLength){
    for(int j = modBlockLength(body.y); j < modBlockLength(body.y + body.height); j = j+blockLength){
      map->find(Vector2{(float)i,(float)j})->second->setBlockType(type_oh_block);
    }
  }
}

Structure::Structure(Rectangle body, std::map<Vector2,Block*,Vec2Compare>* map,BlockType type_oh_block, Color color){
  this->body = body;
  this->color = color;
  this->map = map;
  for(int i = modBlockLength(body.x); i < modBlockLength(body.x+body.width); i = i+blockLength){
    for(int j = modBlockLength(body.y); j < modBlockLength(body.y + body.height); j = j+blockLength){
      map->find(Vector2{(float)i,(float)j})->second->setBlockType(type_oh_block);
    }
  }
}

void Structure::draw(){
  switch(health == 100){
  case true:
    if(this->fill == true){DrawRectanglePro(this->body, Vector2{0,0},0.0f,this->color);}else{DrawRectangleLinesEx(this->body, 5, this->color);}
    break;
  case false:
    DrawRectanglePro(Rectangle{body.x,body.y-(blockLength/2),body.width*(this->health/100),blockLength/5}, Vector2{0,0},0.0f,GREEN);
    if(this->fill == true){DrawRectanglePro(this->body, Vector2{0,0},0.0f,this->color);}else{DrawRectangleLinesEx(this->body, 5, this->color);}
    break;
  }
}
  

void Structure::setFill(bool value){this->fill = value;}
void Structure::setColor(Color color){this->color = color;}
Rectangle Structure::getBody(){return this->body;}
int Structure::getHealth(){return this->health;}
float Structure::getX(){return this->body.x;}
float Structure::getY(){return this->body.y;}
float Structure::getWidth(){return this->body.width;}
float Structure::getHeight(){return this->body.height;}
