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
#include <vector>
#include <time.h>
#include <list>
#include "geometry.h"
#include <queue>

#include "Node.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "free_camera.h"
#include "Theme.h"
#include "PerlinNoise.h"
#include "ppm.h"

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

	theme* layout;
	Transform temp_trans;
	geometry terrain;

	std::vector <Mesh*> Buildings;
	std::vector <Texture*> Buildings_Tex;
	std::vector <Transform> Buildings_Trans;


	std::vector <Mesh*> HelathPacks;
	std::vector <Texture*> HelathPacks_Tex;
	std::vector <Transform> HelathPacks_Trans;

	std::vector <Mesh*> Sides;
	std::vector <Texture*> Sides_Tex;
	std::vector <Transform> Sides_Trans;

	std::vector <Mesh*> Node_Mesh;
	std::vector <Texture*> Node_Tex;
	std::vector <Transform> Node_Trans;

	//values for size
	int coordx;
	int coordy;
	int camSpeed;
	int sides;


	std::priority_queue<node> untriedNodes[2]; // list of open (not-yet-tried) nodes
	std::deque<ivec2> route; //The series of waypoins to be followed
	int **nodeMap;
	int **closedNodes; // map of closed (tried-out) nodes
	int **openNodes; // map of open (not-yet-tried) nodes
	int **direction; // map of directions
	int nodeIndex = 0;
	int directions = 8;
	

	// Setup scene. Last seed params is optional; = denotes default value
	// If called from loadGameScene, requires seed value(as string)
	void Init(GLFWwindow* window);
	void Loop(GLFWwindow* window);
	void Input(GLFWwindow* window);
	void Update(GLFWwindow* window);
	void Render(GLFWwindow* window);
	void Algortithm(GLFWwindow* window);
	void CreateScene(GLFWwindow* window);
	void CreatePath(GLFWwindow* window);
	void CreateNoise();
	void CreateTerrain(GLFWwindow* window , geometry &geom,const Texture *height_map, unsigned int width, unsigned int depth, float height_scale);
};