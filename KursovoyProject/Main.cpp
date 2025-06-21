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

void printLegend() {
    cout << "Legend: S - Ship, X - Hit, * - Miss, . - Empty" << endl;
}

int main()
{
    srand(static_cast<unsigned int>(time(0)));

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

    cout << "========================================" << endl;
    cout << "        Welcome to Battleship!" << endl;
    cout << "========================================" << endl;
    cout << "You will play against the computer." << endl;
    cout << "Enter coordinates as numbers from 0 to 9." << endl;
    cout << "Good luck!" << endl << endl;

    // Розміщення кораблів комп'ютера
    ComputerPlayer npc(&playerBoard);
    npc.placeShipsRandomly(npcBoard, npcShips);
    cout << "Computer has placed its ships." << endl << endl;

    // Гравець обирає спосіб розміщення кораблів
    cout << "Choose how your ships will be placed:" << endl;
    cout << "1 - Manual" << endl;
    cout << "2 - Random" << endl;
    int placementChoice;
    cin >> placementChoice;

    if (placementChoice == 2) {
        ComputerPlayer playerPlacer(&npcBoard); // дошка не використовується, лише для інтерфейсу
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
                int x, y;
                char direction;
                cout << "Place a ship of size " << ship->getSize() << ":" << endl;
                cout << "Enter X coordinate (0-9): ";
                cin >> x;
                cout << "Enter Y coordinate (0-9): ";
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
                    printLegend();
                }
            }
        }
    }

    bool gameOver = false;
    while (!gameOver) {
        // Хід гравця - продовжуємо доки не промажемо
        bool playerHit = true;
        while (playerHit && !gameOver) {
            cout << "----------------------------------------" << endl;
            cout << "Your turn!" << endl;
            npcBoard.printBoardHidden();
            printLegend();
            int x, y;
            cout << "Enter X coordinate (0-9): ";
            cin >> x;
            cout << "Enter Y coordinate (0-9): ";
            cin >> y;

            playerHit = npcBoard.shoot(x, y);
            if (playerHit) {
                cout << "Result: Hit!" << endl;
                if (npcBoard.isSunk(x, y)) {
                    cout << "Result: Ship sunk!" << endl;
                    // Корабель вже позначено як потоплений у методі shoot()
                }
                cout << "You hit! Take another shot!" << endl;
            }
            else {
                cout << "Result: Miss!" << endl;
            }

            // Перевірка, чи всі кораблі комп'ютера потоплені
            if (npcBoard.allShipsSunk()) {
                cout << "Congratulations! You win!" << endl;
                gameOver = true;
                break;
            }
        }

        if (gameOver) break;

        // Хід комп'ютера - продовжуємо доки не промажемо
        bool npcHit = true;
        while (npcHit && !gameOver) {
            cout << "----------------------------------------" << endl;
            cout << "Computer's turn!" << endl;

            // Робимо хід комп'ютера і отримуємо результат
            npcHit = npc.makeMove();

            playerBoard.printBoard();
            printLegend();

            if (npcHit) {
                cout << "Computer hit! Computer takes another shot!" << endl;
            }

            // Перевірка, чи всі кораблі гравця потоплені
            if (playerBoard.allShipsSunk()) {
                cout << "Sorry, you lose!" << endl;
                gameOver = true;
            }
        }
    }

    // Очищення пам'яті
    for (Ship* ship : playerShips) {
        delete ship;
    }
    for (Ship* ship : npcShips) {
        delete ship;
    }
    return 0;
}