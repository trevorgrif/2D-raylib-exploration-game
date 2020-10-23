#include "Character.h"
#include <iostream>
#include <math.h>

int Character::numSelected{0};
BlockType Character::MCP_type{Undefined};
Vector2 Character::mouseClickPoint;

Character::Character(Rectangle Body,  const char* name, Camera2D* camera, TileMap* map, std::vector<Item*>* itemTable){
  Inven = new Inventory;
  this->body = Body;
  this->name = name;
  this->camera = camera;
  this->map = map;
  this->startPos.x = this->body.x;
  this->startPos.y = this->body.y;
  this->select();
  this->AvatarSkin = LoadTexture("textures/player/avatar.png"); //Careful of this if ever loading multiple characters
  this->itemTable = itemTable;
  
 
  for(int i = 0; i <= 9; i++)
    this->Inven->CreateSlot();
  
}

Character::Character(Rectangle Body,  const char* name, Camera2D* camera, TileMap* map, std::vector<Item*>* itemTable, std::string NameArr[10]){
  Inven = new Inventory;
  this->body = Body;
  this->name = name;
  this->camera = camera;
  this->map = map;
  this->startPos.x = this->body.x;
  this->startPos.y = this->body.y;
  this->select();
  this->AvatarSkin = LoadTexture("textures/player/avatar.png"); //Careful of this if ever loading multiple characters
  this->itemTable = itemTable;
  for(int i = 0; i <= 9; i++){
    Inven->CreateSlot();
    Inven->SetItemAtSlot((*itemTable)[2],i);
  }
}

Character::~Character(){
  delete Inven;
}

void Character::updateUnit(Camera2D* camera){ // Interprets mouse action and updates unit accordingly
  Vector2 destin;
  Vector2 MCP;
  
  currV = GetMousePosition();
  switch(markSet){
  case false: 
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      x_init = currV.x; //Marking initial click point
      y_init = currV.y;
      markSet = true;
    }
    break;
  case true:
    if(!regionActive){
      if(abs(x_init-currV.x) > 2*blockLength || abs(y_init- currV.y) > 2*blockLength){
	regionActive = true;
      }
    }
    else{
      //Deselecting all units
      this->deselect();
      if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
	currV = GetMousePosition();
	switch(Vec2Quad(Vector2{currV.x-x_init,currV.y-y_init})){
	case 1:
	  region = Rectangle{(float)x_init, (float)y_init, currV.x - x_init, currV.y - y_init};
	  DrawRectangleLinesEx(region,(int)(blockLength/5), BLACK);
	  break;
	case 2:
	  region = Rectangle{currV.x, (float)y_init, x_init-currV.x, currV.y-y_init};
	  DrawRectangleLinesEx(region ,(int)(blockLength/5), BLACK);
	  break;
	case 3:
	  region = Rectangle{currV.x, currV.y, x_init-currV.x, y_init-currV.y}; 
	  DrawRectangleLinesEx(region ,(int)(blockLength/5), BLACK);
	  break;
	case 4:
	  region = Rectangle{(float)x_init, currV.y, currV.x-x_init, y_init-currV.y};
	  DrawRectangleLinesEx(region ,(int)(blockLength/5), BLACK);
	  break;
	};
      }
    }
    break;
  }
  if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
    if(markSet == true && regionActive == true){ //Click and Drag Detection
      Vector2 topLeft =  GetScreenToWorld2D(Vector2{region.x,region.y},*camera);
      Vector2 botRight = GetScreenToWorld2D(Vector2{region.x+region.width,region.y+region.height},*camera);
      if(this->getX() >= topLeft.x && this->getX() <= botRight.x && this->getY() >= topLeft.y && this->getY() <= botRight.y){
	this->select();
      }
    }
    else if(markSet == true && regionActive == false){ // Click Detection 
      Vector2 mPos = GetScreenToWorld2D(currV,*camera);
      MCP.x = modBlockLength(mPos.x);
      MCP.y = modBlockLength(mPos.y);
      analyzeMCP(MCP);
   
      destin.x = modBlockLength(MCP.x); // Setting the destination vector
      destin.y = modBlockLength(MCP.y);

      findNearestFreeBlock(); // Path finding (Just clears path rn
      findPath(destin);
	
      MCP_type = Undefined;
    }  
    markSet = false;
    regionActive = false;
  }
  /*if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){ // Check for deselect request 
    if(this->selected == true)
      //this->deselect();
      }*/
  if(path.size())
    moveAlongPath(destin);
  KeyPressAnalysis();
}

void Character::findNearestFreeBlock(){ // Finds nearest open space and sets destin.x and destin.y to that location
  path.clear();
}

void Character::moveAlongPath(Vector2 &destin){ // Increments player.pos towards next point on path
  Vector2 nextPoint = path.front();
  float x = nextPoint.x;
  float y = nextPoint.y;

  if(body.x - x < 0){
    Direction = 0;
  }
  else if(body.x - x > 0){
    Direction = 1;
  }
 
  //if(abs(body.x-x) <= speed*GetFrameTime() && abs(body.y-y) <= speed*GetFrameTime()) // If about to arrive
  //map->map->find(Vector2{startPos.x,startPos.y})->second->setBlockType(BlockType::freeSpace); // Free old block
  if(abs(body.x-x) <= speed*GetFrameTime()) //if close enough then set equal and stop moving"
    body.x = x;
  if(abs(body.y-y) <= speed*GetFrameTime())
    body.y = y;
  if(body.y == y && body.x == x){ // Arriving on a block
    SaveData();
    startPos = Vector2{path.front().x,path.front().y};
    path.pop_front();
    if(path.size()){ // Just arrived and still moving check if next block space is blocked. If blocked reroute, if not claim next space
      if(map->GetBlock(Vector2{path.front().x,path.front().y})->isBlocked() == true){// If path is obstructed along the journey then find new path
	std::cout << this->name << " path is obstructed at " << path.front().x << " " << path.front().y << std::endl;
	findNearestFreeBlock();
	findPath(destin);
      }
      /*if(map->map->find(Vector2{path.front().x,path.front().y})->second->isBlocked() == false && path.size()){
	map->map->find(Vector2{path.front().x,path.front().y})->second->setBlockType(BlockType::unitSpace);
	}*/
    }
    return; 
  }
  else{ // if far enough then increment
    if(body.x < x)
      body.x = body.x+speed*GetFrameTime();
    if(body.x > x)
      body.x = body.x-speed*GetFrameTime();
    if(body.y < y)
      body.y = body.y+speed*GetFrameTime();
    if(body.y > y)
      body.y = body.y-speed*GetFrameTime();
  }
}

void Character::findPath(Vector2 destin){ //A* Search path finding
  int dispX, dispY;

  //May be useful later but undefined right now
  /*if(map->map->find(Vector2{destin.x,destin.y})->second->getBlockType() == BlockType::structSpace){ 
    std::cout << "It's a structure\n";
    return;
    }*/

  //Determine the Destination Point on the Relative Grid
  destin.x = destin.x/blockLength;
  destin.y = destin.y/blockLength;

  
  Vector2 src = Vector2{modBlockLength(body.x)/blockLength,modBlockLength(body.y)/blockLength};
  if(isDestination(src.y,src.x,destin)){
    std::cout << "Already at Destination\n";
    return;
  }
    
  int SearchRegWidth = abs(destin.x - src.x) + RelativeGridDim;
  int SearchRegHeight = abs(destin.y - src.y) + RelativeGridDim;
  
  std::vector<std::vector<bool>> closedList(SearchRegHeight, std::vector<bool> (SearchRegWidth,false));
  std::vector<std::vector<cell>> cellDetails(SearchRegHeight, std::vector<cell> (SearchRegWidth));
  int i,j;

  for(i = 0; i < SearchRegHeight; i++){
    for(j = 0; j < SearchRegWidth; j++){
      cellDetails[i][j].f = FLT_MAX;
      cellDetails[i][j].g = FLT_MAX;
      cellDetails[i][j].h = FLT_MAX;
      cellDetails[i][j].parent_i = -1;
      cellDetails[i][j].parent_j = -1;
    }
  }
  //std::cout << "\nStarting Point: " << src.x << " " << src.y << std::endl;
  //std::cout << "Destination Point: " << destin.x << " " << destin.y << std::endl;
  // Initializing Source Node AND converting to new relative grid
  ActualToRelative(i,j,dispX,dispY,destin,src);
  
  cellDetails[i][j].f = 0;
  cellDetails[i][j].g = 0;
  cellDetails[i][j].h = 0;
  cellDetails[i][j].parent_i = i;
  cellDetails[i][j].parent_j = j;

  std::set<pPair> openList;
  openList.insert(std::make_pair(0.0f,std::make_pair(i,j)));
  
  while(!openList.empty()){
    pPair p = *openList.begin();
    openList.erase(openList.begin());

    i = p.second.first; 
    j = p.second.second;
    closedList[i][j] = true;
    // Checking Succesors
    if(i < SearchRegHeight && j < SearchRegWidth){ 
      if(processSuccessor(i,j,i+1,j+1,cellDetails,closedList,openList,destin, src, dispX, dispY))
	return;
    }
    if(i < SearchRegHeight){
      if(processSuccessor(i,j,i+1,j,cellDetails,closedList,openList,destin, src, dispX, dispY)) 
	return;
    }
    if(j < SearchRegWidth){
      if(processSuccessor(i,j,i,j+1,cellDetails,closedList,openList,destin, src, dispX, dispY)) 
	return;
    }
    if(i > 0){
      if(processSuccessor(i,j,i-1,j,cellDetails,closedList,openList,destin, src, dispX, dispY))
	return;
    }
    if(i > 0 && j < SearchRegWidth){
      if(processSuccessor(i,j,i-1,j+1,cellDetails,closedList,openList,destin, src, dispX, dispY)) 
	return;
    }
    if(j > 0){
      if(processSuccessor(i,j,i,j-1,cellDetails,closedList,openList,destin, src, dispX, dispY)) 
	return;
    }
    if(j > 0 && i < SearchRegHeight){
      if(processSuccessor(i,j,i+1,j-1,cellDetails,closedList,openList,destin, src, dispX, dispY))
	return;
    }
    if(i > 0 && j > 0){
      if(processSuccessor(i,j,i-1,j-1,cellDetails,closedList,openList,destin, src, dispX, dispY))
	return;
    }
  }
  if(!path.size())
    std::cout << "Path not found!\n";
  destin.x = destin.x*blockLength;
  destin.y = destin.y*blockLength;
}

bool Character::processSuccessor(int i, int j, int newi, int newj, std::vector<std::vector<cell>> &cellDetails,  std::vector<std::vector<bool>> &closedList, std::set<pPair> &openList, Vector2 destin, Vector2 src, int dispX, int dispY){
  bool Blocked1 = isUnBlocked(i,newj, dispX, dispY, src);
  bool Blocked2 = isUnBlocked(newi,j, dispX, dispY, src);
  bool Blocked3 = isUnBlocked(newi,newj, dispX, dispY, src);
  
  if(Blocked3){
    if(isDestination(newi, newj, destin) == true ){
      if(newi != i && newj != j && (Blocked1 == false || Blocked2 == false)){ // Case of moving diagonally
	return false;
      }
      //std::cout << "Found the desintation: " << newj << " " << newi << std::endl << std::endl;
      cellDetails[newi][newj].parent_i = i; 
      cellDetails[newi][newj].parent_j = j;  
      setPath(cellDetails, src, destin, dispX, dispY);
      return true; 
      } 
    else if(closedList[newi][newj] == false && Blocked3 == true){
      if(newi != i && newj != j && (Blocked1 == false || Blocked2 == false)){ //Case of moving diagonally
	return false;
      }
      float gNew = cellDetails[i][j].g + 1.0; 
      float hNew = computeH(newi, newj,destin); 
      float fNew = gNew + hNew; 
      if(cellDetails[newi][newj].f == FLT_MAX || cellDetails[newi][newj].f > fNew) { 
	openList.insert(std::make_pair (fNew, std::make_pair (newi, newj)));  
	cellDetails[newi][newj].f = fNew; 
	cellDetails[newi][newj].g = gNew; 
	cellDetails[newi][newj].h = hNew; 
	cellDetails[newi][newj].parent_i = i; 
	cellDetails[newi][newj].parent_j = j; 
      } 
    }
  }
  return false;
}
void Character::setPath(std::vector<std::vector<cell>> &cellDetails, Vector2 src, Vector2 destin, int dispX, int dispY){
  int row = destin.y; 
  int col = destin.x;

  std::stack<Pair> Path;
  std::stack<Pair> tempPath;
  //std::cout << "Relative Path: ";
  while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col )){ //Extract Path from cellDetails
    //std::cout << "(" << col << "," << row << ") ";
    Path.push (std::make_pair (row, col)); 
    int temp_row = cellDetails[row][col].parent_i; 
    int temp_col = cellDetails[row][col].parent_j; 
    row = temp_row; 
    col = temp_col; 
  }
  //std::cout << std::endl << std::endl;
  
  while(!Path.empty()){ // Convert the path coordinates
     Pair temp_p = Path.top();
     Path.pop();
     RelativeToActual(temp_p.first,temp_p.second,dispX,dispY,src);
     tempPath.push(temp_p);
  }
  
  while(!tempPath.empty()){ // Re-flip path order
    Path.push(tempPath.top());
    tempPath.pop();
   }

  RelativeToActual(row,col,dispX,dispY,src); // Convert the destination coors
  
  //Set as Path to be travelled
  Path.push (std::make_pair (row, col));
  startPos = Vector2{(float)blockLength*Path.top().second,(float)blockLength*Path.top().first};
  Path.pop(); //Don't need to travel to starting point
  //std::cout << "Actual Path: ";
  while (!Path.empty()){ 
    Pair p = Path.top();
    Vector2 temp = Vector2{(float)blockLength*p.second,(float)blockLength*p.first};
    this->path.push_back(temp);
    //std::cout << "(" << temp.x << " " << temp.y << ") ";
    Path.pop(); 
  }
  //std::cout << std::endl << std::endl;
  //map->map->find(Vector2{startPos.x,startPos.y})->second->setBlockType(BlockType::freeSpace);
  //map->map->find(Vector2{path.front().x,path.front().y})->second->setBlockType(BlockType::unitSpace); //Claiming first block
  //std::cout << std::endl;
  return; 
}

void Character::RelativeToActual(int &row, int &col, int dispX, int dispY, Vector2 src){
  if(src.x >= dispX)
    col = col - (RelativeGridDim/(blockLength*2)) + dispX;
  else
    col = col - (RelativeGridDim/(blockLength*2)) + src.x;
  if(src.y >= dispY)
    row = row - (RelativeGridDim/(blockLength*2)) + dispY;
  else
    row = row - (RelativeGridDim/(blockLength*2)) + src.y;
}

void Character::ActualToRelative(int &i, int &j, int &dispX,int &dispY, Vector2 &destin, Vector2 src){
  if(src.y >= destin.y){
    i = (RelativeGridDim/(blockLength*2)) + src.y-destin.y;
    dispY = destin.y;
    destin.y = destin.y - destin.y + (RelativeGridDim/(blockLength*2));
  }
  else{
    i = (RelativeGridDim/(blockLength*2));
    dispY = destin.y;
    destin.y = (RelativeGridDim/(blockLength*2)) + destin.y - src.y;
  }
  if(src.x >= destin.x){
    j = (RelativeGridDim/(blockLength*2)) + src.x - destin.x;
    dispX = destin.x;
    destin.x = (RelativeGridDim/(blockLength*2));
  }
  else{
    j = (RelativeGridDim/(blockLength*2));
    dispX = destin.x;
    destin.x = (RelativeGridDim/(blockLength*2)) + destin.x - src.x;
  }
}

bool Character::isUnBlocked(int row, int col, int dispX, int dispY, Vector2 src){ // Takes Relative Coordinates and Checks if block space is blocked
  RelativeToActual(row,col,dispX,dispY,src);
  if(map->GetBlock(Vector2{col*blockLength,row*blockLength}) != nullptr){
    return !map->GetBlock(Vector2{col*blockLength,row*blockLength})->isBlocked();
  }
  return false;
}

int Character::Vec2Quad(Vector2 v){
  if(v.x >= 0 && v.y >= 0)
    return 1;
  else if(v.x <= 0 && v.y >= 0)
    return 2;
  else if(v.x <= 0 && v.y <= 0)
    return 3;
  else
    return 4;
}

void Character::analyzeMCP(Vector2 MCP){
  if(map->GetBlock(MCP) != nullptr)
    MCP_type = map->GetBlock(MCP)->getBlockType();
}

bool Character::isDestination(int row, int col, Vector2 destin){
  if(row == destin.y && col == destin.x)
    return true;
  return false;
}

float Character::computeH(int row, int col, Vector2 destin){
  return (float)sqrt((row-destin.y)*(row-destin.y) + (col-destin.x)*(col-destin.x));
}

void Character::draw(){
  static int frame = 1;
  static float timer = 0.0f;
  Item* ActiveItem = Inven->GetActiveItem();
  if(path.size()){ // Is Moving
    timer += GetFrameTime();
    if(timer > 0.25f){
      timer = 0.0f;
      frame = (frame % 8) + 1;
    }
    DrawTextureRec(AvatarSkin, {blockLength*frame,Direction*blockLength,blockLength,blockLength},{body.x,body.y},RAYWHITE);
    switch(frame){
    case 1:
      if(!Direction)
	ActiveItem->Draw({body.x+8, body.y}, Direction);
      else
	ActiveItem->Draw({body.x-7, body.y}, Direction);
      break;
    case 2:
      if(!Direction)
	ActiveItem->Draw({body.x+8, body.y}, Direction);
      else
	ActiveItem->Draw({body.x-8, body.y}, Direction);
      break;
    case 3:
      if(!Direction)
	ActiveItem->Draw({body.x+9, body.y-1}, Direction);
      else
	ActiveItem->Draw({body.x-9, body.y-1}, Direction);
      break;
    case 4:
      if(!Direction)
	ActiveItem->Draw({body.x+8, body.y}, Direction);
      else
	ActiveItem->Draw({body.x-8, body.y}, Direction);
      break;
      }
  }
  else{ // Is still
    DrawTextureRec(AvatarSkin, {0,Direction*blockLength,blockLength,blockLength},{body.x,body.y},RAYWHITE);
    if(!Direction)
      ActiveItem->Draw({body.x+7, body.y+1}, Direction);
    else
    ActiveItem->Draw({body.x-7, body.y+1}, Direction);
  }
  
  /*switch(selected){
  case true:
    DrawRectanglePro(Rectangle{body.x,body.y-(blockLength/5),body.width*(this->health/100),blockLength/10}, Vector2{0,0},0.0f,GREEN);
    DrawTexture(AvatarSkin, body.x,body.y,RAYWHITE);
    break;
    }*/
  
}

void Character::KeyPressAnalysis(){
  switch(GetKeyPressed()){
  case KEY_ONE:
    Inven->SetActiveSlot(0);
    break;
  case KEY_TWO:
    Inven->SetActiveSlot(1);
    break;
  case KEY_THREE:
    Inven->SetActiveSlot(2);
    break;
  case KEY_FOUR:
    Inven->SetActiveSlot(3);
    break;
  case KEY_FIVE:
    Inven->SetActiveSlot(4);
    break;
  case KEY_SIX:
    Inven->SetActiveSlot(5);
    break;
  case KEY_SEVEN:
    Inven->SetActiveSlot(6);
    break;
  case KEY_EIGHT:
    Inven->SetActiveSlot(7);
    break;
  case KEY_NINE:
    Inven->SetActiveSlot(8);
    break;
  case KEY_ZERO:
    Inven->SetActiveSlot(9);
    break;
  case KEY_SPACE:
    UseActiveItem();
    break;
  }
}

void Character::UseActiveItem(){
  //Run Animation of Using Item

  //Run something like map->map->find(Block Player is facing)->second->HitBy(Item CurrItem)
  if(map->GetBlock({body.x + blockLength, body.y}) != nullptr)
    map->GetBlock({body.x + (-1*Direction+(Direction < 1))*blockLength, body.y})->HitBy(Inven->GetItem(Inven->GetActiveSlot()));
}

bool Character::isSelected(){return selected;}

void Character::select(){if(this->selected == false){this->selected = true; displacement = numSelected; numSelected++;}}
void Character::deselect(){if(this->selected == true){this->selected = false; displacement = 0; numSelected--;}}
void Character::setSpeed(float newSpeed){this->speed = newSpeed;}
void Character::setHealth(float newHealth){this->health = newHealth;}

float Character::getHeight(){return body.height;}
float Character::getWidth(){return body.width;}
float Character::getX(){return body.x;}
float Character::getY(){return body.y;}


void Character::SetWorldName(std::string NewName){
  this->WorldName = NewName;//SHould probably be static eh?
}

void Character::CreateWorldSaveDir(){
  std::string TempPath;
  TempPath = "data/worlds/";
  TempPath.append(WorldName);
  std::filesystem::create_directories(TempPath);
  TempPath.append("/player");
  std::filesystem::create_directories(TempPath);
}

void Character::LoadData(){
  //Open file location and save data
  std::ifstream iStream;
  iStream.open("data/worlds/" + WorldName + "/player/status.txt");
  if(!iStream.fail()){
    iStream >> body.x >> body.y >> Direction;
  }
  else
    std::cout << "ERROR FAILED TO LOAD" << "/data/worlds/" << WorldName << "/player/status.txt\n";
}

void Character::SaveData(){
  //Open the file location and save data
  std::ofstream oStream;
  std::string path = "data/worlds/" + WorldName + "/player/status.txt";
  oStream.open(path, std::ofstream::out | std::ofstream::trunc);
  oStream << body.x << " " << body.y << " " << Direction;
}

void Character::Reset(){
  body.x = 0;
  body.y = 0;
  health = 100;
  Direction = 0;
  path.clear();
}
