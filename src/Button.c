#include "Button.h"
//TODO: Make variable text size so font is centered and correct font size
Button::Button(const char * text,float x,float y,float width, float height, Vector2 origin, float rotation){
  Rec = {x,y,width,height};
  this->color = RED; //Default colors are RED and GREEN (choose better options)
  this->hoverColor = GREEN;
  this->rotation = rotation;
  this->origin = origin;
  this->text = text;
}

float Button::getX(){
  return Rec.x;
}

float Button::getY(){
  return Rec.y;
}

float Button::getWidth(){
  return Rec.width;
}

float Button::getHeight(){
  return Rec.height;
}

Vector2 Button::getOrigin(){
  return origin;
}

float Button::getRotation(){
  return rotation;
}

Color Button::getColor(){
  return color;
}

void Button::Draw(){
  DrawRectanglePro(Rec,origin,rotation,color);
  DrawTextRec(GetFontDefault(), text, Rectangle{Rec.x-origin.x,Rec.y-origin.y,Rec.width,Rec.height},40.0f*Rec.width/Rec.height,1.0f,true, BLUE);
}

void Button::setColor(Color newColor){
  color = newColor;
}

void Button::setHoverColor(Color newColor){
  hoverColor = newColor;
}

bool Button::isMouseHover(){
  float mx = GetMouseX();
  float my = GetMouseY();
  if(mx >= Rec.x && mx <= Rec.x+Rec.width && my >= Rec.y && my <= Rec.y+Rec.height){
    setColor(hoverColor);
    return true;
  }
  setColor(RED);
  return false;
}

bool Button::isMouseClick(){
  if(isMouseHover() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    return true;
  }
  return false;
}

void Button::setButtonColor(Color newColor){
  this->color = newColor;
}
