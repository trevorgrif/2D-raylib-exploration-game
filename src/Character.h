#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include <map>
#include "Chunk.h"

class Character{
 private:
  Rectangle body;
  float speed{300}; //Note: unaffected by Monitor FPS since GetFrameTime() is called when moving
  float x;
  float y;
  float m;
  float b;
  const char * name;
  Camera2D* camera;
  bool CameraFocus{true};
  bool isSelected{false};
  bool isMoving{false};
  std::map<Vector2,Chunk*,Vec2Compare>* map;
  
 public:
  Character(Rectangle Body, const char * name, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map);
  Character(){};

  void setSpeed(float newSpeed);
  void draw();
  void updatePos(Camera2D* camera);
  void updateStatus(Camera2D* camera);
  void moveToPoint(int x,int y);
  float getX();
  float getY();
  float getWidth();
  float getHeight();
  float gety(float x);
  
  
};

#endif //CHARACTER_H
