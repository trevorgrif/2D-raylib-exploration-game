#include "Character.h"
#include <iostream>
#include <math.h>

int Character::numSelected{0};
ChunkType Character::MCP_type{null};
Vector2 Character::mouseClickPoint;
//TODO: BUGS: Players stop moving as soon as BlockedChunk is clicked. Make players finish movement to current chunk alwaysss. Maybe Restructure how movement is handeled...
Character::Character(Rectangle Body,  const char* name, Camera2D* camera, std::map<Vector2,Chunk*,Vec2Compare>* map, Vector2 mapDim){
  this->body = Body;
  this-> name = name;
  this->camera = camera;
  this->map = map;
  this->mapDim.x = mapDim.x;
  this->mapDim.y = mapDim.y;
  for(int i = modChunkLength(body.x); i < modChunkLength(body.x+body.width); i = i+chunkLength){
    for(int j = modChunkLength(body.y); j < modChunkLength(body.y+body.height); j = j+chunkLength){
      map->find(Vector2{(float)i,(float)j})->second->setChunkType(unitSpace);
    }
  }
}

void Character::updateUnit(Camera2D* camera){ // Interprets mouse action and updates unit accordingly
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
      if(abs(x_init-currV.x) > 2*chunkLength || abs(y_init- currV.y) > 2*chunkLength){
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
	  DrawRectangleLinesEx(region,(int)(chunkLength/5), BLACK);
	  break;
	case 2:
	  region = Rectangle{currV.x, (float)y_init, x_init-currV.x, currV.y-y_init};
	  DrawRectangleLinesEx(region ,(int)(chunkLength/5), BLACK);
	  break;
	case 3:
	  region = Rectangle{currV.x, currV.y, x_init-currV.x, y_init-currV.y}; 
	  DrawRectangleLinesEx(region ,(int)(chunkLength/5), BLACK);
	  break;
	case 4:
	  region = Rectangle{(float)x_init, currV.y, currV.x-x_init, y_init-currV.y};
	  DrawRectangleLinesEx(region ,(int)(chunkLength/5), BLACK);
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
      mouseClickPoint.x = mPos.x;
      mouseClickPoint.y = mPos.y;
      analyzeMCP();
      if(MCP_type == unitSpace){
	Chunk* chunk = map->find(Vector2{modChunkLength(mouseClickPoint.x),modChunkLength(mouseClickPoint.y)})->second;
	if(chunk->getRect().x == modChunkLength(getX()) && chunk->getRect().y == modChunkLength(getY()))
	  this->select();
	MCP_type = null;
      }
      else if(MCP_type == freeSpace || MCP_type == structSpace){ // Find closest actual free space pos. //TODO: Fix structSpace so that sceneSpaces are clickable later
	if(this->selected == true){
	  mPos = GetScreenToWorld2D(GetMousePosition(),*camera);
	  destinX = mPos.x;
	  destinY = mPos.y;
	  if(path.size()){
	    map->find(Vector2{path.front().x,path.front().y})->second->setChunkType(ChunkType::freeSpace);
	    map->find(Vector2{startPos.x,startPos.y})->second->setChunkType(ChunkType::freeSpace);
	  }
	  findNearestFreeChunk();
	}
	MCP_type = null;
      }
    }  
    markSet = false;
    regionActive = false;
  }
  if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){ // Check for deselect request
    if(this->selected == true)
      this->deselect();
  }
  if(path.size())
    moveAlongPath();
}

void Character::findNearestFreeChunk(){ // Finds nearest open space and sets destinX and destinY to that location
  while(path.size() && map->find(Vector2{path.back().x,path.back().y})->second->isBlocked() == true){ 
    if(path.size() == 1){
      destinX = body.x;
      destinY = body.y;
      path.pop_back();
    }
    else{
      path.pop_back();
      destinX = path.back().x;
      destinY = path.back().y;
    }
  }
  destinX = modChunkLength(destinX);
  destinY = modChunkLength(destinY);
  path.clear();
  findPath();
}

void Character::moveAlongPath(){ // Increments player.pos towards next point on path
  Vector2 nextPoint = path.front();
  float x = nextPoint.x;
  float y = nextPoint.y;
 
  if(abs(body.x-x) <= speed*GetFrameTime() && abs(body.y-y) <= speed*GetFrameTime()) // If about to arrive
    map->find(Vector2{startPos.x,startPos.y})->second->setChunkType(ChunkType::freeSpace); // Free old chunk
  if(abs(body.x-x) <= speed*GetFrameTime()) //if close enough then set equal and stop moving"
    body.x = x;
  if(abs(body.y-y) <= speed*GetFrameTime())
    body.y = y;
  if(body.y == y && body.x == x){
    startPos = Vector2{path.front().x,path.front().y};
    path.pop_front();
    if(path.size()){
      if(map->find(Vector2{path.front().x,path.front().y})->second->isBlocked() == true){// If path is obstructed along the journey then find new path
	std::cout << this->name << " path is obstructed at " << path.front().x << " " << path.front().y << std::endl;
	findNearestFreeChunk();
      }
      if(map->find(Vector2{path.front().x,path.front().y})->second->isBlocked() == false && path.size()){
	map->find(Vector2{path.front().x,path.front().y})->second->setChunkType(ChunkType::unitSpace);
      }
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

void Character::findPath(){ //A* Search path finding
  std::cout << "Trying to find path for " << this->name << " to the point " << destinX << " " << destinY << std::endl;
  if(map->find(Vector2{destinX,destinY})->second->getChunkType() == ChunkType::structSpace)
    return;
  destinX = destinX/chunkLength;
  destinY = destinY/chunkLength;
  Vector2 src = Vector2{modChunkLength(body.x)/chunkLength,modChunkLength(body.y)/chunkLength};
  if(isDestination(src.y,src.x))
    return;
  
  std::vector<std::vector<bool>> closedList(mapDim.y/chunkLength, std::vector<bool> (mapDim.x/chunkLength,false));
  std::vector<std::vector<cell>> cellDetails(mapDim.y/chunkLength, std::vector<cell> (mapDim.x/chunkLength));
  int i,j;

  for(i = 0; i < mapDim.y/chunkLength; i++){
    for(j = 0; j < mapDim.x/chunkLength; j++){
      cellDetails[i][j].f = FLT_MAX;
      cellDetails[i][j].g = FLT_MAX;
      cellDetails[i][j].h = FLT_MAX;
      cellDetails[i][j].parent_i = -1;
      cellDetails[i][j].parent_j = -1;
    }
  }

  // Initializing Source Node
  i = src.y;
  j = src.x;
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
    if(processSuccessor(i,j,i-1,j,cellDetails,closedList,openList))
      return;
    if(processSuccessor(i,j,i+1,j,cellDetails,closedList,openList))
      return;  
    if(processSuccessor(i,j,i,j+1,cellDetails,closedList,openList))
      return; 
    if(processSuccessor(i,j,i,j-1,cellDetails,closedList,openList))
      return;
    if(processSuccessor(i,j,i-1,j+1,cellDetails,closedList,openList))
      return;
    if(processSuccessor(i,j,i+1,j+1,cellDetails,closedList,openList))
      return;
    if(processSuccessor(i,j,i+1,j-1,cellDetails,closedList,openList))
      return;
    if(processSuccessor(i,j,i-1,j-1,cellDetails,closedList,openList))
      return;
  }
  destinX = destinX*chunkLength;
  destinY = destinY*chunkLength;
}

bool Character::processSuccessor(int i, int j, int newi, int newj, std::vector<std::vector<cell>> &cellDetails,  std::vector<std::vector<bool>> &closedList, std::set<pPair> &openList){
  if(isValid(newi, newj) == true){ 
      if(isDestination(newi, newj) == true && isUnBlocked(i,newj) == true && isUnBlocked(newi,j) == true){  
	  cellDetails[newi][newj].parent_i = i; 
	  cellDetails[newi][newj].parent_j = j;  
	  setPath(cellDetails);
	  destinX = destinX*chunkLength;
	  destinY = destinY*chunkLength;
	  return true; 
	} 
      else if(closedList[newi][newj] == false && isUnBlocked(newi, newj) == true && isUnBlocked(i,newj) == true && isUnBlocked(newi,j) == true){ 
	  float gNew = cellDetails[i][j].g + 1.0; 
	  float hNew = computeH(newi, newj); 
	  float fNew = gNew + hNew; 
	  if(cellDetails[newi][newj].f == FLT_MAX || cellDetails[newi][newj].f > fNew) { 
	      openList.insert( std::make_pair (fNew, std::make_pair (newi, newj)));  
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
void Character::setPath(std::vector<std::vector<cell>> &cellDetails){
  std::cout << this->name << ": ";
  int row = destinY; 
  int col = destinX; 
  
  std::stack<Pair> Path; 
  
  while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col )){ 
    Path.push (std::make_pair (row, col)); 
    int temp_row = cellDetails[row][col].parent_i; 
    int temp_col = cellDetails[row][col].parent_j; 
    row = temp_row; 
    col = temp_col; 
  } 
  Path.push (std::make_pair (row, col));
  startPos = Vector2{(float)chunkLength*Path.top().second,(float)chunkLength*Path.top().first};
  Path.pop(); //Don't need to travel to starting point
  while (!Path.empty()){ 
    Pair p = Path.top();
    Vector2 temp = Vector2{(float)chunkLength*p.second,(float)chunkLength*p.first};
    this->path.push_back(temp);
    std::cout << "(" << temp.x << " " << temp.y << ") ";
    Path.pop(); 
  }
  map->find(Vector2{path.front().x,path.front().y})->second->setChunkType(ChunkType::unitSpace); //Claiming first chunk
  std::cout << std::endl;
  return; 
}

bool Character::isUnBlocked(int row, int col){
  return !map->find(Vector2{col*chunkLength,row*chunkLength})->second->isBlocked();
}

int Character::Vec2Quad(Vector2 v){ //Returns quadrant BR = 1, BL = 2, TL = 3, TR = 4
  if(v.x >= 0 && v.y >= 0)
    return 1;
  else if(v.x <= 0 && v.y >= 0)
    return 2;
  else if(v.x <= 0 && v.y <= 0)
    return 3;
  else
    return 4;
}

void Character::analyzeMCP(){
  auto it = map->cend();
  it--;
  if(mouseClickPoint.x < 0 || mouseClickPoint.y < 0 || mouseClickPoint.x > it->first.x + chunkLength || mouseClickPoint.y > it->first.y + chunkLength) //End of Map
    return;
  Chunk* chunk = map->find(Vector2{modChunkLength(mouseClickPoint.x),modChunkLength(mouseClickPoint.y)})->second;
  if(chunk->getChunkType() == unitSpace){;
    MCP_type = unitSpace;
  }
  else if(chunk->getChunkType() == freeSpace){
    MCP_type = freeSpace;
  }
  else if(chunk->getChunkType() == structSpace){
    MCP_type = structSpace;
  }
}

bool Character::isDestination(int row, int col){
  if(row == destinY && col == destinX)
    return true;
  return false;
}

bool Character::isValid(int row, int col){return   (row >= 0) && (row < mapDim.y/chunkLength) && (col >= 0) && (col < mapDim.x/chunkLength);}

float Character::computeH(int row, int col){
  return (float)sqrt((row-destinY)*(row-destinY) + (col-destinX)*(col-destinX));
}

bool Character::isSelected(){return selected;}

void Character::select(){if(this->selected == false){this->selected = true; displacement = numSelected; numSelected++;}}
void Character::deselect(){if(this->selected == true){this->selected = false; displacement = 0; numSelected--;}}
void Character::draw(){DrawRectanglePro(body, Vector2{0,0},0.0f,BLACK);}
void Character::setSpeed(float newSpeed){this->speed = newSpeed;}

float Character::getHeight(){return body.height;}
float Character::getWidth(){return body.width;}
float Character::getX(){return body.x;}
float Character::getY(){return body.y;}
