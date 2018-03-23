#define _USE_MATH_DEFINES
#include "Theme.h"

void theme::create_Buildings(Type type) 
{
	std::string spawn_rate;

	switch (type) 
	{
		case Desert: 
			spawn_rate = "Low";
			break;
		case City:
			spawn_rate = "High";
			break;
		case Seaside:
			spawn_rate = "Med";
			break;
		case Village:
			spawn_rate = "Med";
			break;
	}

}

