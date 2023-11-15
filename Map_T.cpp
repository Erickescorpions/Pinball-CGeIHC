#include "Map_T.h"

void Map_T::toArray(std::map<std::string, PointLight>& map, PointLight* array, int array_size)
{
    //std::cout << "mapa de luces pointLight, tamanio: " << map.size();
    std::map<std::string, PointLight>::iterator it = map.begin();
    int index = 0;

    while (it != map.end() && index < array_size)
    {
        array[index] = it->second;

        ++index;
        ++it;
    }
}

void Map_T::toArray(std::map<std::string, SpotLight>& map, SpotLight* array, int array_size)
{
    //std::cout << "mapa de luces spotLight, tamanio: " << map.size();
    std::map<std::string, SpotLight>::iterator it = map.begin();
    int index = 0;

    while (it != map.end() && index < array_size)
    {
        array[index] = it->second;

        ++index;
        ++it;
    }
}