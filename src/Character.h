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
  float displacement;
  const char * name;
  Camera2D* camera;
  bool CameraFocus{true};
  bool selected{false};
  bool isMoving{false};
  std::map<Vector2,Chunk*,Vec2Compare>* map;
  static int numSelected;
  static int MouseDisplaceX;
  static int MouseDisplaceY;
  
 public:
  Character(Rectangle Body, const char * name, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map);
  Character(){};
  
  void setSpeed(float newSpeed);
  void draw();
  void updatePos(Camera2D* camera);
  void updateStatus(Camera2D* camera);
  void moveToPoint(float x,float y);
  float getX();
  float getY();
  float getWidth();
  float getHeight();
  float gety(float x);
  float modChunkLength(float x);
  void select();
  void deselect();
  bool isSelected();
  bool mouseMoved();
  int Vec2Quad(Vector2 v);
  
  
};



#endif //CHARACTER_H
