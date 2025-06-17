#include <iostream>
#include "ComputerPlayer.h"
#include "Board.h"
#include "Ship.h"

ComputerPlayer::ComputerPlayer(Board* board) {
    playerBoard = board;

    // Инициализация всех возможных выстрелов
    for (int i = 0; i < board->getSize(); i++) {
        for (int j = 0; j < board->getSize(); j++) {
            availableShots.push_back({ j, i });
        }
    }
    // Случайное перемешивание для непредсказуемости
    random_shuffle(availableShots.begin(), availableShots.end());
}

void ComputerPlayer::makeMove() {
    pair<int, int> target;

    if (!nextTargets.empty()) {
        target = nextTargets.back();
        nextTargets.pop_back();
    }
    else if (!successfulHits.empty()) {
        target = generateTargetsAroundHit();
    }
    else if (!availableShots.empty()) {
        target = availableShots.back();
        availableShots.pop_back();
    }

    int x = target.first;
    int y = target.second;
    cout << "Computer shoots at (" << x << "," << y << ")" << endl;

    bool hit = playerBoard->shoot(x, y);

    if (hit) {
        if (playerBoard->isSunk(x, y)) {
            successfulHits.clear();
            nextTargets.clear();
        }
        else {
            successfulHits.push_back({ x, y });

            if (successfulHits.size() >= 2) {
                updateTargetsBasedOnDirection();
            }
            else {
                addAdjacentCellsAsTargets(x, y);
            }
        }
    }

    removeFromAvailableShots(x, y);
}

void ComputerPlayer::placeShipsRandomly(Board& board, vector<Ship*>& npcShips) {
    for (Ship* ship : npcShips) {
        bool placed = false;
        while (!placed) {
            int x, y;
            bool horizontal = rand() % 2 == 0;
            if (horizontal) {
                x = rand() % board.getSize();
                y = rand() % board.getSize();
            }
            else {
                x = rand() % (board.getSize() - ship->getSize() + 1);
                y = rand() % board.getSize();
            }
            placed = board.setShip(ship, x, y, horizontal, false);
        }
    }
}

// Реализации вспомогательных методов
void ComputerPlayer::addAdjacentCellsAsTargets(int x, int y) {
    int size = playerBoard->getSize();
    if (x > 0) nextTargets.push_back({ x - 1, y });
    if (x < size - 1) nextTargets.push_back({ x + 1, y });
    if (y > 0) nextTargets.push_back({ x, y - 1 });
    if (y < size - 1) nextTargets.push_back({ x, y + 1 });
}

void ComputerPlayer::updateTargetsBasedOnDirection() {
    if (successfulHits.size() < 2) return;

    int x1 = successfulHits[0].first;
    int y1 = successfulHits[0].second;
    int x2 = successfulHits[1].first;
    int y2 = successfulHits[1].second;

    if (x1 == x2) { // Вертикальное направление
        if (y1 < y2) {
            nextTargets.push_back({ x1, y2 + 1 });
        }
        else {
            nextTargets.push_back({ x1, y2 - 1 });
        }
    }
    else if (y1 == y2) { // Горизонтальное направление
        if (x1 < x2) {
            nextTargets.push_back({ x2 + 1, y1 });
        }
        else {
            nextTargets.push_back({ x2 - 1, y1 });
        }
    }
}

pair<int, int> ComputerPlayer::generateTargetsAroundHit() {
    if (successfulHits.empty()) return { 0, 0 };

    int x = successfulHits.back().first;
    int y = successfulHits.back().second;

    // Возвращаем первую доступную соседнюю клетку
    if (x > 0) return { x - 1, y };
    if (x < playerBoard->getSize() - 1) return { x + 1, y };
    if (y > 0) return { x, y - 1 };
    if (y < playerBoard->getSize() - 1) return { x, y + 1 };

    return { 0, 0 }; // Если нет доступных клеток
}

void ComputerPlayer::removeFromAvailableShots(int x, int y) {
    auto it = find(availableShots.begin(), availableShots.end(), make_pair(x, y));
    if (it != availableShots.end()) {
        availableShots.erase(it);
    }
}
