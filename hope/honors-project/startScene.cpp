// Internals
#include "startScene.h"
#include "windowMgr.h"
#include "PerlinNoise.h"
#include "ppm.h"
#include <vector>
#include <time.h>
#include <list>
#include <queue>


// Default constructor
startScene::startScene() { }
// Deconstructor
startScene::~startScene() { }


std::vector <node*> nodes;
std::vector <node*> wanted_nodes;

// Setup scene; seed is an optional param passed in by loadGameScene
void startScene::Init(GLFWwindow* window)
{
	// Set GL properties 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, cursor_x, cursor_y);
	//for (int i = 0; i < 100; i++) 
	//{
	CreateScene(window);
	CreatePath(window);
	Algortithm(window);
	CreateNoise();
	//}

	textureShader = new Shader("..\\honors-project\\textureShader");

	plainTexture = new Texture("..\\honors-project\\RESULT.png");
	CreateTerrain(window, terrain, plainTexture, 20, 20, 2.0f);

	startMesh = new Mesh(Mesh::CUBOID, vec3(coordx - 100.0f, 200.0f, 100.0f), 200.0f, 400.0f, 200.0f);
	startTexture = new Texture("..\\honors-project\\ballRed.jpg");

	endMesh = new Mesh(Mesh::CUBOID, vec3(100.0f, 200.0f, coordy - 100.0f), 200.0f, 400.0f, 200.0f);
	endTexture = new Texture("..\\honors-project\\ballBlue.jpg");

	for (int i = 0; i < nodes.size(); i++) 
	{
		Mesh* temp = new Mesh(Mesh::BOX, vec3(nodes.at(i)->getxPos(), 0.0f, nodes.at(i)->getyPos()), 200.0f, 200.0f, 200.0f);
		Texture* tempTex = new Texture("..\\honors-project\\redBlue.jpg");
		Node_Mesh.push_back(temp);
		Node_Tex.push_back(tempTex);
		Node_Trans.push_back(temp_trans);
	}
	for (int i =0; i < layout->Buildings.size(); i++)
	{
		Mesh* temp = new Mesh(Mesh::CUBOID, vec3(layout->Buildings.at(i)->get_Posistion().x, layout->Buildings.at(i)->get_Posistion().y + (layout->Buildings.at(i)->get_Size().y/2), layout->Buildings.at(i)->get_Posistion().z), layout->Buildings.at(i)->get_Size().x, layout->Buildings.at(i)->get_Size().y, layout->Buildings.at(i)->get_Size().z);
		Texture* tempTex = new Texture("..\\honors-project\\ballBlue.jpg");
		Buildings.push_back(temp);
		Buildings_Tex.push_back(tempTex);
		Buildings_Trans.push_back(temp_trans);
	}
	for (int i = 0; i < layout->Health_packs.size(); i++)
	{
		float a = 40.0f;
		if (layout->Health_packs.at(i)->get_Size() == size::Large) {
			a = 80.0f;
		}
		Mesh* temp = new Mesh(Mesh::CUBOID, vec3(layout->Health_packs.at(i)->get_Posistion().x, 10.0f, layout->Health_packs.at(i)->get_Posistion().z), a, a,a);
		Texture* tempTex = new Texture("..\\honors-project\\ballRed.jpg");
		HelathPacks.push_back(temp);
		HelathPacks_Tex.push_back(tempTex);
		HelathPacks_Trans.push_back(temp_trans);
	}

	int a =0, b=0 ,c=0 ,d=0;
	switch (sides) 
	{
		case 0:
			break;
		case 1:
			a = rand() % 3 + 1;
			break;
		case 2:
			a = rand() % 3 + 1;
			b = rand() % 3 + 1;
			while (a == b)
			{
				b = rand() % 3 + 1;
			}
			break;
		case 3:
			a = rand() % 3 + 1;
			b = rand() % 3 + 1;
			c = rand() % 3 + 1;
			while (a == b || b ==c)
			{
				b = rand() % 3 + 1;
			}
			while (a == c)
			{
				c = rand() % 3 + 1;
			}
			break;
		case 4:
			a = 1;
			b = 2;
			c = 3;
			d = 4;

	}
	if (a == 1 || b == 1 || c == 1) 
	{
		Mesh* temp = new Mesh(Mesh::CUBOID, vec3(0.0f, 500.0f, coordy / 2), 0.0f, 1000.0f, coordy);
		Texture* tempTex = new Texture("..\\honors-project\\ballRed.jpg");
		Sides.push_back(temp);
		Sides_Tex.push_back(tempTex);
		Sides_Trans.push_back(temp_trans);
	}
	if (a == 2 || b == 2 || c == 2)
	{
		Mesh* temp = new Mesh(Mesh::CUBOID, vec3(coordx / 2, 500.0f, 0.0f),coordx, 1000.0f, 0.0f);
		Texture* tempTex = new Texture("..\\honors-project\\ballRed.jpg");
		Sides.push_back(temp);
		Sides_Tex.push_back(tempTex);
		Sides_Trans.push_back(temp_trans);
	}
	if (a == 3 || b == 3 || c == 3)
	{
		Mesh* temp = new Mesh(Mesh::CUBOID, vec3(0.0f, 500.0f, coordy / 2), 0.0f, 1000.0f, coordy);
		Texture* tempTex = new Texture("..\\honors-project\\ballRed.jpg");
		Sides.push_back(temp);
		Sides_Tex.push_back(tempTex);
		Sides_Trans.push_back(temp_trans);
	}
	if (a == 4 || b == 4 || c == 4 || d ==4)
	{
		Mesh* temp = new Mesh(Mesh::CUBOID, vec3(coordx / 2, 500.0f, 0.0f), coordx, 1000.0f, 0.0f);
		Texture* tempTex = new Texture("..\\honors-project\\ballRed.jpg");
		Sides.push_back(temp);
		Sides_Tex.push_back(tempTex);
		Sides_Trans.push_back(temp_trans);
	}
	plainTransform.setPos(vec3(coordx/2, 0, coordy/2));
	freeCam = new free_camera();
	freeCam->set_Posistion(vec3(0, 10, -10));
	freeCam->rotate(-10.0, 0.0);
	freeCam->set_Target(vec3(0, 0, 0));
	freeCam->set_projection(quarter_pi<float>(), (float)1600 / (float)900, 0.414f, 30000.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
			int check =  1;
			for (int k = 0 ; k < nodes.size() ; k++)
			{
				if (j >= (nodes.at(k)->getxPos()/10) -10 && i >= (nodes.at(k)->getyPos()/10) - 10)
				{
					if (j <= (nodes.at(k)->getxPos() / 10) + 10 && i <= (nodes.at(k)->getyPos() / 10)+ 10)
					{
						check = 0;
					}
				}
			}
			for (int k = 0; k < layout->Buildings.size(); k++)
			{
				if (j >= (layout->Buildings.at(k)->get_Posistion().x/10) - layout->Buildings.at(k)->get_Size().x / 20 && i >= (layout->Buildings.at(k)->get_Posistion().z/ 10) - layout->Buildings.at(k)->get_Size().z / 20)
				{
					if (j <= (layout->Buildings.at(k)->get_Posistion().x / 10) + layout->Buildings.at(k)->get_Size().x / 20 && i <= (layout->Buildings.at(k)->get_Posistion().z / 10) + layout->Buildings.at(k)->get_Size().z / 20)
					{
						check = 2;
					}
				}
			}
			for (int k = 0; k < layout->Health_packs.size(); k++)
			{
				if (j >= (layout->Health_packs.at(k)->get_Posistion().x / 10) - 10  && i >= (layout->Buildings.at(k)->get_Posistion().z / 10) -10)
				{
					if (j <= (layout->Health_packs.at(k)->get_Posistion().x / 10) + 10 && i <= (layout->Buildings.at(k)->get_Posistion().z / 10) + 10)
					{
						check = 3;
					}
				}
			}
			for (int k = 0; k < layout->Health_packs.size(); k++)
			{
				if (j >= (layout->Health_packs.at(k)->get_Posistion().x / 10) - 10 && i >= (layout->Buildings.at(k)->get_Posistion().z / 10) - 10)
				{
					if (j <= (layout->Health_packs.at(k)->get_Posistion().x / 10) + 10 && i <= (layout->Buildings.at(k)->get_Posistion().z / 10) + 10)
					{
						check = 3;
					}
				}
			}
			for (int k = 0; k < route.size(); k++)
			{
				if (j >= (route.at(k).x / 10) - 10 && i >= (route.at(k).y / 10) - 10)
				{
					if (j <= (route.at(k).x / 10) + 10 && i <= (route.at(k).y / 10) + 10)
					{
						check = 0;
					}
				}
			}
			if (check == 1) 
			{
				n = pn.noise(x / 0.5, y / 0.5, 0);
				n = n - floor(n * n);
			}
			if (check == 0)
			{
				n = 1;
			}
			if (check == 2)
			{
				n = 0;
			}
			if (check == 3)
			{
				n = 0.5;
			}
			image.r[kk] = floor(255 * n);
			image.g[kk] = floor(255 * n);
			image.b[kk] = floor(255 * n);
			kk++;
		}
	}
	image.write("RESULT.ppm");
	
}

void startScene::CreateTerrain(GLFWwindow* window , geometry &geom, const Texture *height_map, unsigned int width, unsigned int depth, float height_scale)
{
	////// Contains our position data
	//std::vector<vec3> positions;
	////// Contains our normal data
	//std::vector<vec3> normals;
	////// Contains our texture coordinate data
	//std::vector<vec2> tex_coords;
	////// Contains our texture weights
	//std::vector<vec4> tex_weights;
	////// Contains our index data
	//std::vector<unsigned int> indices;
	////// Extract the texture data from the image
	//// Extract the texture data from the image
	//glBindTexture(GL_TEXTURE_2D, height_map->m_texture);
	//auto data = new vec4[coordx/10 * coordy/10];
	//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (void*)data);

	//// Determine ratio of height map to geometry
	//float width_point = static_cast<float>(width) / static_cast<float>(coordx/10);
	//float depth_point = static_cast<float>(depth) / static_cast<float>(coordy/10);

	//// Point to work on
	//vec3 point;

	////  Iterate through each point, calculate vertex and add to vector
	//for (int x = 0; x < coordy/50; ++x)
	//{
	//	// Calculate x position of point
	//	point.x = -(width / 2.0f) + (width_point * static_cast<float>(x));

	//	for (int z = 0; z < coordy/50; ++z)
	//	{
	//		// Calculate z position of point
	//		point.z = -(depth / 2.0f) + (depth_point * static_cast<float>(z));
	//		// Y position based on red component of height map data
	//		point.y = data[(z * coordx/10) + x].y * height_scale;
	//		// Add point to position data
	//		positions.push_back(point);

	//	}
	//}
	//// Part 1 - Add index data
	//for (unsigned int x = 0; x < coordx/50 - 1; ++x)
	//{
	//	for (unsigned int y = 0; y < coordy/50 - 1; ++y)
	//	{
	//		// Get four corners of patch
	//		unsigned int top_left = (y * coordx/50) + x;
	//		unsigned int top_right = (y * coordx/50) + x + 1;
	//		unsigned int bottom_left = ((y + 1) * coordx/50) + x;
	//		unsigned int bottom_right = ((y + 1) * coordx/50) + x + 1;
	//		// Push back indices for triangle 1
	//		indices.push_back(top_left);
	//		indices.push_back(bottom_right);
	//		indices.push_back(bottom_left);
	//		// Push back indices for triangle 2
	//		indices.push_back(top_left);
	//		indices.push_back(top_right);
	//		indices.push_back(bottom_right);
	//	}
	//}
	//// Resize the normals buffer
	//normals.resize(positions.size());
	////  Calculate normals for the height map
	//for (unsigned int i = 0; i < indices.size() / 3; ++i)
	//{
	//	// Get indices for the triangle
	//	auto idx1 = indices[i * 3];
	//	auto idx2 = indices[i * 3 + 1];
	//	auto idx3 = indices[i * 3 + 2];
	//	// Calculate two sides of the triangle
	//	vec3 side1 = positions[idx1] - positions[idx3];
	//	vec3 side2 = positions[idx1] - positions[idx2];
	//	// Normal is cross product of these two sides
	//	vec3 normal = normalize(cross(side2, side1));
	//	// Add to normals in the normal buffer using the indices for the triangle
	//	normals[idx1] += normal;
	//	normals[idx2] += normal;
	//	normals[idx3] += normal;
	//}

	////  Normalize all the normals
	//for (auto &n : normals)
	//	n = normalize(n);
	////  Add texture coordinates for geometry
	//for (unsigned int x = 0; x < coordx/50; ++x)
	//{
	//	for (unsigned int z = 0; z < coordy/50; ++z)
	//	{
	//		tex_coords.push_back(vec2(width_point * x, depth_point * z)* 0.05f);
	//	}
	//}

	//// Part 4 - Calculate texture weights for each vertex
	//for (unsigned int x = 0; x < coordx/50; x++)
	//{
	//	for (unsigned int z = 0; z < coordy/50; ++z)
	//	{

	//		// Calculate tex weight
	//		vec4 tex_weight(
	//			clamp(1.0f - abs(data[(coordx/50 * z) + x].y - 0.0f) / 0.25f, 0.0f, 1.0f),
	//			clamp(1.0f - abs(data[(coordx/50 * z) + x].y - 0.15f) / 0.25f, 0.0f, 1.0f),
	//			clamp(1.0f - abs(data[(coordx/50 * z) + x].y - 0.5f) / 0.25f, 0.0f, 1.0f),
	//			clamp(1.0f - abs(data[(coordx/50 * z) + x].y - 0.9f) / 0.25f, 0.0f, 1.0f));
	//		// Sum the components of the vector
	//		auto total = tex_weight.x + tex_weight.y + tex_weight.z + tex_weight.w;
	//		// Divide weight by sum
	//		tex_weight /= total;
	//		if (tex_weight.y < 0.01f /* && point.z >= 4.9f */)
	//		{
	//			int _x = x;
	//			int _z = z;
	//		}
	//		// Add tex weight to weights
	//		tex_weights.push_back(tex_weight);
	//	}
	//}
	//plainMesh = new Mesh(Mesh::Terrain, vec3(coordx - 100.0f, 200.0f, 100.0f), 200.0f, positions, tex_coords, 400.0f, 200.0f);
	///////add in geomtry class
	////// Delete data
	//delete[] data;
}

void startScene::CreatePath(GLFWwindow* window) 
{
	nodeMap = new int*[coordx];
	for (int i = 0; i < coordx; i++)
		nodeMap[i] = new int[coordy];

	openNodes = new int*[coordx];
	for (int i = 0; i < coordx; i++)
		openNodes[i] = new int[coordy];

	closedNodes = new int*[coordy];
	for (int i = 0; i < coordx; i++)
		closedNodes[i] = new int[coordy];

	direction = new int*[coordx];
	for (int i = 0; i < coordx; i++)
		direction[i] = new int[coordy];

	int dx[9] = { 100, 100, 0, -100, -100, -100, 0, 100, 0 }; //The x coordinate of the posible directions
	int dy[9] = { 0, 100, 100, 100, 0, -100, -100, -100, 0 };

	while(nodes.size() < 6) 
	{
		int x = rand() % coordx;
		int z = rand() % coordy;
		bool check = false;
		for(int i = 0; i < layout->Buildings.size(); i++)
		{
			if (x >= layout->Buildings.at(i)->get_Posistion().x && x <= layout->Buildings.at(i)->get_Posistion().x + layout->Buildings.at(i)->get_Size().x)
			{
				if (z >= layout->Buildings.at(i)->get_Posistion().z && z <= layout->Buildings.at(i)->get_Posistion().z + layout->Buildings.at(i)->get_Size().z) 
				{
					check = true;
				}
			}
		}
		int weight = (x-z) * (x-z);
		int dist;
		if (!check == true) 
		{
			node* a = new node(x,z,3,weight);
			nodes.push_back(a);
			untriedNodes[nodeIndex].push(*a);
		}

	}
	//node* node1;
	//node* node2;
	//int x, y,
	//	xdx, //The Current x plus a direction
	//	ydy; //The Current y plus a direction

	//// create the start node and push into list of open nodes
	//node1 = new node(0, 0, 0, 0);
	//node1->UpdatePriority(coordx, coordy);
	//untriedNodes[nodeIndex].push(*node1);

	//// reset the node maps
	//for (y = 0; y < coordy; ++y)
	//{
	//	for (x = 0; x < coordx; ++x)
	//	{
	//		closedNodes[x][y] = 0;
	//		openNodes[x][y] = 0;
	//	}
	//}
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
	//	if (x >= coordx - 101 && y >= coordy- 101)
	//	{
	//		// generate the path from finish to start
	//		// by following the directions
	//		while (!(x >= coordx - 101 && y >= coordy -101))
	//		{
	//			int j = direction[x][y];
	//			x += dx[j];
	//			y += dy[j];
	//			route.push_front(ivec2(x, y));;
	//		}

	//		delete node1;
	//		// empty the leftover nodes
	//		while (!untriedNodes[nodeIndex].empty())
	//			untriedNodes[nodeIndex].pop();
	//	}

	//	// generate moves (child nodes) in all possible directions
	//	for (int i = 0; i < directions; i++)
	//	{
	//		xdx = x + dx[i];
	//		ydy = y + dy[i];

	//		if (!(xdx<0 || xdx>coordx - 1 || ydy<0 || ydy>coordy - 1 || nodeMap[xdx][ydy] == 1 || closedNodes[xdx][ydy] == 1))
	//		{
	//			// generate a child node
	//			node2 = new node(xdx, ydy, node1->getDistance(),node1->getWeight());
	//			node2->NextDistance(i);
	//			node2->UpdatePriority(coordx, coordy);

	//			// if it is not in the open list then add into that
	//			if (openNodes[xdx][ydy] == 0)
	//			{
	//				openNodes[xdx][ydy] = node2->getWeight();
	//				untriedNodes[nodeIndex].push(*node2);
	//				// mark its parent node direction
	//				direction[xdx][ydy] = (i + directions / 2) % directions;
	//			}
	//			else if (openNodes[xdx][ydy] > node2->getWeight())
	//			{
	//				// update the priority info
	//				openNodes[xdx][ydy] = node2->getWeight();
	//				// update the parent direction info
	//				direction[xdx][ydy] = (i + directions / 2) % directions;

	//				// replace the node
	//				// by emptying one node to the other one
	//				// except the node to be replaced will be ignored
	//				// and the new node will be pushed in instead
	//				while (!(untriedNodes[nodeIndex].top().getxPos() == xdx &&
	//					untriedNodes[nodeIndex].top().getyPos() == ydy))
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
	gen.seed(rand() % 1000);
	std::binomial_distribution<int> spped(100, 0.5);
	std::binomial_distribution<int> dist(20000, 0.5);
	coordx = dist(gen)+1000;
	coordy = dist(gen)+1000;
	camSpeed = spped(gen);
	//creates theme
	layout = new theme();
	int tye = rand() % 3 + 1;
	switch (tye)
	{
		case 1:
			layout->set_Type(Type::Desert);
			sides = 0;
			break;
		case 2:
			layout->set_Type(Type::City);
			sides = rand() % 2 + 2;
			break;
		case 3:
			layout->set_Type(Type::Seaside);
			sides = rand() % 2;
			break;
		case 4:
			layout->set_Type(Type::Village);
			sides = rand() % 2 + 1;
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
	std::cout << "Number of Sides = (" << sides << ")" << std::endl;

}
void startScene::Loop(GLFWwindow* window)
{
	glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
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
	//plainTexture->Bind(0);
	//textureShader->Update(plainTransform, mvp);
	//plainMesh->Draw();

	startTexture->Bind(0);
	textureShader->Update(startTransform, mvp);
	startMesh->Draw();

	endTexture->Bind(0);
	textureShader->Update(endTransform, mvp);
	endMesh->Draw();
	for (int i = 0; i < layout->Health_packs.size(); i++)
	{
		HelathPacks_Tex.at(i)->Bind(0);
		textureShader->Update(HelathPacks_Trans.at(i), mvp);
		HelathPacks.at(i)->Draw();
	}
	for (int i = 0; i < layout->Buildings.size(); i++)
	{
		Buildings_Tex.at(i)->Bind(0);
		textureShader->Update(Buildings_Trans.at(i), mvp);
		Buildings.at(i)->Draw();
	}
	for (int i = 0; i < sides; i++)
	{
		Sides_Tex.at(i)->Bind(0);
		textureShader->Update(Sides_Trans.at(i), mvp);
		Sides.at(i)->Draw();
	}
	for (int i = 0; i < nodes.size(); i++)
	{
		Node_Tex.at(i)->Bind(0);
		textureShader->Update(Node_Trans.at(i), mvp);
		Node_Mesh.at(i)->Draw();
	}
	textureShader->Update(shaderTrans, (freeCam->get_Projection() * freeCam->get_View()));
	glfwSwapBuffers(window);
	glfwPollEvents();
}