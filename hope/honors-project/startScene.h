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


	Shader* textureShader;
	Transform shaderTrans;

	// Cameras
	free_camera* freeCam;
	double cursor_x, cursor_y = 0.0;// For finding cursor pos on screen
	
	Mesh* plainMesh;
	Texture* plainTexture;
	Transform plainTransform;

	Mesh* startMesh;
	Texture* startTexture;
	Transform startTransform;

	Mesh* endMesh;
	Texture* endTexture;
	Transform endTransform;

	//values for size
	int coordx;
	int coordy;
	int camSpeed;

	// Setup scene. Last seed params is optional; = denotes default value
	// If called from loadGameScene, requires seed value(as string)
	void Init(GLFWwindow* window);
	void Loop(GLFWwindow* window);
	void Input(GLFWwindow* window);
	void Update(GLFWwindow* window);
	void Render(GLFWwindow* window);
	void Algortithm(GLFWwindow* window);
	void CreateScene(GLFWwindow* window);
};