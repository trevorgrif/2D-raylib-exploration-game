#include "raylib.h" //Inherited from Block and others
#include "Block.h"
#include <iostream> //Mostly for debugging
#include "Button.h"
#include "Character.h"
#include "TileMap.h"
#include "FastNoise.h"
#include "daytimer.h"
#include <map>
#include <fstream>
#include <filesystem>

#include <math.h>

#include <thread>

//Globals I need to get rid of eventually
bool loadMainMenu = true;
bool GameOver = false;
bool GamePause = false;
bool NewWorldNameGiven = false;
bool LOADINGSAVE = false;

float screenWidth;
float screenHeight;

DayTimer DayTime;

//Menu Functions
void MainMenu(std::map<std::string,Button>* ButtonTable);
void EnterWorldName(Camera2D *camera, std::vector<Character*>*& unitTable, std::string &WorldName);
void LoadWorldSaves(Camera2D *camera, std::vector<Character*>*& unitTable, std::string &WorldName);
void PauseDraw(std::vector<Character*>* unitTable, std::map<std::string,Button>* ButtonTable);
void EndScreen();

//Initialization Functions
void MakeButtons(std::map<std::string,Button>* ButtonTable);
void LoadMap(Camera2D *camera, std::vector<Character*>*& unitTable, std::string WorldName);
void initMap(Camera2D *camera, std::vector<Character*>*& unitTable, std::string WorldName);
void ClearGameData(std::vector<Character*>*& unitTable);

//Game Processing Functions
void GameLogic(std::vector<Character*>* unitTable, Camera2D* camera);
void GameDraw(std::vector<Character*>* unitTable, Camera2D* camera);
void CameraUpdate(Camera2D* Camera, std::vector<Character*>* unitTable);

int main(void){
  // Initialization: Screen, Camera, List Declarations
  InitWindow(screenWidth, screenHeight, "Less-Simple Game");
  screenWidth = GetScreenWidth();
  screenHeight = GetScreenHeight();
  ToggleFullscreen();

  Textures.LoadTextures();

  Camera2D* camera = new Camera2D;//TODO: check memory leak
  camera->offset = {screenWidth/2, screenHeight/2 };
  camera->target = {0,0};
  camera->rotation = 0.0f;
  camera->zoom = 3.0f;

  Block tempCam;
  tempCam.SetCamera(camera);

  std::vector<Character*>* unitTable = new std::vector<Character*>; //Need to delete
  
  std::map<std::string,Button>* ButtonTable = new std::map<std::string,Button>; // Need to delete
  MakeButtons(ButtonTable);
  std::string WorldName;
  
  // Main game loop
  while(!WindowShouldClose())    // Detect window close button or ESC key
    {
      if(loadMainMenu == true){
	BeginDrawing();
	MainMenu(ButtonTable);
	EndDrawing();
      }
      else if(NewWorldNameGiven == false){
	switch(LOADINGSAVE){
	case false:
	  BeginDrawing();
	  EnterWorldName(camera, unitTable, WorldName);
	  EndDrawing();
	  break;
	case true:
	  BeginDrawing();
	  LoadWorldSaves(camera, unitTable, WorldName);
	  EndDrawing();
	  break;
	}
      }
      else if(GameOver == false){
	BeginDrawing();
	switch(GamePause){
	case false:
	  GameLogic(unitTable, camera);
	  GameDraw(unitTable, camera);
	  break;
	case true:
	  GameDraw(unitTable, camera);
	  PauseDraw(unitTable, ButtonTable);
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

void MainMenu(std::map<std::string,Button>* ButtonTable){
  // Logic
  if(ButtonTable->find("M1")->second.isMouseClick()){
    loadMainMenu = false;
  }
  else if(ButtonTable->find("M2")->second.isMouseClick()){
    loadMainMenu = false;
    LOADINGSAVE = true;
  }
  // Display Menu items
  ClearBackground(RAYWHITE);
  ButtonTable->find("M1")->second.Draw();
  ButtonTable->find("M2")->second.Draw();
}

void GameLogic(std::vector<Character*>* unitTable, Camera2D* camera){
  DayTime.IncrementTime();
  
  static int count = 0;
  CameraUpdate(camera,unitTable);
  for(int i = 0; i < 1; i++){
    (*unitTable)[i]->updateUnit(camera);
    if(count == 0){
      Vector2 P1 = GetScreenToWorld2D({0,0},*camera);
      Vector2 P2 = GetScreenToWorld2D({0,screenHeight}, *camera);
      Vector2 P3 = GetScreenToWorld2D({screenWidth,0}, *camera);
      Vector2 P4 = GetScreenToWorld2D({screenWidth,screenHeight}, *camera);
      
      if( ((*unitTable)[i]->GetMap()->GetBlock(P1) == nullptr) || ((*unitTable)[i]->GetMap()->GetBlock(P2) == nullptr) || ((*unitTable)[i]->GetMap()->GetBlock(P3) == nullptr) || ((*unitTable)[i]->GetMap()->GetBlock(P4) == nullptr) ){ //Any of the four corners aren't loading blocks
	(*unitTable)[i]->GetMap()->UpdateChunkList(Vector2{(*unitTable)[i]->getX(),(*unitTable)[i]->getY()});
      }
    }
  }
  count++;
  count = count % 400;
  if(IsKeyPressed(KEY_P)){
    GamePause = true;
  }
}

void GameDraw(std::vector<Character*>* unitTable, Camera2D* camera){ 
  ClearBackground(RAYWHITE);
  BeginMode2D(*camera);

  Vector2 v = GetScreenToWorld2D(Vector2{0,0},*camera);
  int cxmin = v.x;
  int cymin = v.y;
  v = GetScreenToWorld2D(Vector2{screenWidth,screenHeight},*camera);
  int cxmax = v.x;
  int cymax = v.y;
  
  //Draw Player
  (*unitTable)[0]->GetMap()->DrawChunkListBelow();
  (*unitTable)[0]->draw();
  (*unitTable)[0]->GetMap()->DrawChunkListAbove();
  

  //Time of Day
  float value = 100*cos(DayTime.GetTime()*M_PI*2/2399)+100;
  Vector2 LightLevelDimension = GetScreenToWorld2D({GetScreenWidth(),GetScreenHeight()},*camera);
  Vector2 LightLevelPosition = GetScreenToWorld2D({0,0},*camera);
  LightLevelDimension.x = LightLevelDimension.x - LightLevelPosition.x;
  LightLevelDimension.y = LightLevelDimension.y - LightLevelPosition.y;
  DrawRectangleV(LightLevelPosition,LightLevelDimension,CLITERAL(Color){0,0,0,value});
  

  if((*unitTable)[0]->GetInventory()->IsOpen())
    (*unitTable)[0]->GetInventory()->Draw(camera);
  
  Vector2 tempo = GetScreenToWorld2D({5,5},*camera);
  DrawFPS(tempo.x,tempo.y);
  EndMode2D();
}
#define Fe
void PauseDraw(std::vector<Character*>* unitTable, std::map<std::string,Button>* ButtonTable){
  if(ButtonTable->find("P1")->second.isMouseClick()){
    GamePause = false;
  }
  else if(ButtonTable->find("P3")->second.isMouseClick()){
    ClearGameData(unitTable);
    loadMainMenu = true;
    GamePause = false;
    NewWorldNameGiven = false;
    LOADINGSAVE = false;
  }
  ButtonTable->find("P1")->second.Draw();
  ButtonTable->find("P3")->second.Draw(); 
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

void initMap(Camera2D *camera, std::vector<Character*>*& unitTable, std::string WorldName){
  TileMap* map = new TileMap();
  unitTable->push_back(new Character(Rectangle{(float)0,(float)0,16,16},"Player", camera, map));
  
  //Create UnitTable
  (*unitTable)[0]->SetWorldName(WorldName);
  (*unitTable)[0]->CreateWorldSaveDir();
  (*unitTable)[0]->Reset();
  
  //Setting Save file location
  map->SetWorldName(WorldName);
  map->CreateWorldSaveDir();
  map->CreateSeed();
  map->LoadChunkList();
}

// All load needs to do is set the world name so that all data gets grabbed!!
void LoadMap(Camera2D *camera, std::vector<Character*>*& unitTable, std::string WorldName){  
  TileMap* map = new TileMap();
  unitTable->push_back(new Character(Rectangle{(float)0,(float)0,16,16},"Player", camera, map));
  
  //Reset Variables
  (*unitTable)[0]->SetWorldName(WorldName);
  (*unitTable)[0]->Reset();
  (*unitTable)[0]->LoadData();
  
  //Setting Save file location
  map->SetWorldName(WorldName);
  map->CreateSeed();
  map->LoadChunkList();
}

void ClearGameData(std::vector<Character*>*& unitTable){
  delete (*unitTable)[0]->GetMap();
  delete (*unitTable)[0];
  unitTable->clear();
}

void MakeButtons(std::map<std::string,Button>* ButtonTable){ //This is dumb
  ButtonTable->insert(std::pair<std::string,Button>{"M1",{"New",(float)(screenWidth/2.0-100) ,(float)(screenHeight*3/8-50.0),200,100,Vector2{0,0},0.0f}});
  ButtonTable->insert({"M2",{"Load",(float)(screenWidth/2.0-100) ,(float)(screenHeight*5/8-50.0),200,100,Vector2{0,0},0.0f}});
  ButtonTable->insert({"P1",{"Resume",(float)(screenWidth/2.0-100) ,(float)(screenHeight*3/8-50.0),250,125,Vector2{0,0},0.0f}});
  ButtonTable->insert({"P3",{"Quit",(float)(screenWidth/2.0-100) ,(float)(screenHeight*5/8-50.0),250,125,Vector2{0,0},0.0f}});
}
void LoadWorldSaves(Camera2D *camera, std::vector<Character*>*& unitTable, std::string &WorldName){
  ClearBackground(RAYWHITE);
  //Draw Back Button
  Button BackButton("X", 10, 10, 200, 100, Vector2{0,0}, 0.0f);
  BackButton.setPrimaryColor(RED);
  BackButton.setAccentColor(MAROON);
  BackButton.Draw();
  if(BackButton.isMouseClick()){
    loadMainMenu = true;
    NewWorldNameGiven = false;
    LOADINGSAVE = false;
  }
  
  //Get a table of world save files
  std::vector<std::string> WorldFileNames;
  for( const auto & entry : std::filesystem::directory_iterator("data/worlds/")){
    std::string temp = entry.path();
    temp.erase(0,12);
    WorldFileNames.push_back(temp);
  }
  
  //Make a Button for each WorldSaveFile
  std::vector<Button> ButtonList;
  for(int i = 0; i < WorldFileNames.size(); i++){
    Button temp(WorldFileNames[i].c_str(),(float)(screenWidth/2.0-100) ,(float)i*110 + 100,200,100,Vector2{0,0},0.0f); // Button Class not drawing text if button is rescaled at all?
    ButtonList.push_back(temp);
  }
  for(int i = 0; i < ButtonList.size(); i++){
    ButtonList[i].Draw();
  }
  for(int i = 0; i < ButtonList.size(); i++){
    if(ButtonList[i].isMouseClick()){
      LoadMap(camera, unitTable, WorldFileNames[i]);
      NewWorldNameGiven = true;
    }
  }
}

void EnterWorldName(Camera2D *camera, std::vector<Character*>*& unitTable, std::string &WorldName){
  ClearBackground(RAYWHITE);

  //Draw Back Button
  Button BackButton("X", 10, 10, 200, 100, Vector2{0,0}, 0.0f);
  BackButton.setPrimaryColor(RED);
  BackButton.setAccentColor(MAROON);
  BackButton.Draw();
  if(BackButton.isMouseClick()){
    WorldName = "";
    loadMainMenu = true;
    NewWorldNameGiven = false;
    LOADINGSAVE = false;
  }
  
  //Getting Key Input 
  int key = GetKeyPressed(); //TODO: Re-Implement the max key, make sure . can't be pressed
  // Check if more characters have been pressed on the same frame
  while (key > 0){
    if ((key >= 32) && (key <= 125)){
      WorldName.push_back((char)key);
    }
    key = GetKeyPressed();  // Check next character in the queue
  }
  if (IsKeyPressed(KEY_BACKSPACE)){
    WorldName.pop_back();
  }

  //Draw Text on Screen
  Rectangle TextBox{screenWidth/2 - 200, screenHeight/2 - 50, 400,100};
  Rectangle Text{TextBox.x+10,TextBox.y+30,380,40};
  Rectangle Instruct1{TextBox.x, TextBox.y - 30, 400,30};
  DrawTextRec(GetFontDefault(), "Enter World Name:", Instruct1, 30.0f,1.0f, true, BLACK);
  Rectangle Instruct2{TextBox.x, TextBox.y + 100, 400,30};
  DrawTextRec(GetFontDefault(), "then press ENTER", Instruct2, 30.0f,1.0f, true, BLACK);
  DrawRectangleLinesEx(TextBox, 3,BLACK);
  DrawTextRec(GetFontDefault(), WorldName.c_str(), Text, 40.0f,1.0f,true, BLACK);
  
  if(IsKeyPressed(KEY_ENTER)){
    NewWorldNameGiven = true;
    initMap(camera, unitTable, WorldName);
    WorldName = "";
  }

}

void EndScreen(){
  //Report Final Score and redirect to Main Menu
}
