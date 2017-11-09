
#include "sceneMgr.h"
#include "windowMgr.h"
/// Scene Manager is a property of windowMgr
/// It tracks gamestate, called by scene files on particular input

// Constructor; set to start scene
sceneMgr::sceneMgr()
{
	// On construction, first scene is start menu
	curScene = 1;
}

// Deconstructor
sceneMgr::~sceneMgr() { }

// Update game state to new scene, initialise the new scene
void sceneMgr::changeScene(unsigned int newScene, int courseLength)
{
	// Change current scene tracker to new scene
	curScene = newScene;

	// Initialise the new scene
	switch (curScene)
	{
	// Exit
	case 0: break;
	// Start scene
	case 1: 
		// Setup, pass in window held by winMgr
		startScene.Init(windowMgr::getInstance()->win);
		break;
	}
}



