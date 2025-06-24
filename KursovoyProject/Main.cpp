#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>
#include "Ship.h"
#include "FourCellShip.h"
#include "ThreeCellShip.h"
#include "TwoCellShip.h"
#include "OneCellShip.h"
#include "Board.h"
#include "ComputerPlayer.h"
#include "PlayerStats.h"
#include <ctime>

using namespace std;

void printLegend() {
    cout << "Legend: S - Ship, X - Hit, * - Miss, . - Empty, # - Sunk, O - Around sunk" << endl;
}

// ������� ��� ��������� �������� ������ ����� � �������
int getValidIntInput(int min, int max) {
    int input;
    while (true) {
        cin >> input;
        if (cin.fail() || input < min || input > max){
			cin.clear(); // ������� ���� ������
			cin.ignore(numeric_limits<streamsize>::max(), '\n');// �������� ������� �����
            cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        }
        else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // �������� ������� �����
            return input;
        }
    }
}

// ������� ��� ��������� �������� ���� ������
string getValidPlayerName() {
    string name;
    while (true) {
        cout << "Enter your player name (letters and numbers only, 3-20 characters): ";
        getline(cin, name);

        if (name.length() < 3 || name.length() > 20) {
            cout << "Name must be between 3 and 20 characters." << endl;
            continue;
        }

        bool valid = true;
        for (char c : name) {
            if (!isalnum(c)) {
                valid = false;
                break;
            }
        }

        if (valid) {
            return name;
        }
        else {
            cout << "Name can only contain letters and numbers." << endl;
        }
    }
}

// ������� ��� ������������ ����� � ����� �������
int letterToNumber(char letter) {
    if (letter >= 'A' && letter <= 'J') {
        return letter - 'A';
    }
    else if (letter >= 'a' && letter <= 'j') {
        return letter - 'a';
    }
	return -1; // ������� �����
}

// ������� ��� ��������� ������� ��������� �� ������
pair<int, int> getValidCoordinates() {
    char columnChar;
    int row;

    while (true) {
        cout << "Enter column (A-J): ";
        cin >> columnChar;

        int column = letterToNumber(columnChar);
        if (column == -1) {
            cout << "Invalid column. Please enter a letter from A to J." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cout << "Enter row (1-10): ";
        row = getValidIntInput(1, 10);

		return make_pair(column, row - 1); // ������������� ����� � ������ (0-9)
    }
}

// ������� ��� ��������� ���������� �������
bool getShipOrientation() {
    char direction;
    while (true) {
        cout << "Orientation (h - horizontal, v - vertical): ";
        cin >> direction;

        if (direction == 'h' || direction == 'H') {
            return true; // horizontal
        }
        else if (direction == 'v' || direction == 'V') {
            return false; // vertical
        }
        else {
            cout << "Invalid input. Please enter 'h' for horizontal or 'v' for vertical." << endl;
        }
    }
}

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    cout << "========================================" << endl;
    cout << "        Welcome to Battleship!" << endl;
    cout << "========================================" << endl;

    // ������� ������
    string playerName = getValidPlayerName();
    PlayerStats playerStats(playerName);

    bool isNewPlayer = !playerStats.loadFromFile();
    if (isNewPlayer) {
        cout << "Welcome, new player " << playerName << "!" << endl;
    }
    else {
        cout << "Welcome back, " << playerName << "!" << endl;
        playerStats.displayStats();
    }

    cout << endl;

    Board playerBoard;
    Board npcBoard;

    vector<Ship*> playerShips = {
        new FourCellShip(),
        new ThreeCellShip(),
        new ThreeCellShip(),
        new TwoCellShip(),
        new TwoCellShip(),
        new TwoCellShip(),
        new OneCellShip(),
        new OneCellShip(),
        new OneCellShip(),
        new OneCellShip()
    };

    vector<Ship*> npcShips = {
        new FourCellShip(),
        new ThreeCellShip(),
        new ThreeCellShip(),
        new TwoCellShip(),
        new TwoCellShip(),
        new TwoCellShip(),
        new OneCellShip(),
        new OneCellShip(),
        new OneCellShip(),
        new OneCellShip()
    };

    cout << "You will play against the computer." << endl;
    cout << "Enter coordinates using columns A-J and rows 1-10." << endl;
    cout << "Good luck!" << endl << endl;

	// ��������� ������� ����'�����
    ComputerPlayer npc(&playerBoard);
    npc.placeShipsRandomly(npcBoard, npcShips);
    cout << "Computer has placed its ships." << endl << endl;

	// ������� ����� ����� ��������� �������
    cout << "Choose how your ships will be placed:" << endl;
    cout << "1 - Manual" << endl;
    cout << "2 - Random" << endl;
    cout << "Enter your choice (1-2): ";
    int placementChoice = getValidIntInput(1, 2);

    if (placementChoice == 2) {
        ComputerPlayer playerPlacer(&npcBoard);
        playerPlacer.placeShipsRandomly(playerBoard, playerShips);
        cout << "Your ships have been placed randomly!" << endl;
        playerBoard.printBoard();
        printLegend();
    }
    else {
        cout << "Place your ships on the board:" << endl;
        playerBoard.printBoard();
        printLegend();

        for (Ship* ship : playerShips) {
            bool placed = false;
            while (!placed) {
                cout << "Place a ship of size " << ship->getSize() << ":" << endl;

                pair<int, int> coords = getValidCoordinates();
                int x = coords.first;
                int y = coords.second;

                bool horizontal = getShipOrientation();

                placed = playerBoard.setShip(ship, x, y, horizontal, true);
                if (placed) {
                    cout << "Ship placed successfully!" << endl;
                    playerBoard.printBoard();
                    printLegend();
                }
            }
        }
    }

    bool gameOver = false;
    while (!gameOver) {
        // ճ� ������
        bool playerHit = true;
        while (playerHit && !gameOver) {
            cout << "----------------------------------------" << endl;
            cout << "Your turn!" << endl;
            npcBoard.printBoardHidden();
            printLegend();

            pair<int, int> coords = getValidCoordinates();
            int x = coords.first;
            int y = coords.second;

            playerHit = npcBoard.shoot(x, y);
            if (playerHit) {
                cout << "Result: Hit!" << endl;
                if (npcBoard.isSunk(x, y)) {
                    cout << "Result: Ship sunk!" << endl;
					// �������� ��������, ������� �� ���, ���'���� � ��� ��������
                }
                cout << "You hit! Take another shot!" << endl;
            }
            else {
                cout << "Result: Miss!" << endl;
            }

			// ��������, �������� �� �� ������ ����'�����
            if (npcBoard.allShipsSunk()) {
                cout << "========================================" << endl;
                cout << "    Congratulations! You win!" << endl;
                cout << "========================================" << endl;
                playerStats.addWin();
                playerStats.displayStats();
                gameOver = true;
                break;
            }
        }

        if (gameOver) break;

		// ճ� ����'����� - ���������� ���� �� ������������
        bool npcHit = true;
        while (npcHit && !gameOver) {
            cout << "----------------------------------------" << endl;
            cout << "Computer's turn!" << endl;

			// ������ ��� ����'�����
            npcHit = npc.makeMove();

            playerBoard.printBoard();
            printLegend();

            if (npcHit) {
                cout << "Computer hit! Computer takes another shot!" << endl;
            }

			// ��������, �������� �� �� ������ ������
            if (playerBoard.allShipsSunk()) {
                cout << "========================================" << endl;
                cout << "        Sorry, you lose!" << endl; 
                cout << "========================================" << endl;
                playerStats.addLoss();
                playerStats.displayStats();
                gameOver = true;
            }
        }
    }

    // ������� ������
    for (Ship* ship : playerShips) {
        delete ship;
    }
    for (Ship* ship : npcShips) {
        delete ship;
    }
    return 0;
}