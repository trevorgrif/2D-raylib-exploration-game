#include "Character.h"
#include <iostream>
#include <math.h>

int Character::numSelected{0};
int Character::MouseDisplaceX{GetMouseX()};
int Character::MouseDisplaceY{GetMouseY()};

Character::Character(Rectangle Body,  const char* name, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map){
  this->body = Body;
  this->x = Body.x;
  this->y = Body.y;
  this-> name = name;
  this->camera = camera;
  this->map = map;
}

void Character::setSpeed(float newSpeed){
  this->speed = newSpeed;
}

void Character::updatePos(Camera2D* camera){
  if(selected){
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && !mouseMoved()){
      Vector2 mPos = GetMousePosition();
      mPos = GetScreenToWorld2D(mPos,*camera);
      int dirX = this->body.x - mPos.x;
      dirX = (dirX > 0) - (dirX < 0);
      int dirY = this->body.y - mPos.y;
      dirY = (dirY > 0) - (dirY < 0);
      this->x = mPos.x - (dirX*displacement*20);
      this->y = mPos.y - (dirY*displacement*20);
      while(map->find(Vector2{modChunkLength(this->x),modChunkLength(this->y)})->second->isBlocked() == true){
	if(modChunkLength(this->x) != modChunkLength(this->body.x)){ 
	  this->m = (this->y - this->body.y)/(this->x - this->body.x);
	  this->b = -1*this->m*this->body.x +this->body.y;
	  if(this->x > this->body.x)
	    this->x = this->x - 5;
	  else if(this->x < this->body.x)
	    this->x = this->x + 5;
	  this->y = gety(this->x);
	}
	else{
	  if(this->y > this->body.y)
	    this->y = this->y - 20;
	  else
	    this->y = this->y + 20;
	}
      }
      this->x = modChunkLength(this->x);
      this->y = modChunkLength(this->y);
      isMoving = true;
    }   
  }
  if(isMoving)
    moveToPoint(this->x,this->y);
  MouseDisplaceX = GetMouseX();
  MouseDisplaceY = GetMouseY();
  
}

void Character::updateStatus(Camera2D* camera){
  static float x_init{GetMouseX()};
  static float y_init{GetMouseY()};
  static bool regionActive{false};
  Vector2 currV;
  static Rectangle region;

  currV = GetMousePosition();
  if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    x_init = currV.x; //Marking initial click point
    y_init = currV.y; 
  }
  else if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){ //If holding button (not examined on first pass) check for mouse movement
    currV = GetMousePosition();
    if(x_init != currV.x || y_init != currV.y){
      std::cout << "Region Detected\n";
      regionActive = true;
    }
    else if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){ // If clicked (no drag) on a unit then select it
      Vector2 mPos = GetScreenToWorld2D(currV,*camera);
      int mx = (int)mPos.x;
      int my = (int)mPos.y;
      if(mx >= body.x && mx <= body.x+body.width && my >= body.y && my <= body.y+body.height){
	this->select();
      }  
    }
    if(regionActive == true){
      //Deselecting all units
	this->deselect();
      if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
	currV = GetMousePosition();
	switch(Vec2Quad(Vector2{currV.x-x_init,currV.y-y_init})){
	case 1:
	  region = Rectangle{x_init, y_init, currV.x - x_init, currV.y - y_init};
	  DrawRectangleLinesEx(region,5, BLACK);
	  break;
	case 2:
	  region = Rectangle{currV.x, y_init, x_init-currV.x, currV.y-y_init};
	  DrawRectangleLinesEx(region ,5, BLACK);
	  break;
	case 3:
	  region = Rectangle{currV.x, currV.y, x_init-currV.x, y_init-currV.y}; 
	  DrawRectangleLinesEx(region ,5, BLACK);
	  break;
	case 4:
	  region = Rectangle{x_init, currV.y, currV.x-x_init, y_init-currV.y};
	  DrawRectangleLinesEx(region ,5, BLACK);
	  break;
	};
      }
      if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
	std::cout << "Mouse Releases\n";
	Vector2 topLeft =  GetScreenToWorld2D(Vector2{region.x,region.y},*camera);
	Vector2 botRight = GetScreenToWorld2D(Vector2{region.x+region.width,region.y+region.height},*camera);
	  if(this->getX() >= topLeft.x && this->getX() <= botRight.x && this->getY() >= topLeft.y && this->getY() <= botRight.y){
	    this->select();
	  }
	regionActive = false;
      }
    }
  }
  else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){ //If Nothing else check for deselect request
    if(this->selected == true)
      this->deselect();
  }
}

void Character::moveToPoint(float x, float y){
  if(isMoving){
    if(abs(body.x-x) < speed*GetFrameTime()) //if "close enough", set equal and stop moving"
      body.x = x;
    if(abs(body.y-y) < speed*GetFrameTime())
      body.y = y;
    if(body.y == y && body.x == x){
      isMoving = false;
      return;
    }
    if(body.x < x)
      body.x = body.x+speed*GetFrameTime();
    if(body.x > x)
      body.x = body.x-speed*GetFrameTime();
    if(body.y < y)
      body.y = body.y+speed*GetFrameTime();
    if(body.y > y)
      body.y = body.y-speed*GetFrameTime();
  }
}

int Character::Vec2Quad(Vector2 v){
  if(v.x >= 0 && v.y >= 0)
    return 1;
  else if(v.x <= 0 && v.y >= 0)
    return 2;
  else if(v.x <= 0 && v.y <= 0)
    return 3;
  else
    return 4;
}


void Character::draw(){DrawRectanglePro(body, Vector2{0,0},0.0f,BLACK);}
float Character::getX(){return body.x;}
float Character::getY(){return body.y;}
float Character::getWidth(){return body.width;}
float Character::getHeight(){return body.height;}
float Character::gety(float x){return this->m*x + this->b;}
float Character::modChunkLength(float x){return (float)((int)x - (int)x%20);}
void Character::select(){if(this->selected == false){this->selected = true; displacement = numSelected; numSelected++;}}
void Character::deselect(){if(this->selected == true){this->selected = false; displacement = 0; numSelected--;}}
bool Character::isSelected(){return selected;}
bool Character::mouseMoved(){if(MouseDisplaceX == GetMouseX() && MouseDisplaceY == GetMouseY()){return false;}return true;}
