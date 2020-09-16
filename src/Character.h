#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include "ProcMap.h"
#include <map>
#include <list>
#include <set>
#include <cstring>
#include <utility>
#include <bits/stdc++.h>
#include "Block.h"
#include "FastNoise.h"
#include "Inventory.h"

typedef std::pair<int,int> Pair;
typedef std::pair<float,Pair> pPair;

struct cell{
  int parent_i, parent_j;
  double f,g,h; //f = g + h
};

class Character{
 private:
  ProcMap* map;
  std::list<Vector2> path;
  Camera2D* camera;
 
  std::map<std::string,Item>* itemTable;
  
  Vector2 currV;
  Vector2 startPos;
  Vector2 mapDim;
  
  const char * name;

  static Vector2 mouseClickPoint;
  static BlockType MCP_type;
  static int numSelected;
  
  Rectangle body;
  Rectangle region;

  int x_init{GetMouseX()};
  int y_init{GetMouseY()};
  int RelativeGridDim{4*16*(int)blockLength};
  int Direction{0}; // 0 == Right 1 == Left

  float speed{8*blockLength}; //Note: unaffected by Monitor FPS since GetFrameTime() is called when moving
  float displacement;
  float health{100};
  
  bool CameraFocus{true}; //Unused
  bool selected{false};
  bool isMoving{false};
  bool regionActive{false};
  bool markSet{false};

  Texture2D AvatarSkin;
  
 public:
  Character(Rectangle Body, const char * name, Camera2D* camera, ProcMap* map, std::map<std::string,Item>* itemTable);
  Character(Rectangle Body, const char * name, Camera2D* camera, ProcMap* map, std::map<std::string,Item>* itemTable, std::string NameArr[10]);
  Character(){};

  Inventory* Inven = new Inventory; //Need to delete this

  int Vec2Quad(Vector2 v);

  bool isUnBlocked(int row, int col, int dispX, int dispY, Vector2 src);  
  bool isDestination(int row, int col, Vector2 destin);
  bool isValid(int row, int col);
  bool isSelected();
  bool processSuccessor(int i, int j,int newi,int newj, std::vector<std::vector<cell>> &cellDetails,  std::vector<std::vector<bool>> &closedList, std::set<pPair> &openList, Vector2 destin, Vector2 src,int dispX,int dispY);

  float computeH(int row, int col, Vector2 destin);
  float getHeight();
  float getWidth();
  float getX();
  float getY();

  void setPath(std::vector<std::vector<cell>> &cellDetails, Vector2 src, Vector2 destin, int dispX,int dispY);
  void RelativeToActual(int &row, int &col, int dispX, int dispY, Vector2 src);
  void ActualToRelative(int &i, int &j, int &dispX, int &dispY,Vector2 &destin, Vector2 src);
  void findPath(Vector2 destin); //A* Search Algorithm
  void updateUnit(Camera2D* camera);
  void KeyPressAnalysis();
  void UseActiveItem();
  void setSpeed(float newSpeed);
  void findNearestFreeBlock(Vector2 &destin);
  void moveAlongPath(Vector2 &destin);
  void analyzeMCP(Vector2 MCP);
  void deselect();
  void setHealth(float newHealth);
  void select();
  void draw();
};
#endif //CHARACTER_H

