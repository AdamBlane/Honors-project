// Internals
#include "startScene.h"
#include "windowMgr.h"
#include "PerlinNoise.h"
#include "ppm.h"
#include <vector>
#include <time.h>
#include <list>
#include <queue>
#include "Node.h"

// Default constructor
startScene::startScene() { }
// Deconstructor
startScene::~startScene() { }


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
	plainMesh = new Mesh(Mesh::PLANE, vec3(0.0f, 0.0f, 0.0f), coordx, 0.1f, coordy);
	plainTexture = new Texture("..\\honors-project\\RESULT.png");

	startMesh = new Mesh(Mesh::CUBOID, vec3(coordx - 100.0f, 200.0f, 100.0f), 200.0f, 400.0f, 200.0f);
	startTexture = new Texture("..\\honors-project\\ballRed.jpg");

	endMesh = new Mesh(Mesh::CUBOID, vec3(100.0f, 200.0f, coordy - 100.0f), 200.0f, 400.0f, 200.0f);
	endTexture = new Texture("..\\honors-project\\ballBlue.jpg");

	for (int i =0; i < layout->Buildings.size(); i++)
	{
		Mesh* temp = new Mesh(Mesh::CUBOID, layout->Buildings.at(i)->get_Posistion(), layout->Buildings.at(i)->get_Size().x, layout->Buildings.at(i)->get_Size().y, layout->Buildings.at(i)->get_Size().z);
		Texture* tempTex = new Texture("..\\honors-project\\ballBlue.jpg");
		Buildings.push_back(temp);
		Buildings_Tex.push_back(tempTex);
		Buildings_Trans.push_back(temp_trans);
	}

	plainTransform.setPos(vec3(coordx/2, 0, coordy/2));
	freeCam = new free_camera();
	freeCam->set_Posistion(vec3(0, 10, -10));
	freeCam->rotate(-10.0, 0.0);
	freeCam->set_Target(vec3(0, 0, 0));
	freeCam->set_projection(quarter_pi<float>(), (float)1600 / (float)900, 0.414f, 30000.0f);
}
void startScene::CreateNoise() 
{
    int wi = coordx/10, he = coordy / 10;
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
			double n = 1;
		/*	if ((i <= j - 40 || i >= j + 40))
			{*/
				n = pn.noise(x/0.5 , y/0.5 , 0);
				n = n - floor(n * n);

			//}
			image.r[kk] = floor(255 * n);
			image.g[kk] = floor(255 * n);
			image.b[kk] = floor(255 * n);
			kk++;
		}
	}
	image.write("RESULT.ppm");
	
}
void startScene::CreateTerrain(const Texture &height_map, unsigned int width, unsigned int depth, float height_scale)
{
	//// Contains our position data
	std::vector<vec3> positions;
	//// Contains our normal data
	std::vector<vec3> normals;
	//// Contains our texture coordinate data
	std::vector<vec2> tex_coords;
	//// Contains our texture weights
	std::vector<vec4> tex_weights;
	//// Contains our index data
	std::vector<unsigned int> indices;
	//// Extract the texture data from the image
	// Extract the texture data from the image
	//glBindTexture(GL_TEXTURE_2D, height_map.get_id);
	auto data = new vec4[coordx * coordy];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (void*)data);

	// Determine ratio of height map to geometry
	float width_point = static_cast<float>(width) / static_cast<float>(coordx);
	float depth_point = static_cast<float>(depth) / static_cast<float>(coordy);

	// Point to work on
	vec3 point;

	//  Iterate through each point, calculate vertex and add to vector
	for (int x = 0; x < coordx; ++x)
	{
		// Calculate x position of point
		point.x = -(width / 2.0f) + (width_point * static_cast<float>(x));

		for (int z = 0; z < coordy; ++z)
		{
			// Calculate z position of point
			point.z = -(depth / 2.0f) + (depth_point * static_cast<float>(z));
			// Y position based on red component of height map data
			point.y = data[(z * coordx) + x].y * height_scale;
			// Add point to position data
			positions.push_back(point);
		}
	}
	// Part 1 - Add index data
	for (unsigned int x = 0; x < coordx - 1; ++x)
	{
		for (unsigned int y = 0; y < coordy - 1; ++y)
		{
			// Get four corners of patch
			unsigned int top_left = (y * coordx) + x;
			unsigned int top_right = (y * coordx) + x + 1;
			unsigned int bottom_left = ((y + 1) * coordx) + x;
			unsigned int bottom_right = ((y + 1) * coordx) + x + 1;
			// Push back indices for triangle 1
			indices.push_back(top_left);
			indices.push_back(bottom_right);
			indices.push_back(bottom_left);
			// Push back indices for triangle 2
			indices.push_back(top_left);
			indices.push_back(top_right);
			indices.push_back(bottom_right);
		}
	}
	// Resize the normals buffer
	normals.resize(positions.size());
	//  Calculate normals for the height map
	for (unsigned int i = 0; i < indices.size() / 3; ++i)
	{
		// Get indices for the triangle
		auto idx1 = indices[i * 3];
		auto idx2 = indices[i * 3 + 1];
		auto idx3 = indices[i * 3 + 2];
		// Calculate two sides of the triangle
		vec3 side1 = positions[idx1] - positions[idx3];
		vec3 side2 = positions[idx1] - positions[idx2];
		// Normal is cross product of these two sides
		vec3 normal = normalize(cross(side2, side1));
		// Add to normals in the normal buffer using the indices for the triangle
		normals[idx1] += normal;
		normals[idx2] += normal;
		normals[idx3] += normal;
	}

	//  Normalize all the normals
	for (auto &n : normals)
		n = normalize(n);
	//  Add texture coordinates for geometry
	for (unsigned int x = 0; x < coordx; ++x)
	{
		for (unsigned int z = 0; z < coordy; ++z)
		{
			tex_coords.push_back(vec2(width_point * x, depth_point * z)* 0.05f);
		}
	}

	// Part 4 - Calculate texture weights for each vertex
	for (unsigned int x = 0; x < coordx; x++)
	{
		for (unsigned int z = 0; z < coordy; ++z)
		{

			// Calculate tex weight
			vec4 tex_weight(
				clamp(1.0f - abs(data[(coordx * z) + x].y - 0.0f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(coordx * z) + x].y - 0.15f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(coordx * z) + x].y - 0.5f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(coordx * z) + x].y - 0.9f) / 0.25f, 0.0f, 1.0f));
			// Sum the components of the vector
			auto total = tex_weight.x + tex_weight.y + tex_weight.z + tex_weight.w;
			// Divide weight by sum
			tex_weight /= total;
			if (tex_weight.y < 0.01f /* && point.z >= 4.9f */)
			{
				int _x = x;
				int _z = z;
			}
			// Add tex weight to weights
			tex_weights.push_back(tex_weight);
		}
		std::cout << x << "done " << std::endl;
	}

	// Add necessary buffers to the geometry
	//geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	//geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
	//geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	//geom.add_buffer(tex_weights, BUFFER_INDEXES::TEXTURE_COORDS_1);
	//geom.add_index_buffer(indices);
	/////add in geomtry class
	//// Delete data
	delete[] data;
}
//A* Needs to comple
void startScene::CreatePath(GLFWwindow* window) 
{
	//std::priority_queue<node> untriedNodes[2]; // list of open (not-yet-tried) nodes
	//int nodeIndex = 0;
	//node* node1;
	//node* node2;
	//int **closedNodes, **openNodes, **nodeMap, **direction;
	//int x, y, xdx, //The Current x plus a direction
	//	zdz; //The Current z plus a direction

	//		 // reset the node maps
	//for (y = 0; y < coordy; y++)
	//{
	//	for (x = 0; x < coordy; x++)
	//	{
	//		closedNodes[x][y] = 0;
	//		openNodes[x][y] = 0;
	//	}
	//}

	//// create the start node and push into list of open nodes
	//node1 = new node(0, 0, 0, 0);
	//////change when spawns move
	//node1->UpdatePriority(coordx, coordy);
	//untriedNodes[nodeIndex].push(*node1);
	////openNodes[x][z] = x0->GetPriority(); // mark it on the open nodes map

	//// A* search
	//while (!untriedNodes[nodeIndex].empty())
	//{
	//	// get the current node w/ the highest priority
	//	// from the list of open nodes
	//	node1 = new node(untriedNodes[nodeIndex].top().getxPos(), untriedNodes[nodeIndex].top().getyPos(), untriedNodes[nodeIndex].top().getDistance(), untriedNodes[nodeIndex].top().getWeight());

	//	x = node1->getxPos();
	//	y = node1->getyPos();

	//	untriedNodes[nodeIndex].pop(); // remove the node from the open list
	//	openNodes[x][y] = 0;
	//	// mark it on the closed nodes map
	//	closedNodes[x][y] = 1;

	//	// quit searching when the goal state is reached
	//	if (x == coordx && y == coordy)
	//	{
	//		waypoints.clear();
	//		// generate the path from finish to start
	//		// by following the directions
	//		while (!(x == xStart && z == zStart))
	//		{
	//			int j = directions[x][z];
	//			x += dx[j];
	//			z += dz[j];
	//			waypoints.push_front(ivec2(x, z));
	//		}

	//		delete node1;
	//		// empty the leftover nodes
	//		while (!untriedNodes[nodeIndex].empty())
	//			untriedNodes[nodeIndex].pop();
	//		return true;
	//	}

	//	// generate moves (child nodes) in all possible directions
	//	for (int i = 0; i < dir; i++)
	//	{
	//		xdx = x + dx[i];
	//		zdz = z + dz[i];

	//		if (!(xdx<0 || xdx>xSize - 1 || zdz<0 || zdz>zSize - 1 || nodeMap[xdx][zdz] == 1 || closedNodes[xdx][zdz] == 1))
	//		{
	//			// generate a child node
	//			node2 = new Node(xdx, zdz, node1->GetDistance(),
	//				node1->GetPriority());
	//			node2->NextDistance(i);
	//			node2->UpdatePriority(xFinish, zFinish);

	//			// if it is not in the open list then add into that
	//			if (openNodes[xdx][zdz] == 0)
	//			{
	//				openNodes[xdx][zdz] = node2->GetPriority();
	//				untriedNodes[nodeIndex].push(*node2);
	//				// mark its parent node direction
	//				directions[xdx][zdz] = (i + dir / 2) % dir;
	//			}
	//			else if (openNodes[xdx][zdz] > node2->GetPriority())
	//			{
	//				// update the priority info
	//				openNodes[xdx][zdz] = node2->GetPriority();
	//				// update the parent direction info
	//				directions[xdx][zdz] = (i + dir / 2) % dir;

	//				// replace the node
	//				// by emptying one node to the other one
	//				// except the node to be replaced will be ignored
	//				// and the new node will be pushed in instead
	//				while (!(untriedNodes[nodeIndex].top().GetxPos() == xdx &&
	//					untriedNodes[nodeIndex].top().GetzPos() == zdz))
	//				{
	//					untriedNodes[1 - nodeIndex].push(untriedNodes[nodeIndex].top());
	//					untriedNodes[nodeIndex].pop();
	//				}
	//				untriedNodes[nodeIndex].pop(); // remove the wanted node

	//											   // empty the larger size node to the smaller one
	//				if (untriedNodes[nodeIndex].size() > untriedNodes[1 - nodeIndex].size()) nodeIndex = 1 - nodeIndex;
	//				while (!untriedNodes[nodeIndex].empty())
	//				{
	//					untriedNodes[1 - nodeIndex].push(untriedNodes[nodeIndex].top());
	//					untriedNodes[nodeIndex].pop();
	//				}
	//				nodeIndex = 1 - nodeIndex;
	//				untriedNodes[nodeIndex].push(*node2); // add the better node instead
	//			}
	//			else delete node2;
	//		}
	//	}
	//	delete node1;
	//}
}

void startScene::CreateScene(GLFWwindow* window)
{
	std::srand(time(NULL));
	std::default_random_engine gen;
	std::binomial_distribution<int> spped(11, 0.5);
	std::binomial_distribution<int> dist(20000, 0.5);
	coordx = dist(gen)+1000;
	coordy = dist(gen)+1000;
	camSpeed = 10 * spped(gen);
	//creates theme
	layout = new theme();
	int tye = rand() % 3 + 1;
	switch (tye)
	{
		case 1:
			layout->set_Type(Type::Desert);
			break;
		case 2:
			layout->set_Type(Type::City);
			break;
		case 3:
			layout->set_Type(Type::Seaside);
			break;
		case 4:
			layout->set_Type(Type::Village);
			break;
	}
	layout->create_Buildings(layout->get_Type(), coordx , coordy);
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
	std::cout << "Theme = (" << layout->get_Type() << ")" << std::endl;

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

	startTexture->Bind(0);
	textureShader->Update(startTransform, mvp);
	startMesh->Draw();

	endTexture->Bind(0);
	textureShader->Update(endTransform, mvp);
	endMesh->Draw();

	for (int i = 0; i < layout->Buildings.size(); i++)
	{
		Buildings_Tex.at(i)->Bind(0);
		////figue out
		textureShader->Update(Buildings_Trans.at(i), mvp);
		Buildings.at(i)->Draw();
	}

	textureShader->Update(shaderTrans, (freeCam->get_Projection() * freeCam->get_View()));
	glfwSwapBuffers(window);
	glfwPollEvents();
}