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
	// ����� ������� ����� �� �����
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

vector<Ship*> ships;

Ship* Board::findShipByCoordinates(int x, int y) {
    if (x < 0 || x >= size || y < 0 || y >= size) {
        cout << "Coordinates (" << x << ", " << y << ") are out of bounds." << endl;
        return nullptr;
    }

    for (Ship* ship : ships) {
        const vector<pair<int, int>>& coords = ship->getCoordinates();
        for (const auto& coord : coords) {
            if (coord.first == x && coord.second == y) {
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
        return false; // ��� ������� � ���� ������
    }
    return ship->isSunk();
}


bool Board::shoot(int x, int y) {
    if (x < 0 || x >= size || y < 0 || y >= size) {
        cout << "Shot outside the board. Try again." << endl;
        return false;
    }

    if (board[y][x] == 'S') {
        board[y][x] = 'X'; // �������� ���������
        cout << "Hit!" << endl;

        // ���������, ������ �� �� � �������
        Ship* hitShip = findShipByCoordinates(x, y);
        if (hitShip) {
            // ��������� �������� �������
            hitShip->hit();

            // ���������, �������� �� �������
            if (hitShip->isSunk()) {
                cout << "Ship sunk!" << endl;
                markSunkShip(hitShip); // �������� ����������� ������� �� �����
            }
        }

        return true;
    }
    else if (board[y][x] == '.' || board[y][x] == ' ') {
        board[y][x] = 'O'; // �������� ������
        cout << "Miss!" << endl;
        return false;
    }
    else if (board[y][x] == 'X' || board[y][x] == 'O') {
        cout << "You've already shot at this position. Try again." << endl;
        return false;
    }

    return false;
}

void Board::markSunkShip(Ship* ship) {
    const vector<pair<int, int>>& coords = ship->getCoordinates();

    // �������� ��� ������ ������� ��� �����������
    for (const auto& coord : coords) {
        int x = coord.first;
        int y = coord.second;
        Board::board[y][x] = '#'; // ��� ������ ������ ��� ������������ �������

        // �������� ������ ������ ������� ��� �������
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
    for (Ship* ship : ships) {
        if (!ship->isSunk()) {
            return false; // ���� ���� �� ���� ������� �� ��������
        }
    }
    return true; // ��� ������� ���������
}

bool Board::setShip(Ship* ship, int x, int y, bool horizontal, bool isPlayer) {
    // ���������� ������� �� ����� �� �������� ������
    int shipSize = ship->getSize();
    bool isPlayerShip = isPlayer;

    // ��������, ��� ������� ���������� �� �����
    if ((horizontal && x + shipSize > size) || (!horizontal && y + shipSize > size)) {
        if (isPlayerShip == true)
        {
            cout << "Ship extends beyond the board. Choose another position." << endl;
        }
        return false;
    }

    // ��������, ��� ������ �� ������ ������� ���������
    for (int i = 0; i < shipSize; i++) {
        int newX = horizontal ? x + i : x;
        int newY = horizontal ? y : y + i;

        // �������� ����� ������
        if (board[newY][newX] != '.') {
			if (isPlayerShip == true)
			{
				cout << "Debug - Cell (" << newX << "," << newY << ") contains: [" << (int)board[newY][newX] << "]" << endl;
                cout << "Cell (" << newX << "," << newY << ") is already occupied. Choose another position." << endl;
			}
            return false;
        }


        // �������� �������� ������ (�� �������� ���� ������� �� ����� ��������)
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int checkX = newX + dx;
                int checkY = newY + dy;

                if (checkX >= 0 && checkX < size && checkY >= 0 && checkY < size) 
                {
                    
                    if (board[checkY][checkX] == 'S') {
                        if (isPlayerShip == true)
                        {
                            cout << "Ships cannot touch each other. Choose another position." << endl;
                        }
                        return false;
                    }
                }
            }
        }
    }

    // ���� ��� �������� ��������, ��������� �������
    vector<pair<int, int>> coords;

    for (int i = 0; i < shipSize; i++) {
        int newX = horizontal ? x + i : x;
        int newY = horizontal ? y : y + i;

        coords.push_back({ newX, newY });
        board[newY][newX] = 'S';  // 'S' ��� ����������� �������
    }

    // ������������� ���������� � ������� �������
    ship->setCoordinates(coords);
    return true;
}