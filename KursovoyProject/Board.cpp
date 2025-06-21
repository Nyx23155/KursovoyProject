#include <iostream>
#include <vector>
#include "Ship.h"
#include "Board.h"
using namespace std;

Board::Board()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = '.';
        }
    }
}

void Board::printBoard() {
    // Вывод игровой доски на экран
    cout << "  ";
    for (int i = 0; i < size; i++) {
        cout << i << " ";
    }
    cout << endl;

    for (int i = 0; i < size; i++) {
        cout << i << " ";
        for (int j = 0; j < size; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

Ship* Board::findShipByCoordinates(int x, int y) {
    if (x < 0 || x >= size || y < 0 || y >= size) {
        cout << "Coordinates (" << x << ", " << y << ") are out of bounds." << endl;
        return nullptr;
    }

    for (Ship* ship : ships) {
        for (const auto& coord : ship->getCoordinates()) {
            if (coord.first == x && coord.second == y) { // (x, y)
                return ship;
            }
        }
    }

    cout << "No ship found at coordinates (" << x << ", " << y << ")." << endl;
    return nullptr;
}

bool Board::isSunk(int x, int y) {
    Ship* ship = findShipByCoordinates(x, y);
    if (!ship) {
        return false; // Нет корабля в этой клетке
    }
    return ship->isSunk();
}

bool Board::shoot(int x, int y) {
    // Проверка границ поля
    if (x < 0 || x >= size || y < 0 || y >= size) {
        return false; // Мимо (или можно выбросить исключение)
    }

    // Проверяем, уже ли стреляли в эту клетку
    if (board[x][y] == 'X' || board[x][y] == '*') {
        // Уже стреляли сюда, считаем как промах
        return false;
    }

    Ship* ship = findShipByCoordinates(x, y);
    if (ship) {
        ship->hit();
        board[x][y] = 'X'; // Обозначаем попадание
        return true; // Попадание
    }
    else {
        board[x][y] = '*'; // Обозначаем промах
        return false; // Мимо
    }
}

void Board::markSunkShip(Ship* ship) {
    const vector<pair<int, int>>& coords = ship->getCoordinates();

    // Отмечаем все клетки корабля как потопленные
    for (const auto& coord : coords) {
        int x = coord.first;
        int y = coord.second;
        board[y][x] = '#'; // Или другой символ для потопленного корабля

        // Отмечаем клетки вокруг корабля как промахи
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < size && ny >= 0 && ny < size &&
                    (board[ny][nx] == '.' || board[ny][nx] == ' ')) {
                    board[ny][nx] = 'O';
                }
            }
        }
    }
}

   bool Board::allShipsSunk() const {
       for (const Ship* ship : ships) {
           if (!ship->isSunk()) return false;
       }
       return true;
   }
   
   bool Board::setShip(Ship* ship, int x, int y, bool horizontal, bool isPlayer) {
       int shipSize = ship->getSize();
       bool isPlayerShip = isPlayer;

       // Проверка, что корабль помещается на доску
       if ((horizontal && x + shipSize > size) || (!horizontal && y + shipSize > size)) {
           if (isPlayerShip) {
               cout << "Ship extends beyond the board. Choose another position." << endl;
           }
           return false;
       }

       // Проверка, что клетки не заняты другими кораблями и не касаются других кораблей
       for (int i = 0; i < shipSize; i++) {
           int newX = horizontal ? x + i : x;
           int newY = horizontal ? y : y + i;

           // Проверка самой клетки
           if (board[newY][newX] != '.') {
               if (isPlayerShip) {
                   cout << "Cell (" << newX << "," << newY << ") is already occupied. Choose another position." << endl;
               }
               return false;
           }

           // Проверка соседних клеток (по правилам игры корабли не могут касаться)
           for (int dx = -1; dx <= 1; dx++) {
               for (int dy = -1; dy <= 1; dy++) {
                   int checkX = newX + dx;
                   int checkY = newY + dy;
                   if (checkX >= 0 && checkX < size && checkY >= 0 && checkY < size) {
                       if (board[checkY][checkX] == 'S') {
                           if (isPlayerShip) {
                               cout << "Ships cannot touch each other. Choose another position." << endl;
                           }
                           return false;
                       }
                   }
               }
           }
       }

       // Если все проверки пройдены, размещаем корабль
       vector<pair<int, int>> coords;
       for (int i = 0; i < shipSize; i++) {
           int newX = horizontal ? x + i : x;
           int newY = horizontal ? y : y + i;
           coords.push_back({ newX, newY });      // (x, y)
           board[newY][newX] = 'S';               // board[y][x]
       }

       // Устанавливаем координаты в объекте корабля
       ship->setCoordinates(coords);

       // Добавляем корабль в список кораблей доски
       this->ships.push_back(ship);

       return true;
   }

