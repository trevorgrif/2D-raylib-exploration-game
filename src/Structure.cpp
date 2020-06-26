#include "Structure.h"

Structure::Structure(Rectangle body){
  this->body = body;
}

Structure::Structure(Rectangle body, Color color){
  this->body = body;
  this->color = color;
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
