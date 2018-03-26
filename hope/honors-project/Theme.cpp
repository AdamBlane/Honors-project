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
		if (type == City && r <= 80) 
		{
			b->set_Type(type::Skyscraper);
		}
		else if ((type == Village && r <= 70) || (type == Seaside && r <= 75) || (type == Desert && r <= 20))
		{
			b->set_Type(type::House);
		}
		else if ((type == Village && r >= 95) || (type == Seaside && r >= 80) || (type == Desert && r >= 35) || (type == City && r >= 95))
		{
			b->set_Type(type::Barrier);
		}
		else 
		{
			b->set_Type(type::Car);
		}
		Buildings.push_back(b);
	}
}
