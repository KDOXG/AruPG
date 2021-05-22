#include "lib.h"

Map::Map()
{

}

void Map::setMapEffect(int x, int y, Abilities effect)
{
	area[x][y].effect.push_back(effect);
}

bool Map::removeMapEffect(int x, int y, std::string name)
{
	Abilities aux;
	int n = area[x][y].effect.size();
	bool flag = false;
	for (int i = 0; i < n; i++)
	{
		aux = area[x][y].effect.front();
		area[x][y].effect.pop_front();
		if (string_equal(aux.name, name.c_str()))
		{
			flag = true;
			continue;
		}
		area[x][y].effect.push_back(aux);
	}
	return flag;
}

void Map::removeMapEffect(int x, int y)
{
	area[x][y].effect.clear();
}

bool Map::checkMapEffect(int x, int y)
{
	return !area[x][y].effect.empty();
}

std::list<Abilities> Map::getMapEffect(int x, int y)
{
	return area[x][y].effect;
}

bool Map::findMapEffect(int x, int y, std::string name)
{
	Abilities aux;
	int n = area[x][y].effect.size();
	bool flag = false;
	for (int i = 0; i < n; i++)
	{
		aux = area[x][y].effect.front();
		area[x][y].effect.pop_front();
		if (string_equal(aux.name, name.c_str()))
			flag = true;
		area[x][y].effect.push_back(aux);
	}
	return flag;
}

void Map::playerMove(int old_x, int old_y, int new_x, int new_y, Player* player)
{
	Player* aux;
	int n = area[old_x][old_y].people.size();
	for (int i = 0; i < n; i++)
	{
		aux = area[old_x][old_y].people.front();
		area[old_x][old_y].people.pop_front();
		if (aux == player)
			continue;
		area[old_x][old_y].people.push_back(aux);
	}
	area[new_x][new_y].people.push_back(player);
}

bool Map::checkPlayer(int x, int y)
{
	return !area[x][y].people.empty();
}

bool Map::findPlayer(int x, int y, Player* player)
{
	Player* aux;
	int n = area[x][y].people.size();
	bool flag = false;
	for (int i = 0; i < n; i++)
	{
		aux = area[x][y].people.front();
		area[x][y].people.pop_front();
		if (aux == player)
			flag = true;
		area[x][y].people.push_back(aux);
	}
	return flag;
}