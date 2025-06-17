#include <iostream>
#include <vector>
#include "Ship.h"
#include "FourCellShip.h"
#include "ThreeCellShip.h"
#include "TwoCellShip.h"
#include "OneCellShip.h"
#include "Board.h"
#include "ComputerPlayer.h"
#include <ctime>

using namespace std;

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	OneCellShip o;
	TwoCellShip t;
	ThreeCellShip th;
	FourCellShip f;
	Board playerBoard;
	Board npcBoard;

	vector<Ship*> playerShips =
	{
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

	vector<Ship*> npcShips =
	{
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

	// После создания кораблей NPC
	ComputerPlayer npc(&playerBoard);
	npc.placeShipsRandomly(npcBoard, npcShips);
	cout << "NPC has placed its ships." << endl << endl;

	cout << "Place your ships on the board: " << endl;
	playerBoard.printBoard();

	for (Ship* ship : playerShips) {
		bool placed = false;
		while (!placed) {
			int x, y;
			char direction;
			cout << "Place a ship of size " << ship->getSize() << ":" << endl;
			cout << "Enter X coordinate: ";
			cin >> x;
			cout << "Enter Y coordinate: ";
			cin >> y;
			cout << "Orientation (h - horizontal, v - vertical): ";
			cin >> direction;

			while (direction != 'h' && direction != 'H' && direction != 'v' && direction != 'V') {
				cout << "Invalid input. Please enter 'h' for horizontal or 'v' for vertical: ";
				cin >> direction;
			}

			bool horizontal = (direction == 'h' || direction == 'H');

			placed = playerBoard.setShip(ship, x, y, horizontal, true);
			if (placed) {
				cout << "Ship placed successfully!" << endl;
				playerBoard.printBoard();
			}
		}
	}

	bool gameOver = false;
	while (!gameOver) {
		// Ход игрока
		cout << "Your turn!" << endl;
		npcBoard.printBoard();
		int x, y;
		cout << "Enter X coordinate: ";
		cin >> x;
		cout << "Enter Y coordinate: ";
		cin >> y;

		bool hit = npcBoard.shoot(x, y);
		if (hit) {
			cout << "Hit!" << endl;
			if (npcBoard.isSunk(x, y)) {
				cout << "Ship sunk!" << endl;
			}
		}
		else {
			cout << "Miss!" << endl;
		}

		// Проверка, потоплены ли все корабли NPC
		if (npcBoard.allShipsSunk()) {
			cout << "You win!" << endl;
			gameOver = true;
			break;
		}

		// Ход NPC
		cout << "NPC's turn!" << endl;
		npc.makeMove();
		playerBoard.printBoard();

		// Проверка, потоплены ли все корабли игрока
		if (playerBoard.allShipsSunk()) {
			cout << "You lose!" << endl;
			gameOver = true;
		}
	}

	// после попадания в корабль игра заканчивается, исправить
	// добавить автоматическое роставление кораблей для игрока

	// Очистка памяти
	for (Ship* ship : playerShips) {
		delete ship;
	}
	for (Ship* ship : npcShips) {
		delete ship;
	}
	return 0;
}