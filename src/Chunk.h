#ifndef CHUNK_H
#define CHUNK_H

class Chunk{
 private:
  Rectangle body;
  bool blocked{false};
  
 public:
  Chunk(){}; // make template work
  Chunk(Rectangle body){this->body = body;}
  Rectangle getRect(){return this->body;};
  void setBlocked(bool value){this->blocked = value;};
};

#endif //CHUNK_H