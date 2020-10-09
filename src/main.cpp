#include "raylib.h" //Inherited from Block and others
#include "Block.h"
#include <iostream> //Mostly for debugging
#include "Button.h"
#include "Character.h"
#include "TileMap.h"
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
void MainMenu(std::vector<Character*>* unitTable, Camera2D* camera, TileMap* map, std::map<std::string,Button>* ButtonTable, std::map<std::string,Item*>* itemTable);
void GameLogic(std::vector<Character*>* unitTable, Camera2D* camera, TileMap* map);
void GameDraw(std::vector<Character*>* unitTable, Camera2D* camera, TileMap* map);
void MakeButtons(std::map<std::string,Button>* ButtonTable);
void PauseDraw(std::vector<Character*>* unitTable,std::map<std::string,Button>* ButtonTable, std::string SaveName, TileMap* map);
//void SaveData(std::vector<Character*>* unitTable,std::string SaveName, TileMap* map);
//void LoadMap(std::vector<Character*>* unitTable,Camera2D* camera, std::string SaveName, TileMap* map, std::map<std::string,Item*>* itemTable);
void LoadItemData(std::map<std::string,Item*>* itemTable);
//void ClearGameData(std::vector<Character*>* unitTable, TileMap* map);
void EndScreen();

// Helper Functions
void CameraUpdate(Camera2D* Camera, std::vector<Character*>* unitTable);
void initMap(std::vector<Character*>* unitTable, Camera2D* camera, TileMap* map, std::map<std::string,Item*>* itemTable);

int main(void){
  std::string testSave = "TestSave.txt"; 
  
  // Initialization: Screen, Camera, List Declarations
  InitWindow(screenWidth, screenHeight, "Simple Game");
  screenWidth = GetScreenWidth();
  screenHeight = GetScreenHeight();
  ToggleFullscreen();
  //SetTargetFPS(100);

  Camera2D* camera = new Camera2D;//TODO: check memory leak
  camera->offset = {screenWidth/2, screenHeight/2 };
  camera->target = {0,0};
  camera->rotation = 0.0f;
  camera->zoom = 2.0f;
 
  std::vector<Character*>* unitTable = new std::vector<Character*>; //Need to delete
  std::map<std::string, Item*>* itemTable = new std::map<std::string, Item*>; //Need to delete
  LoadItemData(itemTable);
  TileMap* map = new TileMap(itemTable);
  
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
	  GameLogic(unitTable, camera, map);
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

void MainMenu(std::vector<Character*>* unitTable, Camera2D* camera, TileMap* map, std::map<std::string,Button>* ButtonTable, std::map<std::string,Item*>* itemTable){
  
  // Logic
  if(ButtonTable->find("M1")->second.isMouseClick()){
    loadMainMenu = false;
    initMap(unitTable,camera,map,itemTable);
  }
  else if(ButtonTable->find("M2")->second.isMouseClick()){
    loadMainMenu = false;
    std::string testSave = "TestSave.txt";
    //LoadMap(unitTable, camera, testSave, map, itemTable);
  }

  // Display Menu items
  ClearBackground(RAYWHITE);
  ButtonTable->find("M1")->second.Draw();
  ButtonTable->find("M2")->second.Draw();
}

void GameLogic(std::vector<Character*>* unitTable, Camera2D* camera, TileMap* map){
  static int count = 0;
  CameraUpdate(camera,unitTable);
  for(int i = 0; i < 1; i++){
    (*unitTable)[i]->updateUnit(camera);
    if(count == 0){
      Vector2 P1 = GetScreenToWorld2D({0,0},*camera);
      Vector2 P2 = GetScreenToWorld2D({0,screenHeight}, *camera);
      Vector2 P3 = GetScreenToWorld2D({screenWidth,0}, *camera);
      Vector2 P4 = GetScreenToWorld2D({screenWidth,screenHeight}, *camera);
      if( (map->GetBlock(P1) == nullptr) || (map->GetBlock(P2) == nullptr) || (map->GetBlock(P3) == nullptr) || (map->GetBlock(P4) == nullptr) ){ //Any of the four corners aren't loading blocks
	map->UpdateChunkList(Vector2{(*unitTable)[i]->getX(),(*unitTable)[i]->getY()});
      }
    }
  }
  count++;
  count = count % 500;
  if(IsKeyPressed(KEY_P)){
    GamePause = true;
  }
}

void GameDraw(std::vector<Character*>* unitTable, Camera2D* camera, TileMap* map){ 
  ClearBackground(RAYWHITE);
  BeginMode2D(*camera);

  Vector2 v = GetScreenToWorld2D(Vector2{0,0},*camera);
  int cxmin = v.x;
  int cymin = v.y;
  v = GetScreenToWorld2D(Vector2{screenWidth,screenHeight},*camera);
  int cxmax = v.x;
  int cymax = v.y;

  //Draw all Units 
  for(int i = 0; i < (int)unitTable->size();i++){
    if((*unitTable)[i]->getX()+(*unitTable)[i]->getWidth() >= cxmin && (*unitTable)[i]->getX() <= cxmax && (*unitTable)[i]->getY()+(*unitTable)[i]->getHeight() >= cymin && (*unitTable)[i]->getY() <= cymax){

      map->DrawChunkList();
      (*unitTable)[i]->draw();
    }
  }
  Vector2 tempo{screenWidth-75,0};
  tempo = GetScreenToWorld2D(tempo,*camera);
  DrawFPS(tempo.x,tempo.y);
  EndMode2D();
}

void PauseDraw(std::vector<Character*>* unitTable,std::map<std::string,Button>* ButtonTable,std::string SaveName, TileMap* map){
  SaveName = "";
  map->ClearMap(); //These three lines do nothing, just remove errors until saving/loading is fixed
  (*unitTable)[0]->getX();

  if(ButtonTable->find("P1")->second.isMouseClick()){
    GamePause = false;
  }
  else if(ButtonTable->find("P3")->second.isMouseClick()){
    //ClearGameData(unitTable,map);
    loadMainMenu = true;
    GamePause = false;
  }
  else if(ButtonTable->find("P2")->second.isMouseClick()){
    //SaveData(unitTable,SaveName,map);
  }
  ButtonTable->find("P1")->second.Draw();
  ButtonTable->find("P2")->second.Draw();
  ButtonTable->find("P3")->second.Draw();
  
}

void EndScreen(){
  //Report Final Score and redirect to Main Menu
}

void CameraUpdate(Camera2D* camera, std::vector<Character*>* unitTable){
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
  camera->target.x = (*unitTable)[0]->getX();
  camera->target.y = (*unitTable)[0]->getY();
}

void initMap(std::vector<Character*>* unitTable, Camera2D* camera, TileMap* map, std::map<std::string,Item*>* itemTable){
  //Create UnitTable
  unitTable->push_back(new Character(Rectangle{(float)0,(float)0,16,16},"Player", camera, map, itemTable));
  (*unitTable)[0]->Inven->SetItemAtSlot(itemTable->find("Wood Sword")->second,0);

  //Setting Save file location
  map->SetWorldName("world10");
  map->CreateWorldSaveDir();
}

void MakeButtons(std::map<std::string,Button>* ButtonTable){
  ButtonTable->insert(std::pair<std::string,Button>{"M1",{"New",(float)(screenWidth/2.0-100) ,(float)(screenHeight*3/8-50.0),200,100,Vector2{0,0},0.0f}});
  ButtonTable->insert({"M2",{"Load",(float)(screenWidth/2.0-100) ,(float)(screenHeight*5/8-50.0),200,100,Vector2{0,0},0.0f}});
  ButtonTable->insert({"P1",{"Resume",(float)(screenWidth/2.0-100) ,(float)(screenHeight*2/8-50.0),250,125,Vector2{0,0},0.0f}});
  ButtonTable->insert({"P2",{"Save",(float)(screenWidth/2.0-100) ,(float)(screenHeight*4/8-50.0),250,125,Vector2{0,0},0.0f}});
  ButtonTable->insert({"P3",{"Quit",(float)(screenWidth/2.0-100) ,(float)(screenHeight*6/8-50.0),250,125,Vector2{0,0},0.0f}});
}

//void SaveData(std::vector<Character*>* unitTable,std::string SaveName, TileMap* map){ //Needs to save/Load inventory
  /*std::string directory = "data/worlds/";
  directory.append(SaveName);
  
  std::ofstream outFile;
  outFile.open(directory, std::ofstream::out | std::ofstream::trunc); //Clears files data for reinputting

  std::cout << "Saving...\n";
  outFile << map->getSeed() << std::endl; // Seed
  outFile << unitTable->find(1)->second->getX() << " " << unitTable->find(1)->second->getY() << "\n"; // Player Pos
  std::cout << unitTable->find(1)->second->Inven->GetSize() << "\n";
  for(int i = 0; i <= 9; i++){// Items
    std::cout << "Saving Item " << i << "\n";
    outFile << unitTable->find(1)->second->Inven->GetItem(i)->Name << std::endl;
  }
  COMMENT THIS OUT for(auto it = map->map->cbegin(); it != map->map->cend(); ++it){
    outFile <<  it->first.x << " " << it->first.y << " " << it->second->GetNoiseValue() << " " << it->second->GetShiftX() << " " << it->second->GetShiftY() << " ";
    }
    outFile.close();*/
//}

//void LoadMap(std::vector<Character*>* unitTable,Camera2D* camera, std::string SaveName, TileMap* map, std::map<std::string,Item*>* itemTable){
  /*// Load Data from file
  float tempSeed;
  Vector2 pos;
  float NoiseValue, ShiftX, ShiftY;
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
  while(inFile >> pos.x >> pos.y >> NoiseValue >> ShiftX >> ShiftY){
    map->map->insert({pos,new Block(Rectangle{pos.x,pos.y,blockLength,blockLength},NoiseValue,ShiftX,ShiftY,itemTable)});
  }
  inFile.close();

  //Create UnitTable
  unitTable->insert({1,new Character({PlayerPos.x,PlayerPos.y,blockLength,blockLength},"Player",camera,map,itemTable,ItemArr)});*/
//}

void LoadItemData(std::map<std::string,Item*>* itemTable){
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

    Item* CurrItem = new Item(ItemName,TexturePath,ItemDura,ItemWeight);  // Needs to be deleted..
    
    //Add items to table
    itemTable->insert({ItemName,CurrItem});
    }
}

//void ClearGameData(std::vector<Character*>* unitTable, TileMap* map){ // Frees memory but maintains allocation for larger objects
  //Clear Old/Unwanted Data
  // map->ClearMap();
  //map->GetRanSeed();
  //for(auto it = unitTable->cbegin(); it != unitTable->cend();){
  // delete it->second;
  // it = unitTable->erase(it);
  //}
  
//}


