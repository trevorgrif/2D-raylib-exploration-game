#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

class Button{
 private:
  Rectangle Rec;
  Color color;
  Color hoverColor;
  Vector2 origin;
  float rotation;
  const char * text;
  
 public:
  Button(const char * text, float x, float y, float width, float height,Vector2 origin, float rotatio);
  float getX();
  float getY();
  float getWidth();
  float getHeight();
  Vector2 getOrigin();
  float getRotation();
  float getRec();
  Color getColor();
  void Draw();
  void setColor(Color newColor);
  void setHoverColor(Color newColor);
  void setButtonColor(Color newColor);
  bool isMouseHover();
  bool isMouseClick();
};

#endif //BUTTON_H
