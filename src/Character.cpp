#include "Character.h"
#include <iostream>

Character::Character(Rectangle Body,  const char* name, Camera2D* camera){
  this->body = Body;
  this->x = Body.x;
  this->y = Body.y;
  this-> name = name;
  this->camera = camera;
}

void Character::setSpeed(float newSpeed){
  this->speed = newSpeed;
}

void Character::updatePos(Camera2D* camera){
  if(isSelected){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      Vector2 mPos = GetMousePosition();
      mPos = GetScreenToWorld2D(mPos,*camera);
      this->x = (int)mPos.x;
      this->y = (int)mPos.y;

      isMoving = true;
    }   
  }
  if(isMoving)
    moveToPoint(this->x,this->y);
}

void Character::updateStatus(Camera2D* camera){
  if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    Vector2 mPos = GetMousePosition();
    mPos = GetScreenToWorld2D(mPos,*camera);
    int mx = (int)mPos.x;
    int my = (int)mPos.y;
    std::cout << mx << " " << my << "\n" << body.x << " " << body.y << "\n";
    if(mx >= body.x && mx <= body.x+body.width && my >= body.y && my <= body.y+body.height){
      this->isSelected = true;
    }
    
  }
  else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
    this->isSelected = false;
  }
}

void Character::moveToPoint(int x, int y){
  if(isMoving){
    if(body.x < x)
      body.x = body.x+speed*GetFrameTime();
    if(body.x > x)
      body.x = body.x-speed*GetFrameTime();
    if(body.y < y)
      body.y = body.y+speed*GetFrameTime();
    if(body.y > y)
      body.y = body.y-speed*GetFrameTime();
    if(x == body.x && y == body.y)
      isMoving = false;
  }
}

void Character::draw(){
  DrawRectanglePro(body, Vector2{0,0},0.0f,BLACK);
}

float Character::getX(){
  return body.x;
}

float Character::getY(){
  return body.y;
}
