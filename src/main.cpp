#include "raylib.h"
#include <iostream> //Mostly for debugging
#include "Button.h"
#include "Character.h"
#include "Structure.h"
#include "Chunk.h"
#include "LinkedList.h"

bool loadMainMenu = true;
bool GameOver = false;

float screenWidth;
float screenHeight;

//Temporary Hard Coded Map dimenisons, should be extracted form file and dynbamically changed in MM
int mapWidth{2000};
int mapHeight{2000};
int chunkLength{20};

int mx0,my0;

// Primary Functions
void MainMenu(LinkedList<Structure>* buildList, LinkedList<Structure>* sceneElements, LinkedList<Character>* unitList, Camera2D* camera, LinkedList<Chunk>* map);
void GameLogic(LinkedList<Structure>* buildList, LinkedList<Structure>* sceneElements, LinkedList<Character>* unitList, Camera2D* camera, LinkedList<Chunk>* map);
void GameDraw(LinkedList<Structure>* buildList, LinkedList<Structure>* sceneElements, LinkedList<Character>* unitList, Camera2D* camera, LinkedList<Chunk>* map);
void EndScreen();

// Helper Functions
void CameraUpdate(Camera2D* Camera);
void initMap(LinkedList<Structure>* buildList,LinkedList<Structure>* sceneElements,LinkedList<Character>* unitList, Camera2D* camera, LinkedList<Chunk>* map);

int main(void){
  // Initialization: Screen, Camera, List Declarations
  InitWindow(screenWidth, screenHeight, "Simple Game");
  screenWidth = GetScreenWidth();
  screenHeight = GetScreenHeight();
  ToggleFullscreen();
  SetTargetFPS(144); // Set FPS

  Camera2D* camera = new Camera2D;//TODO: check memory leak
  camera->offset = {screenWidth/2, screenHeight/2 };
  camera->target = {0,0};
  camera->rotation = 0.0f;
  camera->zoom = 1.0f;

  LinkedList<Structure>* buildList = new LinkedList<Structure>;
  LinkedList<Structure>* sceneElements = new LinkedList<Structure>;
  LinkedList<Character>* unitList = new LinkedList<Character>;
  LinkedList<Chunk>* map = new LinkedList<Chunk>;
  
  // Main game loop
  while (!WindowShouldClose())    // Detect window close button or ESC key
    {
      if(loadMainMenu == true){
	MainMenu(buildList, sceneElements, unitList,camera, map); //ideally scene elements pulled from a data file accessible in MM, hardcoded for now
      }
      else if(GameOver == false){
	GameLogic(buildList, sceneElements, unitList, camera, map);
	GameDraw(buildList, sceneElements, unitList, camera, map);
      }
      else{
	EndScreen();
      }
    }
  CloseWindow();

  return 0;
}

void MainMenu(LinkedList<Structure>* buildList, LinkedList<Structure>* sceneElements, LinkedList<Character>* unitList, Camera2D* camera, LinkedList<Chunk>* map){
  // Create Menu Items
  Button start ("start",(float)screenWidth/2.0-100,(float)screenHeight/2-50.0,200,100,Vector2{0,0},0.0f);
  
  // Logic
  if(start.isMouseClick()){
    loadMainMenu = false;
    initMap(buildList,sceneElements,unitList,camera, map);
  }

  // Display Menu items
  BeginDrawing();
  ClearBackground(RAYWHITE);
  start.Draw();
  EndDrawing();
}

void GameLogic(LinkedList<Structure>* buildList, LinkedList<Structure>* sceneElements, LinkedList<Character>* unitList, Camera2D* camera, LinkedList<Chunk>* map){
  // Run calculations and update positions (and existence) of objects
  CameraUpdate(camera);
  //Update all Units Pos and Status (Maybe merge into one function) 
  for(int i = 1; i <= unitList->size(); i++){
    unitList->getNodei(i)->data.updatePos(camera);
    unitList->getNodei(i)->data.updateStatus(camera);
  }
}

void GameDraw(LinkedList<Structure>* buildList, LinkedList<Structure>* sceneElements, LinkedList<Character>* unitList, Camera2D* camera, LinkedList<Chunk>* map){ 
  BeginDrawing();
  ClearBackground(RAYWHITE);
  BeginMode2D(*camera);

  // Everything should only be drawn if in scene window (or close to it)
  Vector2 v = GetScreenToWorld2D(Vector2{0,0},*camera);
  int cx = v.x;
  int cy = v.y;
  v = GetScreenToWorld2D(Vector2{screenWidth,screenHeight},*camera);
  int cx1 = v.x;
  int cy1 = v.y;
  
  //Draw Scene elements
  for(int i = 1; i <= sceneElements->size(); i++){
    Structure* temp = &sceneElements->getNodei(i)->data;
    if(temp->getX()+temp->getWidth() >= cx && temp->getX() <= cx1 && temp->getY()+temp->getHeight() >= cy && temp->getY() <= cy1)
      temp->draw();
  } 
  //Draw all Units
  for(int i = 1; i <= unitList->size();i++){
    Character* temp = &unitList->getNodei(i)->data;
    if(temp->getX()+temp->getWidth() >= cx && temp->getX() <= cx1 && temp->getY()+temp->getHeight() >= cy && temp->getY() <= cy1)
      temp->draw();
  }
  //Draw all Structures
  for(int i = 1; i <= buildList->size(); i++){
    Structure* temp = &buildList->getNodei(i)->data;
    if(temp->getX()+temp->getWidth() >= cx && temp->getX() <= cx1 && temp->getY()+temp->getHeight() >= cy && temp->getY() <= cy1)
      temp->draw();
  }
  //Draw Map (Causes severe lag, dont do it silly)
  /*for(int i=1; i <=map->size(); i++){
    Chunk* temp = &map->getNodei(i)->data;
    //if(temp->getRect().x+temp->getRect().width >= cx && temp->getRect().x <= cx + screenWidth && temp->getRect().y+temp->getRect().height >= cy && temp->getRect().y <= cy+screenHeight)
      DrawRectangleLinesEx(temp->getRect(), 1, RED);
      }*/

  EndMode2D();
  EndDrawing();
}

void EndScreen(){
  //Report Final Score and redirect to Main Menu
}

void CameraUpdate(Camera2D* camera){

  //Zoom settings
  camera->zoom += ((float)GetMouseWheelMove()*0.1f);
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

void initMap(LinkedList<Structure>* buildList,LinkedList<Structure>* sceneElements,LinkedList<Character>* unitList, Camera2D* camera, LinkedList<Chunk>* map){
  //Build Map
  for(int i = 0; i <= mapWidth-chunkLength; i = i+chunkLength){
    for(int j = 0; j <= mapHeight-chunkLength; j = j+chunkLength){
      Node<Chunk>* temp = new Node<Chunk> (Rectangle{i,j,chunkLength,chunkLength}, "Remove this dumb var");
      map->addtoFront(temp);
    }
  }

  //load buildList 
  Node<Structure>* b1 = new Node<Structure> (Structure(Rectangle{50,375,100,250}),"b1");
  Node<Structure>* b2 = new Node<Structure> (Structure(Rectangle{1850,375,100,250}),"b2");
  
  buildList->addtoBack(b1);
  buildList->addtoBack(b2);

  //load sceneElements
  Node<Structure>* Border = new Node<Structure> (Structure(Rectangle{0,0,mapWidth,mapHeight}),"Border"); //Border might end up being useless if movement depends on map path finding
  Border->data.setFill(false);
  Border->data.setColor(BLACK);
  sceneElements->addtoFront(Border);
  
  //load unitList
  Rectangle pBody = {20,20,50,100};
  Node<Character>* player = new Node<Character> (Character(pBody,"Player 1",camera), "Player 1");
  unitList->addtoBack(player);
  
  Rectangle pBody2 = {80,20,50,100};
  Node<Character>* player2 = new Node<Character> (Character(pBody2,"Player 2",camera), "Player 2");
  unitList->addtoBack(player2);

  Rectangle pBody3 = {140,20,50,100};
  Node<Character>* player3 = new Node<Character> (Character(pBody3,"Player 3",camera), "Player 3");
  unitList->addtoBack(player3);
}

