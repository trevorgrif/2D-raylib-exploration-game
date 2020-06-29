#ifndef CHUNK_H
#define CHUNK_H

class Vec2Compare{
public:
  bool operator()(Vector2 a, Vector2 b) {
    if( a.y < b.y){
      return a.y < b.y;
    }else
      return a.x < b.x;
  }
};

class Chunk{
 private:
  Rectangle body{0,0,0,0};
  bool blocked{false};
  
 public:
  Chunk(){}; // make template work
  Chunk(Rectangle body){this->body = body;};
  Rectangle getRect(){return this->body;};
  void setBlocked(bool value){this->blocked = value;};
  bool isBlocked(){return blocked;};
  void setX(float x){this->body.x = x;};
  void setY(float y){this->body.y = y;};
};

#endif //CHUNK_H
