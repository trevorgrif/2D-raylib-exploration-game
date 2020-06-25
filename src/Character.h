#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"

class Character{
 private:
  Rectangle body;
  float speed{300};
  int x;
  int y;
  const char * name;
  Camera2D* camera;
  bool CameraFocus{true};
  bool isSelected{false};
  bool isMoving{false};
  
 public:
  Character(Rectangle Body, const char * name, Camera2D* camera);

  void setSpeed(float newSpeed);
  void draw();
  void updatePos(Camera2D* camera);
  void updateStatus(Camera2D* camera);
  void moveToPoint(int x,int y);
  float getX();
  float getY();
  
  
};

#endif //CHARACTER_H
