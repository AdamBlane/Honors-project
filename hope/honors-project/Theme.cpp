#define _USE_MATH_DEFINES
#include "Theme.h"
#include <iostream>
#include <random>
enum spawn_rate { High, Med, Low };
theme::theme(){}
void theme::create_Buildings(Type type , int coordx, int coordz)
{

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
			number_buildings = rand() % 5 + 25;
			break;
		case Med:
			number_buildings = rand() % 15 + 15;
			break;
		case Low:
			number_buildings = rand() % 15 + 10;
			break;
	}
	std::cout << "Number of Buildings = "<< number_buildings << std::endl;
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
				scalex = rand()% 400 + 260;
				scaley = rand() % 400 + 260;
				scalez = rand() % 400 + 260;
				break;
			case(Barrier):
				scalex = rand() % 700 + 300;
				scaley = rand() % 1 + 0;
				if (scaley == 0) 
				{
					scaley = 200;
				}
				else 
				{
					scaley = 300;
				}
				scalez = rand() % 5 + 20;
				break;
			case(Skyscraper):
				scalex = rand() % 1000 + 1000;
				scaley = rand() % 4500 + 4500;
				scalez = rand() % 1000 + 1000;
				break;
			case(House):
				scalex = rand() % 1500 + 1000;
				scaley = rand() % 800 + 400;
				scalez = rand() % 1400 + 1000;
				break;
		}

		b->set_Size(glm::vec3(scalex, scaley, scalez));
		b->set_Posistion(glm::vec3(x, y, z));
		Buildings.push_back(b);
	}
	create_Health(type, coordx, coordz);
}
void theme::create_Health(Type type, int coordx, int coordz)
{
	int number = rand() % 6 + 1;
	int coin = rand() % 1 ;
	std::cout << "Number of Health Packs = " << number << std::endl;
	for (int i = 0; i < number; i++) 
	{
		health* h = new health();
		switch (type)
		{
			case Desert:
				h->set_Build(1);
				break;
			case City:
				h->set_Build(0);
				break;
			case Seaside:
				if (coin == 0)
				{
					h->set_Build(1);
				}
				else
				{
					h->set_Build(0);
				}
				break;
			case Village:
				if (coin == 0)
				{
					h->set_Build(1);
				}
				else
				{
					h->set_Build(0);
				}
				break;
		}
		int x, y, z;
		if (h->get_Build() == 0) 
		{
			h->set_Posistion(Buildings.at(rand() % Buildings.size())->get_Posistion());
		}
		else 
		{
			int x = rand() % coordx - 40 + 40, y = 0, z = rand() % coordz - 40 + 40;
			h->set_Posistion(glm::vec3(x, y, z));
		}
		if (i == 0) 
		{
			h->set_Size(Large);
		}
		else
		{
			if (coin == 0)
			{
				h->set_Size(Large);
			}
			else
			{
				h->set_Size(Small);
			}
		}
		Health_packs.push_back(h);
	}
}