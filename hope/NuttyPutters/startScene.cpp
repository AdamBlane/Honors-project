// Externals

// Internals
#include "startScene.h"
#include "windowMgr.h" // to access singleton

// Default constructor
startScene::startScene() { }
// Deconstructor
startScene::~startScene() { }

// Setup scene; seed is an optional param passed in by loadGameScene
void startScene::Init(GLFWwindow* window)
{
	// Set GL properties 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, cursor_x, cursor_y);

	// Setup texture shader
	textureShader = new Shader("..\\NuttyPutters\\textureShader");

	// Arrow
	arrowMesh = new Mesh(Mesh::CUBOID, "..\\NuttyPutters\\box.jpg", vec3(1.8f,  2.6f, 0.0f), 3.0f, 0.5f, 0.5f);
	arrowTexture = new Texture("..\\NuttyPutters\\grass.png");
	arrowTransform.getScale() = vec3(0.5);
	// Setup cameras
	freeCam = new free_camera();
	freeCam->set_Posistion(vec3(0, 10, -10));
	freeCam->rotate(-10.0, 0.0);
	freeCam->set_Target(vec3(0, 0, 0));
	freeCam->set_projection(quarter_pi<float>(), (float)1600 / (float)900, 0.414f, 1000.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Main game loop 
void startScene::Loop(GLFWwindow* window)
{
	// Scene background
	glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Input
	Input(window);
	// Update
	Update(window);
	// Render
	Render(window);

}

// Act on input
void startScene::Input(GLFWwindow* window)
{
	// Exit
	if (glfwGetKey(window, GLFW_KEY_B))
	{
		// Access singleton instance to update it's sceneManager's state
		windowMgr::getInstance()->sceneManager.changeScene(0);
	}

	// Create vector to apply to current cam pos
	vec3 freeCamPos = vec3(0, 0, 0);

	// Camera controls
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		freeCamPos = (vec3(0, 0, camSpeed * dt));
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		freeCamPos = (vec3(-camSpeed * dt, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		freeCamPos = (vec3(0, 0, -camSpeed * dt));
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		freeCamPos = (vec3(camSpeed * dt, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_Q))
	{
		freeCamPos = (vec3(0, camSpeed * dt, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_E))
	{
		freeCamPos = (vec3(0, -camSpeed * dt, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		camSpeed = 10.0f;
	}
	else
		camSpeed = 2.0f;

	// Move camera by new pos after input
	freeCam->move(freeCamPos);
}

// Update positions
void startScene::Update(GLFWwindow* window)
{
	// Calculate dt
	lastFrame = thisFrame;
	thisFrame = glfwGetTime();
	dt = (float)(thisFrame - lastFrame);
	if (dt > 0.03)
		dt = 0.016;

	// Free cam stuff
	static double ratio_width = quarter_pi<float>() / 1600.0;
	static double ratio_height = (quarter_pi<float>() * (1600 / 900)) / static_cast<float>(1600);
	double current_x, current_y;
	glfwGetCursorPos(window, &current_x, &current_y);
	// Calc delta
	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;
	// Multiply deltas by ratios
	delta_x *= ratio_width;
	delta_y *= ratio_height * -1; // -1 to invert on y axis
								  // Rotate camera by delta
	freeCam->rotate(delta_x, delta_y);
	freeCam->update(0.001);
	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;
}


// Draw stuff
void startScene::Render(GLFWwindow* window)
{
	// Calculate mvp matrix
	mat4 mvp;
	// If camera type is free camera then
	mvp = freeCam->get_Projection() * freeCam->get_View();
	textureShader->Bind();
	arrowTexture->Bind(0);
	textureShader->Update(arrowTransform, mvp);
	arrowMesh->Draw();
	textureShader->Update(shaderTrans, (freeCam->get_Projection() * freeCam->get_View()));
	glDepthRange(0, 1.0);
	glfwSwapBuffers(window);
	glfwPollEvents();
}