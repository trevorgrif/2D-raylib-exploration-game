#include "textures.h"
#include <iostream>

TextureLoader Textures;

TextureLoader::TextureLoader(){}

void TextureLoader::LoadTextures(){
  //Load all the Texures into the list. Order matters (match with enums.h list order)
  //Empty Hand
  TextureList.push_back(LoadTexture("textures/items/empty.png"));
  
  //Tools
  TextureList.push_back(LoadTexture("textures/items/wood_sword.png"));
  TextureList.push_back(LoadTexture("textures/items/wood_axe.png"));

  //Scene Elements
  TextureList.push_back(LoadTexture("textures/items/tree.png"));
  TextureList.push_back(LoadTexture("textures/items/log.png"));
  TextureList.push_back(LoadTexture("textures/items/stick.png"));
  
  //Blocks
  TextureList.push_back(LoadTexture("textures/blocks/water.png"));
  TextureList.push_back(LoadTexture("textures/blocks/shallowwater.png"));// This is dumb fix this
  TextureList.push_back(LoadTexture("textures/blocks/deepwater.png"));
  TextureList.push_back(LoadTexture("textures/blocks/sand.png"));
  TextureList.push_back(LoadTexture("textures/blocks/grass.png"));
  TextureList.push_back(LoadTexture("textures/blocks/grass.png"));
  TextureList.push_back(LoadTexture("textures/blocks/wetsand.png"));
  TextureList.push_back(LoadTexture("textures/blocks/sandygrass.png"));

  //Misc
}
