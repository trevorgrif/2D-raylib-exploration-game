#ifndef TEXTURES_H
#define TEXTURES_H

#include "enums.h"
#include <vector>
#include "raylib.h"

class TextureLoader {
 private:
  std::vector<Texture2D> TextureList;
  
 public:
  TextureLoader();

  void LoadTextures();
  Texture2D GetTexture(global_enums::OBJECTS name){ return TextureList[name];};
};

extern TextureLoader Textures;

#endif //TEXTURES_H
