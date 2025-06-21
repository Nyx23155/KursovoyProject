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
    // Вивід ігрового поля на екран
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

void Board::printBoardHidden() {
	// Вивід ігрового поля з прихованими кораблями
	cout << "  ";
	for (int i = 0; i < size; i++) {
		cout << i << " ";
	}
	cout << endl;
	for (int i = 0; i < size; i++) {
		cout << i << " ";
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 'S') {
				cout << '.' << " "; // Приховуємо кораблі
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
        return false; // Немає корабля в цій клітинці
    }
    return ship->isSunk();
}

bool Board::shoot(int x, int y) {
    // Перевірка меж поля
    if (x < 0 || x >= size || y < 0 || y >= size) {
        return false;
    }

    // Перевіряємо, чи вже стріляли в цю клітинку
    if (board[y][x] == 'X' || board[y][x] == '*' || board[y][x] == '#' || board[y][x] == 'O') {
        return false; // Вже стріляли сюди
    }

    Ship* ship = findShipByCoordinates(x, y);
    if (ship) {
        ship->hit();
        board[y][x] = 'X'; // Позначаємо влучання

        // ДОДАНО: Перевіряємо, чи потоплений корабель
        if (ship->isSunk()) {
            markSunkShip(ship);
            cout << "Ship sunk!" << endl;
        }

        return true; // Влучання
    }
    else {
        board[y][x] = '*'; // Позначаємо промах
        return false; // Промах
    }
}

void Board::markSunkShip(Ship* ship) {
    const vector<pair<int, int>>& coords = ship->getCoordinates();

    // Позначаємо всі клітинки корабля як потоплені
    for (const auto& coord : coords) {
        int x = coord.first;
        int y = coord.second;
        board[y][x] = '#'; // Потоплений корабель

        // Позначаємо клітинки навколо корабля як промахи
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < size && ny >= 0 && ny < size &&
                    board[ny][nx] == '.') { // Тільки порожні клітинки
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

    // Перевірка, що корабель поміщається на поле
    if ((horizontal && x + shipSize > size) || (!horizontal && y + shipSize > size)) {
        if (isPlayerShip) {
            cout << "Ship extends beyond the board. Choose another position." << endl;
        }
        return false;
    }

    // Перевірка, що клітинки не зайняті іншими кораблями і не торкаються інших кораблів
    for (int i = 0; i < shipSize; i++) {
        int newX = horizontal ? x + i : x;
        int newY = horizontal ? y : y + i;

        // Перевірка самої клітинки
        if (board[newY][newX] != '.') {
            if (isPlayerShip) {
                cout << "Cell (" << newX << "," << newY << ") is already occupied. Choose another position." << endl;
            }
            return false;
        }

        // Перевірка сусідніх клітинок (за правилами гри кораблі не можуть торкатися)
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

    // Якщо всі перевірки пройдені, розміщуємо корабель
    vector<pair<int, int>> coords;
    for (int i = 0; i < shipSize; i++) {
        int newX = horizontal ? x + i : x;
        int newY = horizontal ? y : y + i;
        coords.push_back({ newX, newY });      // (x, y)
        board[newY][newX] = 'S';               // board[y][x]
    }

    // Встановлюємо координати в об'єкті корабля
    ship->setCoordinates(coords);

    // Додаємо корабель у список кораблів поля
    this->ships.push_back(ship);

    return true;
}
