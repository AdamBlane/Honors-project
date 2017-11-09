#pragma once
// Externals
#include "glew_glfw.h"
#include <map>
#include <time.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <random>
#include <windows.h>
#include <atlimage.h>
#include <gdiplusimaging.h>

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "free_camera.h"

using namespace AllCamera;
using namespace std::chrono;

class startScene
{
	public:
	// Default constructor
		startScene();
	// Deconstructor
	~startScene();

	//Trying Skybox
	Mesh* sky;
	Transform skyTransform;
	Shader* skyShader;

	Shader* textureShader;
	Transform shaderTrans;

	double lastFrame = 0;
	double thisFrame = glfwGetTime();
	float dt = 0.016; // First frame; reset thereafter
					  // Rename
	// Cameras
	free_camera* freeCam;
	double cursor_x, cursor_y = 0.0;// For finding cursor pos on screen
	float camSpeed = 1.0f;
	Mesh* arrowMesh;
	Texture* arrowTexture;
	Transform arrowTransform;


	// Setup scene. Last seed params is optional; = denotes default value
	// If called from loadGameScene, requires seed value(as string)
	void Init(GLFWwindow* window);
	void Loop(GLFWwindow* window);
	void Input(GLFWwindow* window);
	void Update(GLFWwindow* window);
	void Render(GLFWwindow* window);
};