#include <iostream>
#include <vector>
#include "Ship.h"

using namespace std;

Ship* ship;

int Ship::getSize() const
{
	return size;
}

int Ship::getHealth() const
{
	return health;
}

void Ship::hit() {
	if (health > 0) health--;
}
bool Ship::isSunk() const {
	return health == 0;
}


void Ship::setCoordinates(const vector<pair<int, int>>& coords)
{
	coordinates = coords;
}

const vector<pair<int, int>>& Ship::getCoordinates() const
{
	return coordinates;
}

Ship::Ship(int size)
{
	this->size = size;
	health = size;
}