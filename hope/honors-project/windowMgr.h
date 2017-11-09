#pragma once

// Externals
#include "glew_glfw.h"

// Internals
#include "sceneMgr.h"

class windowMgr
{
private:
	// Store singleton instance
	static windowMgr* instance;
	// Private constructor to avoid instancing
	windowMgr();
	// Deconstructor
	~windowMgr();
public:
	// Static singleton access method
	static windowMgr* getInstance();
	// This class creates and deals with the window
	GLFWwindow* win;
	// Track gamestate
	sceneMgr sceneManager;


	// Perform initial window setup
	GLFWwindow* windowMgr::Init();
	// Game update loop
	void Update();
	// On window close
	void CleanUp();
};