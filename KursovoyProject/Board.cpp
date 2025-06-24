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

char Board::getColumnLetter(int col) const {
    return 'A' + col;
}

void Board::printBoard() {
    // Вывод игрового поля на экран
    cout << "   ";
    for (int i = 0; i < size; i++) {
        cout << getColumnLetter(i) << " ";
    }
    cout << endl;

    for (int i = 0; i < size; i++) {
        if (i + 1 < 10) {
            cout << " " << (i + 1) << " ";
        }
        else {
            cout << (i + 1) << " ";
        }
        for (int j = 0; j < size; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

void Board::printBoardHidden() {
    // Вывод игрового поля с скрытыми кораблями
    cout << "   ";
    for (int i = 0; i < size; i++) {
        cout << getColumnLetter(i) << " ";
    }
    cout << endl;

    for (int i = 0; i < size; i++) {
        if (i + 1 < 10) {
            cout << " " << (i + 1) << " ";
        }
        else {
            cout << (i + 1) << " ";
        }
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 'S') {
                cout << '.' << " "; // Скрываем корабли
            }
            else {
                cout << board[i][j] << " ";
            }
        }
        cout << endl;
    }
}

Ship* Board::findShipByCoordinates(int x, int y) {
    if (x < 0 || x >= size || y < 0 || y >= size) {
        cout << "Coordinates (" << getColumnLetter(x) << ", " << (y + 1) << ") are out of bounds." << endl;
        return nullptr;
    }

    for (Ship* ship : ships) {
        for (const auto& coord : ship->getCoordinates()) {
            if (coord.first == x && coord.second == y) { // (x, y)
                return ship;
            }
        }
    }

    cout << "No ship found at coordinates (" << getColumnLetter(x) << ", " << (y + 1) << ")." << endl;
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
        return false;
    }

    // Проверяем, стреляли ли уже в эту клетку
    if (board[y][x] == 'X' || board[y][x] == '*' || board[y][x] == '#' || board[y][x] == 'O') {
        return false; // Уже стреляли сюда
    }

    Ship* ship = findShipByCoordinates(x, y);
    if (ship) {
        ship->hit();
        board[y][x] = 'X'; // Отмечаем попадание

        // ДОБАВЛЕНО: Проверяем, потоплен ли корабль
        if (ship->isSunk()) {
            markSunkShip(ship);
            cout << "Ship sunk!" << endl;
        }

        return true; // Попадание
    }
    else {
        board[y][x] = '*'; // Отмечаем промах
        return false; // Промах
    }
}

void Board::markSunkShip(Ship* ship) {
    const vector<pair<int, int>>& coords = ship->getCoordinates();

    // Отмечаем все клетки корабля как потопленные
    for (const auto& coord : coords) {
        int x = coord.first;
        int y = coord.second;
        board[y][x] = '#'; // Потопленный корабль

        // Отмечаем клетки вокруг корабля как промахи
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < size && ny >= 0 && ny < size &&
                    board[ny][nx] == '.') { // Только пустые клетки
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

    // Проверка, что корабль помещается на поле
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
                cout << "Cell (" << getColumnLetter(newX) << "," << (newY + 1) << ") is already occupied. Choose another position." << endl;
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

    // Добавляем корабль в список кораблей поля
    this->ships.push_back(ship);

    return true;
}