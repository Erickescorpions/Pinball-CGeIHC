#pragma once

#include <map>
#include <string>
#include <iostream>
#include "SpotLight.h"
#include "PointLight.h"

class Map_T
{
public:
	 void static toArray(std::map<std::string, PointLight>& map, PointLight* array, int array_size);
	 void static toArray(std::map<std::string, SpotLight>& map, SpotLight* array, int array_size);
private:
};
