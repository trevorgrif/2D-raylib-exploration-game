#ifndef ENUMS_H
#define ENUMS_H

namespace global_enums{
  enum OBJECT_ACTIONS {DEL, null, CREATE_LOG, CREATE_STICK, CREATE_TREE};
  enum OBJECTS {
		//Null
		Empty=0,
		
		//Tools
		Wood_Sword,
		Wood_Axe,

		//Scene Elements
		Tree,
		Log,
		Stick,

		//Blocks
		Water,
		ShallowWater,
		DeepWater,
		Sand,
		Grass,
		Forest,
		WetSand,
		SandyGrass

		//Misc
	    
};
}

#endif //ENUMS_H
