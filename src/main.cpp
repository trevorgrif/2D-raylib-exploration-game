#include "raylib.h"
#include <iostream> //Mostly for debugging
#include "Button.h"
#include "Character.h"
#include "Structure.h"
#include "Chunk.h"
#include <map>

bool loadMainMenu = true;
bool GameOver = false;

float screenWidth;
float screenHeight;

//Temporary Hard Coded Map dimenisons, should be extracted form file and dynbamically changed in MM
int mapWidth{2000};
int mapHeight{2000};
int chunkLength{20};

int MrKey{0};
//NOTE: For player movement, intial click should return closest possible position on map, if structure is built along the jouney which blocks the path then unit should recalculate path after it realizes original path has been obstructed (Everytime player moves along the path it should check if chunk is blocked) 
int mx0,my0;

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
  camera->target = {0,0};
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
  // Create Menu Items
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
  // Run calculations and update positions (and existence) of objects
  CameraUpdate(camera);
  //Update all Units Pos and Status (Maybe merge into one function) 
  for(auto it = unitTable->cbegin(); it != unitTable->cend(); ++it){
    it->second->updatePos(camera);
    it->second->updateStatus(camera);
  }
}

void GameDraw(std::map<int,Structure*>* bldgTable, std::map<int,Structure*>* sceneElements, std::map<int,Character*>* unitTable, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map){ 
  BeginDrawing();
  ClearBackground(RAYWHITE);
  BeginMode2D(*camera);

  // Everything should only be drawn if in scene window (or close to it) (Consider turning into a function is have to do multiple times)
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
  //Draw all Units
  for(auto it = unitTable->cbegin(); it != unitTable->cend();++it){
    if(it->second->getX()+it->second->getWidth() >= cxmin && it->second->getX() <= cxmax && it->second->getY()+it->second->getHeight() >= cymin && it->second->getY() <= cymax)
      it->second->draw();
  }
  //Draw all Structures
  for(auto it = bldgTable->cbegin(); it != bldgTable->cend(); ++it){
    if(it->second->getX()+it->second->getWidth() >= cxmin && it->second->getX() <= cxmax && it->second->getY()+it->second->getHeight() >= cymin && it->second->getY() <= cymax)
      it->second->draw();
  }
  //Draw Map (Causes severe lag, dont do it silly)
  for(auto it = map->cbegin(); it != map->cend(); ++it){
    if(it->second->getRect().x+it->second->getRect().width >= cxmin && it->second->getRect().x <= cxmax && it->second->getRect().y+it->second->getRect().height >= cymin && it->second->getRect().y <= cymax)
      DrawRectangleLinesEx(it->second->getRect(), 1, RED);
    }

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

void initMap(std::map<int,Structure*>* bldgTable,std::map<int,Structure*>* sceneElements,std::map<int,Character*>* unitTable, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map){
  //Build Map
  for(int i = 0; i <= mapWidth-chunkLength; i = i+chunkLength){ //This doesn't seem to work
    for(int j = 0; j <= mapHeight-chunkLength; j = j+chunkLength){
      map->insert({Vector2{i,j}, new Chunk(Rectangle{i,j,chunkLength,chunkLength})});
    }
  }
  //map->insert({Vector2{20,40}, new Chunk(Rectangle{20,40,chunkLength,chunkLength})}); //This works, if you uncomment this then map->find() works
  
  std::cout << map->cbegin()->first.x << " " << map->cend()->first.x << std::endl;
  if(map->find(Vector2{20,40}) == map->cend()) //Checking if the declaration actually worked
    std::cout << "It's the end\n";
  map->find(Vector2{20,40})->second->isBlocked();
  std::cout << "We made it out\n";

  //load bldgTable 
  Structure* b1 = new Structure(Rectangle{3*chunkLength,20*chunkLength,5*chunkLength,12*chunkLength},map);
  Structure* b2 = new Structure(Rectangle{92*chunkLength,20*chunkLength,5*chunkLength,12*chunkLength},map);
  
  bldgTable->insert({getInt(),b1});
  bldgTable->insert({getInt(),b2});

  //load sceneElements
  Structure* Border = new Structure(Rectangle{0,0,mapWidth,mapHeight},map); //Border might end up being useless if movement depends on map path finding
  Border->setFill(false);
  Border->setColor(BLACK);
  sceneElements->insert({getInt(),Border});
  
  //load unitTable
  Rectangle pBody = {20,20,20,20};
  Character* player = new Character(pBody,"Player 1",camera,map);
  unitTable->insert({getInt(),player});
  
  Rectangle pBody2 = {80,20,20,20};
  Character* player2 = new Character(pBody2,"Player 2",camera,map);
  unitTable->insert({getInt(),player2});

  Rectangle pBody3 = {140,20,20,20};
  Character* player3 = new Character(pBody3,"Player 3",camera,map);
  unitTable->insert({getInt(),player3});
}



