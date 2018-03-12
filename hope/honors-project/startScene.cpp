// Externals

// Internals
#include "startScene.h"
#include "windowMgr.h"
#include "PerlinNoise.h"
#include "ppm.h"

#include <time.h>
#include<list>

// Default constructor
startScene::startScene() { }
// Deconstructor
startScene::~startScene() { }
struct node 
{
	int x_coord, y_coord;
};
std::list<node> nodes;
std::list <node> wanted_nodes;
// Setup scene; seed is an optional param passed in by loadGameScene
void startScene::Init(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, cursor_x, cursor_y);
	//for (int i = 0; i < 100; i++) 
	//{
		CreateScene(window);
		CreatePath(window);
		Algortithm(window);
	//}
	CreateNoise();
	textureShader = new Shader("..\\honors-project\\textureShader");
	plainMesh = new Mesh(Mesh::CUBOID, "..\\honors-project\\box.jpg", vec3(0.0f, 0.0f, 0.0f), coordx, 0.1f, coordy);
	plainTexture = new Texture("..\\honors-project\\RESULT.png");

	startMesh = new Mesh(Mesh::CUBOID, "..\\honors-project\\box.jpg", vec3(0.0f, 0.0f, 0.0f), 5.0f, 10.0f, 5.0f);
	startTexture = new Texture("..\\honors-project\\ballRed.jpg");

	endMesh = new Mesh(Mesh::CUBOID, "..\\honors-project\\box.jpg", vec3(coordx, 0.0f, coordy), 5.0f, 10.0f, 5.0f);
	endTexture = new Texture("..\\honors-project\\ballBlue.jpg");

	plainTransform.setPos(vec3(coordx/2, 0, coordy/2));
	freeCam = new free_camera();
	freeCam->set_Posistion(vec3(0, 10, -10));
	freeCam->rotate(-10.0, 0.0);
	freeCam->set_Target(vec3(0, 0, 0));
	freeCam->set_projection(quarter_pi<float>(), (float)1600 / (float)900, 0.414f, 30000.0f);
}
void startScene::CreateNoise() 
{
    int wi = 1000, he = 1000;
	ppm image(wi, he);
	unsigned int seed = 7;
	PerlinNoise pn(seed);

	unsigned int kk = 0;
	for (int i = 0; i < he; i++) 
	{
		for (int j = 0; j < wi; j++)
		{
			double x = (double)j / ((double)wi);
			double y = (double)i / ((double)he);

			double n = 20 * pn.noise(x/0.1, y/0.1, 0);
			n = n - floor(n * n);

			image.r[kk] = floor(255 * n);
			image.g[kk] = floor(255 * n);
			image.b[kk] = floor(255 * n);
			kk++;
		}
	}
	image.write("RESULT.ppm");
	
}
void startScene::CreateTerrain()
{

}
void startScene::CreatePath(GLFWwindow* window) 
{
	for (int i = 0; i < 30; i++) 
	{
		node a;
		a.x_coord = rand() % coordx;
		a.y_coord = rand() % coordy;
		nodes.push_back(a);
	}
	/*while (nodes.size != 0) 
	{

	}*/
}
void startScene::CreateScene(GLFWwindow* window)
{
	std::srand(time(0));
	std::default_random_engine gen;
	std::binomial_distribution<int> spped(11, 0.5);
	std::binomial_distribution<int> dist(20000, 0.5);
	coordx = dist(gen)+1000;
	coordy = dist(gen)+1000;
	camSpeed = 10 * spped(gen);
}

void startScene::Algortithm(GLFWwindow* window) 
{
	float length_of_map = (coordx * coordx) + (coordy * coordy);
	length_of_map = std::sqrtf(length_of_map);
	float time = length_of_map / camSpeed;
	std::cout << "map co-ordinates = (" << coordx << " , " << coordy << ")" << std::endl;
	std::cout << "length of map = " << length_of_map << std::endl;
	std::cout << "speed of player = (" << camSpeed << ")" << std::endl;
	std::cout << "time = (" << time << ")" << std::endl;
}
void startScene::Loop(GLFWwindow* window)
{
	glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Input(window);
	Update(window);
	Render(window);

}

// Act on input
void startScene::Input(GLFWwindow* window)
{
	vec3 freeCamPos = vec3(0, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		freeCamPos = (vec3(0, 0, camSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		freeCamPos = (vec3(-camSpeed, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		freeCamPos = (vec3(0, 0, -camSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		freeCamPos = (vec3(camSpeed, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_Q))
	{
		freeCamPos = (vec3(0, camSpeed, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_E))
	{
		freeCamPos = (vec3(0, -camSpeed, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_M))
	{
		camSpeed = 100.0f;
	}

	freeCam->move(freeCamPos);
}

void startScene::Update(GLFWwindow* window)
{
	static double ratio_width = quarter_pi<float>() / 1600.0;
	static double ratio_height = (quarter_pi<float>() * (1600 / 900)) / static_cast<float>(1600);
	double current_x, current_y;
	glfwGetCursorPos(window, &current_x, &current_y);
	// Calc delta
	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;
	// Multiply deltas by ratios
	delta_x *= ratio_width;
	delta_y *= ratio_height * -1; 
	freeCam->rotate(delta_x, delta_y);
	freeCam->update(0.001);
	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;
}


void startScene::Render(GLFWwindow* window)
{
	// Calculate mvp matrix
	mat4 mvp;
	// If camera type is free camera then
	mvp = freeCam->get_Projection() * freeCam->get_View();
	textureShader->Bind();
	plainTexture->Bind(0);
	textureShader->Update(plainTransform, mvp);
	plainMesh->Draw();
	for (int i = 0; i > 30; i++) 
	{

	}
	startTexture->Bind(0);
	textureShader->Update(startTransform, mvp);
	startMesh->Draw();

	endTexture->Bind(0);
	textureShader->Update(endTransform, mvp);
	endMesh->Draw();

	textureShader->Update(shaderTrans, (freeCam->get_Projection() * freeCam->get_View()));
	glfwSwapBuffers(window);
	glfwPollEvents();
}