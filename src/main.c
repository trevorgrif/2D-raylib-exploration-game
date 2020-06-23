#include "raylib.h"
#include <iostream> //Mostly for debugging
#include <list>
#include "Button.h"
#include "Character.h"

bool loadMainMenu = true;
bool GameOver = false;

const int screenWidth = 800; //try to move back into main() 
const int screenHeight = 450;

int mx0,my0;

// Primary Functions
void MainMenu();
void GameLogic(std::list<Rectangle>& buildList, std::list<Rectangle>& sceneElements, std::list<Character>& unitList, Camera2D& camera);
void GameDraw(std::list<Rectangle>& buildList, std::list<Rectangle>& sceneElements, std::list<Character>& unitList, Camera2D& camera);
void EndScreen();

// Helper Functions
void CameraUpdate(Camera2D& Camera);
void InitializeMap(std::list<Rectangle>& buildList);



int main(void)
{
  // Initialization: Camera, TODO: Make fullscreen, implement camera tracking via mouse, Build scene elements (map), make building class, camera variables
  InitWindow(screenWidth, screenHeight, "Simple Game");
  SetTargetFPS(144); // Set FPS

  Camera2D camera;
  camera.offset = {screenWidth/2, screenHeight/2 };
  camera.target = {0,0};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  std::list<Rectangle> buildList;
  std::list<Rectangle> sceneElements; //Shouldnt be Rect list since other shapes will define the environment
  std::list<Character> unitList;

  // Make a 2 Player's (Testing Purposes)
  Rectangle pBody = {20,20,50,100};
  Character player (pBody,"Player 1",camera);
  unitList.push_back(player);

  Rectangle pBody2 = {80,20,50,100};
  Character player2 (pBody2,"Player 2",camera);
  unitList.push_back(player2);
    
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

void GameLogic(std::list<Rectangle>& buildList, std::list<Rectangle>& sceneElements, std::list<Character>& unitList, Camera2D & camera){
  // Run calculations and update positions (and existence) of objects
  InitializeMap(buildList);
  CameraUpdate(camera);

  //Update all Units Pos and Status (Maybe merge into one function) 
  for(int i = 0; i < (int)unitList.size(); i++){
    auto it = std::next(unitList.begin(),i);
    it->updatePos(camera);
    it->updateStatus(camera);
  }

  //Add Border
  Rectangle Border{0,0,2000,1000};
  sceneElements.push_front(Border);

}

void GameDraw(std::list<Rectangle>& buildList, std::list<Rectangle>& sceneElements, std::list<Character>& unitList, Camera2D& camera){
  // Draw all RELEVANT objects
  
  BeginDrawing();
  ClearBackground(RAYWHITE);
  BeginMode2D(camera);
  
  //Draw Scene elements
  DrawRectangleLinesEx(sceneElements.front(), 5, BLACK); 
  
  //Draw all Units
  for(Character currUnit : unitList){//This is an example of what all Draw functions should look like, need to make Building a class type and Scene element a class type and implement .draw() method
    currUnit.draw();
  }
  //Draw all Buildings
  for(Rectangle currBuild : buildList){
    DrawRectangleRec(currBuild,BLUE);
  }

  
  EndMode2D();
  EndDrawing();
}

void EndScreen(){
  //Report Final Score and redirect to Main Menu
}

void CameraUpdate(Camera2D& camera){

  //Zoom settings
  camera.zoom += ((float)GetMouseWheelMove()*0.05f);
  if (camera.zoom > 3.0f) camera.zoom = 3.0f;
  else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

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

    camera.target.x = camera.target.x+diffX;
    camera.target.y = camera.target.y+diffY;
  }
  
  
}

void InitializeMap(std::list<Rectangle>& buildList){
  // Add bases
  Rectangle b1{50,375,100,250};
  Rectangle b2{1850,375,100,250};

  buildList.push_back(b1);
  buildList.push_back(b2);
}
