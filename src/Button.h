#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

class Button{
 private:
  Rectangle Rec;
  Rectangle RecOffSet;
  Color color;
  Color hoverColor;
  Vector2 origin;
  float rotation;
  const char * text;
  Font FontInUse;
  
 public:
  Button(const char * text, float x, float y, float width, float height,Vector2 origin, float rotatio);

  // getProperty Methods
  float getX();
  float getY();
  float getWidth();
  float getHeight();
  Vector2 getOrigin();
  float getRotation();
  Rectangle getRect();
  Color getColor();

  // Customization Methods
  void setColor(Color newColor);
  void setHoverColor(Color newColor);
  void setButtonColor(Color newColor);

  // Misc Methods
  void Draw();
  bool isMouseHover();
  bool isMouseClick();
};

#endif //BUTTON_H

