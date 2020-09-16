#include "raylib.h" //Inherited from Block and others
#include "Block.h"
#include <iostream> //Mostly for debugging
#include "Button.h"
#include "Character.h"
#include "Structure.h"
#include "ProcMap.h"
#include "FastNoise.h"
#include <map>
#include <fstream>
#include <filesystem>

bool loadMainMenu = true;
bool GameOver = false;
bool GamePause = false;

float screenWidth;
float screenHeight;

// Primary Functions
void MainMenu(std::map<int,Character*>* unitTable, Camera2D* camera, ProcMap* map, std::map<std::string,Button>* ButtonTable, std::map<std::string,Item>* itemTable);
void GameLogic(std::map<int,Character*>* unitTable, Camera2D* camera);
void GameDraw(std::map<int,Character*>* unitTable, Camera2D* camera, ProcMap* map);
void MakeButtons(std::map<std::string,Button>* ButtonTable);
void PauseDraw(std::map<int,Character*>* unitTable,std::map<std::string,Button>* ButtonTable, std::string SaveName, ProcMap* map);
void SaveData(std::map<int,Character*>* unitTable,std::string SaveName, ProcMap* map);
void LoadMap(std::map<int,Character*>* unitTable,Camera2D* camera, std::string SaveName, ProcMap* map, std::map<std::string,Item>* itemTable);
void LoadItemData(std::map<std::string,Item>* itemTable);
void EndScreen();

// Helper Functions
void CameraUpdate(Camera2D* Camera, std::map<int,Character*>* unitTable);
void initMap(std::map<int,Character*>* unitTable, Camera2D* camera, ProcMap* map, std::map<std::string,Item>* itemTable);

int main(void){
  std::string testSave = "TestSave.txt"; 
  
  // Initialization: Screen, Camera, List Declarations
  InitWindow(screenWidth, screenHeight, "Simple Game");
  screenWidth = GetScreenWidth();
  screenHeight = GetScreenHeight();
  ToggleFullscreen();
  SetTargetFPS(144);

  Camera2D* camera = new Camera2D;//TODO: check memory leak
  camera->offset = {screenWidth/2, screenHeight/2 };
  camera->target = {0,0};
  camera->rotation = 0.0f;
  camera->zoom = 2.0f;

  ProcMap* map = new ProcMap;

  std::map<int, Character*>* unitTable = new std::map<int,Character*>; //Need to delete
  std::map<std::string, Item>* itemTable = new std::map<std::string, Item>; //Need to delete
  LoadItemData(itemTable);
  
  std::map<std::string,Button>* ButtonTable = new std::map<std::string,Button>; // Need to delete
  MakeButtons(ButtonTable);
  
  // Main game loop
  while (!WindowShouldClose())    // Detect window close button or ESC key
    {
      if(loadMainMenu == true){
	BeginDrawing();
	MainMenu(unitTable,camera, map, ButtonTable, itemTable); //ideally scene elements pulled from a data file accessible in MM, hardcoded for now
	EndDrawing();
      }
      else if(GameOver == false){
	BeginDrawing();
	switch(GamePause){
	case false:
	  GameLogic(unitTable, camera);
	  GameDraw(unitTable, camera, map);
	  break;
	case true:
	  GameDraw(unitTable, camera, map);
	  PauseDraw(unitTable,ButtonTable,testSave,map);
	  break;
	}
	EndDrawing();
      }
      else{
	EndScreen();
      }
    }
  CloseWindow();
  return 0;
}

void MainMenu(std::map<int,Character*>* unitTable, Camera2D* camera, ProcMap* map, std::map<std::string,Button>* ButtonTable, std::map<std::string,Item>* itemTable){
  
  // Logic
  if(ButtonTable->find("M1")->second.isMouseClick()){
    loadMainMenu = false;
    initMap(unitTable,camera,map,itemTable);
  }
  else if(ButtonTable->find("M2")->second.isMouseClick()){
    loadMainMenu = false;
    std::string testSave = "TestSave.txt";
    LoadMap(unitTable, camera, testSave, map, itemTable);
  }

  // Display Menu items
  ClearBackground(RAYWHITE);
  ButtonTable->find("M1")->second.Draw();
  ButtonTable->find("M2")->second.Draw();
}

void GameLogic(std::map<int,Character*>* unitTable, Camera2D* camera){
  CameraUpdate(camera,unitTable); 
  for(auto it = unitTable->cbegin(); it != unitTable->cend(); ++it){
    it->second->updateUnit(camera);
  }
  if(IsKeyPressed(KEY_P)){
    GamePause = true;
  }
}

void GameDraw(std::map<int,Character*>* unitTable, Camera2D* camera, ProcMap* map){ 
  ClearBackground(RAYWHITE);
  BeginMode2D(*camera);

  Vector2 v = GetScreenToWorld2D(Vector2{0,0},*camera);
  int cxmin = v.x;
  int cymin = v.y;
  v = GetScreenToWorld2D(Vector2{screenWidth,screenHeight},*camera);
  int cxmax = v.x;
  int cymax = v.y;

  //Draw all Units
  for(auto it = unitTable->cbegin(); it != unitTable->cend();++it){
    if(it->second->getX()+it->second->getWidth() >= cxmin && it->second->getX() <= cxmax && it->second->getY()+it->second->getHeight() >= cymin && it->second->getY() <= cymax){
      map->DrawCloseChunks(Vector2{it->second->getX(),it->second->getY()}, 4);
      it->second->draw();
    }
  }
  Vector2 tempo{screenWidth-75,0};
  tempo = GetScreenToWorld2D(tempo,*camera);
  DrawFPS(tempo.x,tempo.y);
  EndMode2D();
}

void PauseDraw(std::map<int,Character*>* unitTable,std::map<std::string,Button>* ButtonTable,std::string SaveName, ProcMap* map){
  if(ButtonTable->find("P1")->second.isMouseClick()){
    GamePause = false;
  }
  else if(ButtonTable->find("P3")->second.isMouseClick()){
    loadMainMenu = true;
    GamePause = false;
  }
  else if(ButtonTable->find("P2")->second.isMouseClick()){
    SaveData(unitTable,SaveName,map);
  }
  ButtonTable->find("P1")->second.Draw();
  ButtonTable->find("P2")->second.Draw();
  ButtonTable->find("P3")->second.Draw();
  
}

void EndScreen(){
  //Report Final Score and redirect to Main Menu
}

void CameraUpdate(Camera2D* camera, std::map<int,Character*>* unitTable){
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
    return;
  }
  auto it = unitTable->cbegin();
  camera->target.x = it->second->getX();
  camera->target.y = it->second->getY();
}

void initMap(std::map<int,Character*>* unitTable, Camera2D* camera, ProcMap* map, std::map<std::string,Item>* itemTable){
  //Clear Old/Unwanted Data
  map->ClearMap();
  map->GetRanSeed();
  for(auto it = unitTable->cbegin(); it != unitTable->cend();){
    delete it->second;
    it = unitTable->erase(it);
  }

  //Create UnitTable
  unitTable->insert({1,new Character(Rectangle{(float)0,(float)0,16,16},"Player",camera,map, itemTable)});
  unitTable->find(1)->second->Inven->AddItem(itemTable->find("Wood Sword")->second,0);
}

void MakeButtons(std::map<std::string,Button>* ButtonTable){
  ButtonTable->insert(std::pair<std::string,Button>{"M1",{"New",(float)(screenWidth/2.0-100) ,(float)(screenHeight*3/8-50.0),200,100,Vector2{0,0},0.0f}});
  ButtonTable->insert({"M2",{"Load",(float)(screenWidth/2.0-100) ,(float)(screenHeight*5/8-50.0),200,100,Vector2{0,0},0.0f}});
  ButtonTable->insert({"P1",{"Resume",(float)(screenWidth/2.0-100) ,(float)(screenHeight*2/8-50.0),250,125,Vector2{0,0},0.0f}});
  ButtonTable->insert({"P2",{"Save",(float)(screenWidth/2.0-100) ,(float)(screenHeight*4/8-50.0),250,125,Vector2{0,0},0.0f}});
  ButtonTable->insert({"P3",{"Quit",(float)(screenWidth/2.0-100) ,(float)(screenHeight*6/8-50.0),250,125,Vector2{0,0},0.0f}});
}

void SaveData(std::map<int,Character*>* unitTable,std::string SaveName, ProcMap* map){ //Needs to save/Load inventory
  std::string directory = "data/worlds/";
  directory.append(SaveName);
  
  std::ofstream outFile;
  outFile.open(directory, std::ofstream::out | std::ofstream::trunc); //Clears files data for reinputting

  std::cout << "Saving...\n";
  outFile << map->getSeed() << std::endl; // Seed
  outFile << unitTable->find(1)->second->getX() << " " << unitTable->find(1)->second->getY() << "\n"; // Player Pos
  for(int i = 0; i <= 9; i++){// Items
    outFile << unitTable->find(1)->second->Inven->GetItem(i).Name << std::endl;
  }
  for(auto it = map->map->cbegin(); it != map->map->cend(); ++it){
    outFile <<  it->first.x << " " << it->first.y << " " << it->second->GetNoiseValue() << " ";
  }
  outFile.close();
}

void LoadMap(std::map<int,Character*>* unitTable,Camera2D* camera, std::string SaveName, ProcMap* map, std::map<std::string,Item>* itemTable){

  //Clear Old/Unwanted Data
  map->ClearMap();
  for(auto it = unitTable->cbegin(); it != unitTable->cend();){
    delete it->second;
    it = unitTable->erase(it);
  }

  // Load Data from file
  float tempSeed;
  Vector2 pos;
  float NoiseValue;
  Vector2 PlayerPos;
  std::cout << "Loading Map Data...\n";

  std::string directory = "data/worlds/";
  directory.append(SaveName);
  
  std::ifstream inFile;
  inFile.open(directory);

  inFile >> tempSeed;
  map->setSeed(tempSeed);

  std::string ItemArr[10];
  inFile >> PlayerPos.x >> PlayerPos.y;
  std::string TempItemName;
  getline(inFile,TempItemName); //Finished line from PlayerPos
  for(int i = 0; i <= 9; i++){// Items
    getline(inFile, TempItemName);
    ItemArr[i] = TempItemName;
  }
  while(inFile >> pos.x >> pos.y >> NoiseValue){
    map->map->insert({pos,new Block(Rectangle{pos.x,pos.y,blockLength,blockLength},NoiseValue)});
  }
  inFile.close();

  
  //Create UnitTable
  unitTable->insert({1,new Character({PlayerPos.x,PlayerPos.y,blockLength,blockLength},"Player",camera,map,itemTable,ItemArr)});
}

void LoadItemData(std::map<std::string,Item>* itemTable){
  std::cout << "Loading item data...\n";

  std::string path = "data/items";
  for( const auto & entry : std::filesystem::directory_iterator(path)){
    //Load Texture
    std::string ItemName;
    std::string TexturePath;
    float ItemDura;
    float ItemWeight;

    std::ifstream ifstream;
    ifstream.open(entry.path());
    getline(ifstream, ItemName);
    getline(ifstream, TexturePath);
    ifstream >> ItemDura >> ItemWeight;
    ifstream.close();

    Item CurrItem(ItemName,TexturePath,ItemDura,ItemWeight);
    
    //Add items to table
    itemTable->insert({ItemName,CurrItem});
  }
}


