#include "raylib.h"
#include <iostream> //Mostly for debugging
#include "Button.h"
#include "Character.h"
#include "LinkedList.h"

bool loadMainMenu = true;
bool GameOver = false;
bool MapInit = false;
bool BordInit = false;

const int screenWidth = 800; //try to move back into main() 
const int screenHeight = 450;

int mx0,my0;

// Primary Functions
void MainMenu();
void GameLogic(LinkedList<Rectangle>* buildList, LinkedList<Rectangle>* sceneElements, LinkedList<Character>* unitList, Camera2D* camera);
void GameDraw(LinkedList<Rectangle>* buildList, LinkedList<Rectangle>* sceneElements, LinkedList<Character>* unitList, Camera2D* camera);
void EndScreen();

// Helper Functions
void CameraUpdate(Camera2D* Camera);
void InitializeMap(LinkedList<Rectangle>* buildList);

int main(void)
{
  // Initialization: Camera, TODO: Make fullscreen, implement camera tracking via mouse, Build scene elements (map), make building class, camera variables
  InitWindow(screenWidth, screenHeight, "Simple Game");
  SetTargetFPS(144); // Set FPS

  Camera2D* camera = new Camera2D;
  camera->offset = {screenWidth/2, screenHeight/2 };
  camera->target = {0,0};
  camera->rotation = 0.0f;
  camera->zoom = 1.0f;

  LinkedList<Rectangle>* buildList = new LinkedList<Rectangle>;
  LinkedList<Rectangle>* sceneElements = new LinkedList<Rectangle>; //Shouldnt be Rect list since other shapes will define the environment
  LinkedList<Character>* unitList = new LinkedList<Character>;
  
  // Make 2 Player's (Testing Purposes)
  Rectangle pBody = {20,20,50,100};
  Node<Character>* player = new Node<Character> (Character(pBody,"Player 1",camera), "Player 1");
  unitList->addtoBack(player);
  
  Rectangle pBody2 = {80,20,50,100};
  Node<Character>* player2 = new Node<Character> (Character(pBody2,"Player 2",camera), "Player 2");
  unitList->addtoBack(player2);

  Rectangle pBody3 = {140,20,50,100};
  Node<Character>* player3 = new Node<Character> (Character(pBody3,"Player 3",camera), "Player 3");
  unitList->addtoBack(player3);

  // Main game loop
  while (!WindowShouldClose())    // Detect window close button or ESC key
    {
      if(loadMainMenu == true){
	MainMenu(); //ideally scene elements pulled from a data file accessible in MM, hardcoded for now
      }
      else if(GameOver == false){
	GameLogic(buildList, sceneElements, unitList, camera);
	GameDraw(buildList, sceneElements, unitList, camera);
      }
      else{
	EndScreen();
      }
    }
  CloseWindow();

  return 0;
}

void MainMenu(){
  // Create Menu Items
  Button start ("start",screenWidth/2-100,screenHeight/2-50,200,100,Vector2{0,0},0.0f);
  
  // Logic
  if(start.isMouseClick())
    loadMainMenu = false;

  // Display Menu items
  BeginDrawing();
  ClearBackground(RAYWHITE);
  start.Draw();
  EndDrawing();
}

void GameLogic(LinkedList<Rectangle>* buildList, LinkedList<Rectangle>* sceneElements, LinkedList<Character>* unitList, Camera2D* camera){
  // Run calculations and update positions (and existence) of objects
  if(MapInit == false)
    InitializeMap(buildList);
  CameraUpdate(camera);
  unitList->printNames();
  //Update all Units Pos and Status (Maybe merge into one function) 
  for(int i = 1; i <= unitList->size(); i++){
    unitList->getNodei(i)->data.updatePos(camera);
    unitList->getNodei(i)->data.updateStatus(camera);
  }
  unitList->printNames();

  //Add Border
  if(BordInit == false){
    Node<Rectangle>* Border = new Node<Rectangle> (Rectangle{0,0,2000,1000},"Border");
    sceneElements->addtoFront(Border);
    BordInit = true;
  }

}

void GameDraw(LinkedList<Rectangle>* buildList, LinkedList<Rectangle>* sceneElements, LinkedList<Character>* unitList, Camera2D* camera){ 
  BeginDrawing();
  ClearBackground(RAYWHITE);
  BeginMode2D(*camera);
  
  //Draw Scene elements
  DrawRectangleLinesEx(sceneElements->getNodei(1)->data, 5, BLACK); 
  
  //Draw all Units
  unitList->printNames();
  for(int i = 1; i <= unitList->size();i++){//This is an example of what all Draw functions should look like, need to make Building a class type and Scene element a class type and implement .draw() method
    unitList->getNodei(i)->data.draw();
  }
  //Draw all Buildings
  for(int i = 1; i <= buildList->size(); i++){
    DrawRectangleRec(buildList->getNodei(i)->data,BLUE);
  }

  
  EndMode2D();
  EndDrawing();
}

void EndScreen(){
  //Report Final Score and redirect to Main Menu
}

void CameraUpdate(Camera2D* camera){

  //Zoom settings
  camera->zoom += ((float)GetMouseWheelMove()*0.05f);
  if (camera->zoom > 3.0f) camera->zoom = 3.0f;
  else if (camera->zoom < 0.1f) camera->zoom = 0.1f;

  //Drag Setting, if right click pressed mark initial point, get new mouse position, compute change in x and y
  if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){ 
    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
      mx0 = GetMouseX();
      my0 = GetMouseY();
    }
    int mx = GetMouseX();
    int my = GetMouseY();
    int diffY = (my - my0)/15;
    int diffX = (mx - mx0)/15;

    camera->target.x = camera->target.x+diffX;
    camera->target.y = camera->target.y+diffY;
  }
  
  
}

void InitializeMap(LinkedList<Rectangle>* buildList){
  // Add bases
  
  Node<Rectangle>* b1 = new Node<Rectangle> (Rectangle{50,375,100,250},"b1");
  Node<Rectangle>* b2 = new Node<Rectangle> (Rectangle{1850,375,100,250},"b2");
  
  buildList->addtoBack(b1);
  buildList->addtoBack(b2);

  MapInit = true;
}

