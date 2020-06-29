#include "Structure.h"

Structure::Structure(Rectangle body,std::map<Vector2,Chunk*,Vec2Compare>* map){
  this->body = body;
  this->map = map;
}

Structure::Structure(Rectangle body, std::map<Vector2,Chunk*,Vec2Compare>* map, Color color){
  this->body = body;
  this->color = color;
  this->map = map;
  for(int x = this->body.x; x <= this->body.x + this->body.width; x = x+20){
    for(int y = this->body.y; y <= this->body.y + this->body.height; y = y+20){
      this->map->at(Vector2{x,y})->setBlocked(true);
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
