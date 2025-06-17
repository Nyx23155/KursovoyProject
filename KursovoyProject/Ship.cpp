#include <iostream>
#include <vector>
#include "Ship.h"
#include "FiveCellShip.h"
#include "FourCellShip.h"
#include "ThreeCellShip.h"
#include "TwoCellShip.h"
#include "OneCellShip.h"

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

void Ship::hit()
{
	if (health  > 0)
	{
		health--;
	}
}


bool Ship::isSunk() const
{
	if (ship == nullptr)
	{
		return true;
	}
	else if (health == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
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