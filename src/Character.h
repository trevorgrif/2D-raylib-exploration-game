#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include <map>
#include <list>
#include "Chunk.h"

class Character{
 private:
  std::map<Vector2,Chunk*,Vec2Compare>* map;
  std::list<Vector2> path;
  Camera2D* camera;
  Vector2 currV;
  
  const char * name;

  static Vector2 mouseClickPoint;
  static ChunkType MCP_type;
  static int numSelected;
  
  Rectangle body;
  Rectangle region;

  int x_init{GetMouseX()};
  int y_init{GetMouseY()};

  float speed{15*chunkLength}; //Note: unaffected by Monitor FPS since GetFrameTime() is called when moving
  float destinX;
  float destinY;
  float m;
  float b;
  float displacement;
  
  bool CameraFocus{true}; //Unused
  bool selected{false};
  bool isMoving{false};
  bool regionActive{false};
  bool markSet{false};
  
 public:
  Character(Rectangle Body, const char * name, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map);
  Character(){};

  int Vec2Quad(Vector2 v);
  bool isSelected();
  
  float gety(float x);
  float getHeight();
  float getWidth();
  float getX();
  float getY();

  void findPath(Vector2 endPoint); //A* Search Algorithm
  void updateUnit(Camera2D* camera);
  void moveToPoint(float x,float y);
  void setSpeed(float newSpeed);
  void findNearestFreeChunk();
  void analyzeMCP();
  void deselect();
  void select();
  void draw();
};
#endif //CHARACTER_H
