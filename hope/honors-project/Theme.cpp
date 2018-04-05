#define _USE_MATH_DEFINES
#include "Theme.h"
#include <iostream>
#include <random>

theme::theme(){}
void theme::create_Buildings(Type type , int coordx, int coordz)
{
	enum spawn_rate {High, Med, Low};
	spawn_rate rate;
	int number_buildings;
	switch (type)
	{
		case Desert:
			rate = Low;
			break;
		case City:
			rate = High;
			break;
		case Seaside:
			rate = Med;
			break;
		case Village:
			rate = Med;
			break;
	}
	switch (rate) 
	{
		case High:
			number_buildings = rand() % 25 + 25;
			break;
		case Med:
			number_buildings = rand() % 15 + 15;
			break;
		case Low:
			number_buildings = rand() % 5 + 5;
			break;
	}
	std::cout << number_buildings << std::endl;
	for (int i = 0; i < number_buildings; i++) 
	{
		building* b = new building();
		int x = rand()% coordx-40 + 40 , y = 0, z= rand() % coordz - 40 + 40;
		int r = rand()% 99 + 1;
		int scalex, scaley, scalez;


		if (type == City && r <= 80) 
		{
			b->set_Type(Skyscraper);
		}
		else if ((type == Village && r <= 70) || (type == Seaside && r <= 75) || (type == Desert && r <= 20))
		{
			b->set_Type(House);
		}
		else if ((type == Village && r >= 95) || (type == Seaside && r >= 80) || (type == Desert && r >= 35) || (type == City && r >= 95))
		{
			b->set_Type(Barrier);
		}
		else 
		{
			b->set_Type(Car);
		}

		switch (b->get_Type())
		{
			case(Car):
				scalex = rand()% 200 + 130;
				scaley = rand() % 200 + 130;
				scalez = rand() % 200 + 130;
				break;
			case(Barrier):
				scalex = rand() % 500 + 100;
				scaley = rand() % 1 + 0;
				if (scaley == 0) 
				{
					scaley = 100;
				}
				else 
				{
					scaley = 150;
				}
				scalez = rand() % 10 + 5;
				break;
			case(Skyscraper):
				scalex = rand() % 500 + 500;
				scaley = rand() % 4000 + 4000;
				scalez = rand() % 500 + 500;
				break;
			case(House):
				scalex = rand() % 700 + 500;
				scaley = rand() % 400 + 200;
				scalez = rand() % 700 + 500;
				break;
		}

		b->set_Size(glm::vec3(scalex, scaley, scalez));
		b->set_Posistion(glm::vec3(x, y, z));
		Buildings.push_back(b);
	}
}
