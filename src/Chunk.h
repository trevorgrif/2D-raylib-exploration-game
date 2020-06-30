#ifndef CHUNK_H
#define CHUNK_H
#include <iostream>


class Vec2Compare{
public:
  bool operator()(const Vector2& a,const Vector2& b) const {
    if( a.y < b.y)
      return true;
    else if(a.y > b.y)
      return false;
    else if(a.x < b.x)
      return true;
    else
      return false;
  }
};

class Chunk{
 private:
  Rectangle body = Rectangle{0,0,0,0};
  bool blocked{false};
  Color color{GREEN};
  
 public:
  Chunk(Rectangle body){this->body = body;};
  Rectangle getRect(){return this->body;};
  void setBlocked(bool value){this->blocked = value; if(value == false){this->color = GREEN;}else{this->color = RED;};};
  bool isBlocked(){return blocked;};
  void setX(float x){this->body.x = x;};
  void setY(float y){this->body.y = y;};
  void setColor(Color color){this->color = color;};
  Color getColor(){return this->color;};
};

#endif //CHUNK_H
