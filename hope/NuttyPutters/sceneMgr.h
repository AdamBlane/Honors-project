#pragma once
// Externals
#include <string>
// Internals
#include "startScene.h"


class sceneMgr {
public:
	// Use int for switch in winMgr.update
	unsigned int curScene;

	// Constructor
	sceneMgr();
	// Deconstructor
	~sceneMgr();

	// Scenes
	startScene startScene; //1

	// Scene get/set; optional param: difficulty via course size, load game seed string
	void changeScene(unsigned int newScene, int courseLength = 12); 
	unsigned int getScene() { return curScene; }


};