#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

class Button{
 private:
  Rectangle Rec;
  Rectangle RecOffSet;
  Color PrimaryColor;
  Color AccentColor;
  Vector2 origin;
  float rotation;
  const char * text;

  static Font FontInUse;
  static int _count;
  
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
  Color getPrimaryColor();

  // Customization Methods
  void setPrimaryColor(Color newColor);
  void setAccentColor(Color newColor);

  // Misc Methods
  void Draw();
  bool isMouseHover();
  bool isMouseClick();
};

#endif //BUTTON_H

