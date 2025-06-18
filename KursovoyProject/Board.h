#pragma once  
#include <iostream>  
#include "Ship.h"

class Board  
{  
public:  
   Board();  
   void printBoard();  
   bool shoot(int x, int y);  
   bool setShip(Ship* ship, int x, int y, bool horizontal, bool isPlayer);
   Ship* findShipByCoordinates(int x, int y);
   void markSunkShip(Ship* ship);
   bool allShipsSunk() const;
   int getSize() const { return size; }
   bool isSunk(int x, int y);

private:  
   static const int size = 10;  
   char board[size][size];
   vector <Ship*> ships;
};
