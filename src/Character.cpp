#include "Character.h"
#include <iostream>
#include <math.h>

int Character::numSelected{0};
ChunkType Character::MCP_type{null};
Vector2 Character::mouseClickPoint;

Character::Character(Rectangle Body,  const char* name, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map){
  this->body = Body;
  this-> name = name;
  this->camera = camera;
  this->map = map;
  for(int i = modChunkLength(body.x); i < modChunkLength(body.x+body.width); i = i+chunkLength){
    for(int j = modChunkLength(body.y); j < modChunkLength(body.y+body.height); j = j+chunkLength){
      map->find(Vector2{(float)i,(float)j})->second->setChunkType(unitSpace);
    }
  }
}
/*
  if(selected)
      if(currPoint != travelPoint) then moveTowardsPoint
      if(isAttacking and in range yadda yadda) then attack
      travelPoint should be set by updateStatus when the following conditions occur
      Unit is selected, clickEventOccurs
          case 1: clickPoint is empty on map
	  case 2: clickPoint is obstructed by environment variable (find nearest open location)
	  case 3: clickPoint is another friendly unit
	  case 4: clickPoint is an enemy or enemy structure (not to be dealt with yet)
        
*/
void Character::updateUnit(Camera2D* camera){ // Interprets mouse action and updates unit accordingly
  currV = GetMousePosition();
  switch(markSet){
  case false: 
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      x_init = currV.x; //Marking initial click point
      y_init = currV.y;
      markSet = true;
    }
    break;
  case true:
    if(!regionActive){
      if(abs(x_init-currV.x) > 2*chunkLength || abs(y_init- currV.y) > 2*chunkLength){
	regionActive = true;
      }
    }
    else{
      //Deselecting all units
      this->deselect();
      if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
	currV = GetMousePosition();
	switch(Vec2Quad(Vector2{currV.x-x_init,currV.y-y_init})){
	case 1:
	  region = Rectangle{(float)x_init, (float)y_init, currV.x - x_init, currV.y - y_init};
	  DrawRectangleLinesEx(region,(int)(chunkLength/5), BLACK);
	  break;
	case 2:
	  region = Rectangle{currV.x, (float)y_init, x_init-currV.x, currV.y-y_init};
	  DrawRectangleLinesEx(region ,(int)(chunkLength/5), BLACK);
	  break;
	case 3:
	  region = Rectangle{currV.x, currV.y, x_init-currV.x, y_init-currV.y}; 
	  DrawRectangleLinesEx(region ,(int)(chunkLength/5), BLACK);
	  break;
	case 4:
	  region = Rectangle{(float)x_init, currV.y, currV.x-x_init, y_init-currV.y};
	  DrawRectangleLinesEx(region ,(int)(chunkLength/5), BLACK);
	  break;
	};
      }
    }
    break;
  }
  if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
    if(markSet == true && regionActive == true){ //Click and Drag Detection
      Vector2 topLeft =  GetScreenToWorld2D(Vector2{region.x,region.y},*camera);
      Vector2 botRight = GetScreenToWorld2D(Vector2{region.x+region.width,region.y+region.height},*camera);
      if(this->getX() >= topLeft.x && this->getX() <= botRight.x && this->getY() >= topLeft.y && this->getY() <= botRight.y){
	this->select();
      }
    }
    else if(markSet == true && regionActive == false){ // Click Detection
      Vector2 mPos = GetScreenToWorld2D(currV,*camera);
      mouseClickPoint.x = mPos.x;
      mouseClickPoint.y = mPos.y;
      analyzeMCP();
      if(MCP_type == unitSpace){
	Chunk* chunk = map->find(Vector2{modChunkLength(mouseClickPoint.x),modChunkLength(mouseClickPoint.y)})->second;
	if(chunk->getRect().x == modChunkLength(getX()) && chunk->getRect().y == modChunkLength(getY()))
	  this->select();
	MCP_type = null;
      }
      else if(MCP_type == freeSpace || MCP_type == structSpace){ // Find closest actual free space pos.
	if(this->selected == true){
	  findNearestFreeChunk();
	  isMoving = true;
	}
	MCP_type = null;
      }
    }  
    markSet = false;
    regionActive = false;
  }
  if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){ // Check for deselect request
    if(this->selected == true)
      this->deselect();
  }
  if(isMoving) // Instead of isMoving variable, check is variable Path is empty or not, always move while not empty
    moveToPoint(destinX,destinY);
}

void Character::findNearestFreeChunk(){ // Finds nearest open space and sets destinX and destinY to that location
  Vector2 mPos = GetScreenToWorld2D(GetMousePosition(),*camera);
  int dirX = this->body.x - mPos.x;
  int dirY = this->body.y - mPos.y;
  dirX = (dirX > 0) - (dirX < 0);
  dirY = (dirY > 0) - (dirY < 0);
  destinX = mPos.x - (dirX*displacement*chunkLength);
  destinY = mPos.y - (dirY*displacement*chunkLength);
  while(map->find(Vector2{modChunkLength(destinX),modChunkLength(destinY)})->second->isBlocked() == true){
    if(modChunkLength(destinX) != modChunkLength(this->body.x)){ 
      this->m = (destinY - this->body.y)/(destinX - this->body.x);
      this->b = -1*this->m*this->body.x +this->body.y;
      if(destinX > this->body.x)
	destinX = destinX - (int)(chunkLength/4);
      else if(destinX < this->body.x)
	destinX = destinX + (int)(chunkLength/4);
      destinY = gety(destinX);
    }
    else{
      if(destinY > this->body.y)
	destinY = destinY - chunkLength;
      else
	destinY = destinY + chunkLength;
    }
  }
  destinX = modChunkLength(destinX);
  destinY = modChunkLength(destinY);
}

void Character::moveToPoint(float x, float y){ // Incements player.pos towards (x,y) //Instead of moving to (x,y) have it move to next point on variable Path
  map->find(Vector2{modChunkLength(body.x),modChunkLength(body.y)})->second->setChunkType(ChunkType::freeSpace);
  if(abs(body.x-x) <= speed*GetFrameTime()) //if close enough then set equal and stop moving"
    body.x = x;
  if(abs(body.y-y) <= speed*GetFrameTime())
    body.y = y;
  if(body.y == y && body.x == x){
    isMoving = false;
    map->find(Vector2{modChunkLength(body.x),modChunkLength(body.y)})->second->setChunkType(ChunkType::unitSpace);
    return;
  }
  else{ // if far enough then increment
    if(body.x < x)
      body.x = body.x+speed*GetFrameTime();
    if(body.x > x)
      body.x = body.x-speed*GetFrameTime();
    if(body.y < y)
      body.y = body.y+speed*GetFrameTime();
    if(body.y > y)
      body.y = body.y-speed*GetFrameTime();
  }
  map->find(Vector2{modChunkLength(body.x),modChunkLength(body.y)})->second->setChunkType(ChunkType::unitSpace);
}

void Character::findPath(Vector2 endPoint){

}

int Character::Vec2Quad(Vector2 v){ //Returns quadrant BR = 1, BL = 2, TL = 3, TR = 4
  if(v.x >= 0 && v.y >= 0)
    return 1;
  else if(v.x <= 0 && v.y >= 0)
    return 2;
  else if(v.x <= 0 && v.y <= 0)
    return 3;
  else
    return 4;
}

void Character::analyzeMCP(){
  auto it = map->cend();
  it--;
  if(mouseClickPoint.x < 0 || mouseClickPoint.y < 0 || mouseClickPoint.x > it->first.x + chunkLength || mouseClickPoint.y > it->first.y + chunkLength) //End of Map
    return;
  Chunk* chunk = map->find(Vector2{modChunkLength(mouseClickPoint.x),modChunkLength(mouseClickPoint.y)})->second;
  if(chunk->getChunkType() == unitSpace){;
    MCP_type = unitSpace;
  }
  else if(chunk->getChunkType() == freeSpace){
    MCP_type = freeSpace;
  }
  else if(chunk->getChunkType() == structSpace){
    MCP_type = structSpace;
  }
}

bool Character::isSelected(){return selected;}

void Character::select(){if(this->selected == false){this->selected = true; displacement = numSelected; numSelected++;}}
void Character::deselect(){if(this->selected == true){this->selected = false; displacement = 0; numSelected--;}}
void Character::draw(){DrawRectanglePro(body, Vector2{0,0},0.0f,BLACK);}
void Character::setSpeed(float newSpeed){this->speed = newSpeed;}

float Character::gety(float x){return this->m*x + this->b;}
float Character::getHeight(){return body.height;}
float Character::getWidth(){return body.width;}
float Character::getX(){return body.x;}
float Character::getY(){return body.y;}
