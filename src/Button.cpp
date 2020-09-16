#include "Button.h"
#include <iostream>

//TODO: Make variable text size so font is centered and correct font size

//Constructor
Button::Button(const char * text,float x,float y,float width, float height, Vector2 origin, float rotation){
  Rec = {x,y,width,height};
  RecOffSet = {x,y+5,width,height};
  this->color = RED; // Should Grab Theme colors from file
  this->hoverColor = GREEN;
  this->rotation = rotation;
  this->origin = origin;
  this->text = text;
  this->FontInUse = LoadFontEx("fonts/Manrope-Regular.ttf",80,0,0);
}

// getProperty Methods
float Button::getX(){return Rec.x;}
float Button::getY(){return Rec.y;}
float Button::getWidth(){return Rec.width;}
float Button::getHeight(){return Rec.height;}
Vector2 Button::getOrigin(){return origin;}
float Button::getRotation(){return rotation;}
Rectangle Button::getRect(){return Rec;}
Color Button::getColor(){return color;}

// Cusomization Methods
void Button::setColor(Color newColor){color = newColor;}
void Button::setHoverColor(Color newColor){hoverColor = newColor;}
void Button::setButtonColor(Color newColor){this->color = newColor;}

// Misc Methods
void Button::Draw(){
  Vector2 textDim = MeasureTextEx(FontInUse, text, 40.0f*Rec.width/Rec.height, 0);
  switch(isMouseHover()){
  case true:
    DrawRectangleRounded(RecOffSet,0.15,50,SKYBLUE);
    DrawRectangleRounded(Rec,0.15,50,BLUE);
    DrawTextRec(FontInUse, text, Rectangle{Rec.x+(Rec.width/2)-(float)(textDim.x/2),Rec.y+(Rec.height/2)-(textDim.y/2),Rec.width,Rec.height},40.0f*Rec.width/Rec.height,1.0f,true, SKYBLUE);
    break;
  case false:
    DrawRectangleRounded(RecOffSet,0.15,50,BLUE);
    DrawRectangleRounded(Rec,0.15,50,SKYBLUE);
    DrawTextRec(FontInUse, text, Rectangle{Rec.x+(Rec.width/2)-(float)(textDim.x/2),Rec.y+(Rec.height/2)-(textDim.y/2),Rec.width,Rec.height},40.0f*Rec.width/Rec.height,1.0f,true, BLUE);
    break;
  };
}

bool Button::isMouseHover(){
  float mx = GetMouseX();
  float my = GetMouseY();
  if(mx >= Rec.x && mx <= Rec.x+Rec.width && my >= Rec.y && my <= Rec.y+Rec.height){return true;}
  else{return false;}
}

bool Button::isMouseClick(){
  if(isMouseHover() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){return true;}
  else{return false;}
}


