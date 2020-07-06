#include "raylib.h"
#include "Chunk.h"
#include <iostream> //Mostly for debugging
#include "Button.h"
#include "Character.h"
#include "Structure.h"
#include <map>

bool loadMainMenu = true;
bool GameOver = false;

float screenWidth;
float screenHeight;

//Temporary Hard Coded Map dimenisons, should be extracted form file and dynamically changed in MM
float mapWidth{2000};
float mapHeight{1000};

int MrKey{0};
//NOTE: For player movement, intial click should return closest possible position on map, if structure is built along the jouney which blocks the path then unit should recalculate path after it realizes original path has been obstructed (Everytime player moves along the path it should check if chunk is blocked) 

// Primary Functions
void MainMenu(std::map<int,Structure*>* bldgTable, std::map<int,Structure*>* sceneElements, std::map<int,Character*>* unitTable, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map);
void GameLogic(std::map<int,Structure*>* bldgTable, std::map<int,Structure*>* sceneElements, std::map<int,Character*>* unitTable, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map);
void GameDraw(std::map<int,Structure*>* bldgTable, std::map<int,Structure*>* sceneElements, std::map<int,Character*>* unitTable, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map);
void EndScreen();

// Helper Functions
void CameraUpdate(Camera2D* Camera);
void initMap(std::map<int,Structure*>* bldgTable,std::map<int,Structure*>* sceneElements,std::map<int,Character*>* unitTable, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map);
int getInt(){MrKey++;return MrKey;}

int main(void){
  // Initialization: Screen, Camera, List Declarations
  InitWindow(screenWidth, screenHeight, "Simple Game");
  screenWidth = GetScreenWidth();
  screenHeight = GetScreenHeight();
  ToggleFullscreen();
  SetTargetFPS(144); // Set FPS

  Camera2D* camera = new Camera2D;//TODO: check memory leak
  camera->offset = {screenWidth/2, screenHeight/2 };
  camera->target = {mapWidth/2,mapHeight/2};
  camera->rotation = 0.0f;
  camera->zoom = 1.0f;

  std::map<int, Structure*>* bldgTable = new std::map<int,Structure*>;
  std::map<int, Structure*>* sceneElements = new std::map<int,Structure*>;
  std::map<int, Character*>* unitTable = new std::map<int,Character*>;
  std::map<Vector2,Chunk*,Vec2Compare>* map = new std::map<Vector2,Chunk*,Vec2Compare>;
  
  // Main game loop
  while (!WindowShouldClose())    // Detect window close button or ESC key
    {
      if(loadMainMenu == true){
	MainMenu(bldgTable, sceneElements, unitTable,camera, map); //ideally scene elements pulled from a data file accessible in MM, hardcoded for now
      }
      else if(GameOver == false){
	GameLogic(bldgTable, sceneElements, unitTable, camera, map);
	GameDraw(bldgTable, sceneElements, unitTable, camera, map);
      }
      else{
	EndScreen();
      }
    }
  CloseWindow();
  return 0;
}

void MainMenu(std::map<int,Structure*>* bldgTable, std::map<int,Structure*>* sceneElements, std::map<int,Character*>* unitTable, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map){
  // Create Menu Items TODO: Make this not look shitty
  Button start ("start",(float)screenWidth/2.0-100,(float)screenHeight/2-50.0,200,100,Vector2{0,0},0.0f);
  
  // Logic
  if(start.isMouseClick()){
    loadMainMenu = false;
    initMap(bldgTable,sceneElements,unitTable,camera, map);
  }

  // Display Menu items
  BeginDrawing();
  ClearBackground(RAYWHITE);
  start.Draw();
  EndDrawing();
}

void GameLogic(std::map<int,Structure*>* bldgTable, std::map<int,Structure*>* sceneElements, std::map<int,Character*>* unitTable, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map){
  CameraUpdate(camera); 
  for(auto it = unitTable->cbegin(); it != unitTable->cend(); ++it){
    it->second->updateUnit(camera);
  }
}

void GameDraw(std::map<int,Structure*>* bldgTable, std::map<int,Structure*>* sceneElements, std::map<int,Character*>* unitTable, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map){ 
  BeginDrawing();
  ClearBackground(RAYWHITE);
  BeginMode2D(*camera);

  Vector2 v = GetScreenToWorld2D(Vector2{0,0},*camera);
  int cxmin = v.x;
  int cymin = v.y;
  v = GetScreenToWorld2D(Vector2{screenWidth,screenHeight},*camera);
  int cxmax = v.x;
  int cymax = v.y;


  //Draw Scene elements
  for(auto it = sceneElements->cbegin(); it != sceneElements->cend(); ++it){
    if(it->second->getX() + it->second->getWidth() >= cxmin && it->second->getX() <= cxmax && it->second->getY()+it->second->getHeight() >= cymin && it->second->getY() <= cymax)
      it->second->draw();
  }
  //Draw all Structures
  for(auto it = bldgTable->cbegin(); it != bldgTable->cend(); ++it){
    if(it->second->getX()+it->second->getWidth() >= cxmin && it->second->getX() <= cxmax && it->second->getY()+it->second->getHeight() >= cymin && it->second->getY() <= cymax)
      it->second->draw();
  }
  //Draw all Units
  for(auto it = unitTable->cbegin(); it != unitTable->cend();++it){
    if(it->second->getX()+it->second->getWidth() >= cxmin && it->second->getX() <= cxmax && it->second->getY()+it->second->getHeight() >= cymin && it->second->getY() <= cymax)
      it->second->draw();
  }
    //Draw Map
  for(auto it = map->cbegin(); it != map->cend(); ++it){
    if(it->second->getRect().x+it->second->getRect().width >= cxmin && it->second->getRect().x <= cxmax && it->second->getRect().y+it->second->getRect().height >= cymin && it->second->getRect().y <= cymax)
      it->second->draw();
  }
  EndMode2D();
  EndDrawing();
}

void EndScreen(){
  //Report Final Score and redirect to Main Menu
}

void CameraUpdate(Camera2D* camera){
  static int mx0;
  static int my0;
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

void initMap(std::map<int,Structure*>* bldgTable,std::map<int,Structure*>* sceneElements,std::map<int,Character*>* unitTable, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map){
  //Build Map
  for(int i = -1*chunkLength; i <= mapWidth; i = i+chunkLength){
    for(int j = -1*chunkLength; j <= mapHeight; j = j+chunkLength){
      map->insert(std::pair<Vector2, Chunk*>{Vector2{(float)i,(float)j}, new Chunk(Rectangle{(float)i,(float)j,chunkLength,chunkLength})});
    }
  }
  
  // Load Scene Elements (Border)
  for(int i = -1*chunkLength; i <= mapWidth; i = i+chunkLength){
    sceneElements->insert({getInt(),new Structure(Rectangle{(float)i,-1*chunkLength,chunkLength,chunkLength},map,ChunkType::borderSpace, BLACK)});
    sceneElements->insert({getInt(),new Structure(Rectangle{(float)i,mapHeight,chunkLength,chunkLength},map,ChunkType::borderSpace, BLACK)});
  }
  for(int i = 0; i <= mapHeight-chunkLength; i = i+chunkLength){
    sceneElements->insert({getInt(),new Structure(Rectangle{-1*chunkLength,(float)i,chunkLength,chunkLength},map,ChunkType::borderSpace, BLACK)});
    sceneElements->insert({getInt(),new Structure(Rectangle{mapWidth,(float)i,chunkLength,chunkLength},map,ChunkType::borderSpace, BLACK)});
  }
    
  //load bldgTable 
  bldgTable->insert({getInt(),new Structure(Rectangle{3*chunkLength,20*chunkLength,5*chunkLength,12*chunkLength},map,ChunkType::structSpace)});
  bldgTable->insert({getInt(),new Structure(Rectangle{92*chunkLength,20*chunkLength,5*chunkLength,12*chunkLength},map,ChunkType::structSpace)});

  //load UnitTable
  for(int i = 0; i < mapWidth-40; i = i+2*chunkLength){
    for(int j= chunkLength; j <= chunkLength;j = j+40){
      unitTable->insert({getInt(),new Character(Rectangle{(float)i,(float)j,20,20},"NULL",camera,map,Vector2{mapWidth,mapHeight})});
    }
  }
  
}


