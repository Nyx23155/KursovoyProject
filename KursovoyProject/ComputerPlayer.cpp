#include <iostream>
#include "ComputerPlayer.h"
#include "Board.h"
#include "Ship.h"

ComputerPlayer::ComputerPlayer(Board* board) {
    playerBoard = board;

    // Ініціалізація всіх можливих пострілів
    for (int i = 0; i < board->getSize(); i++) {
        for (int j = 0; j < board->getSize(); j++) {
            availableShots.push_back({ j, i });
        }
    }
    // Випадкове перемішування для непередбачуваності
    random_shuffle(availableShots.begin(), availableShots.end());
}

bool ComputerPlayer::makeMove() {
    pair<int, int> target;
    bool targetFound = false;

    // Спочатку перевіряємо пріоритетні цілі
    if (!nextTargets.empty()) {
        target = nextTargets.back();
        nextTargets.pop_back();
        targetFound = true;
    }
    // Якщо є успішні влучання, генеруємо цілі навколо них
    else if (!successfulHits.empty()) {
        target = generateTargetsAroundHit();
        targetFound = true;
    }
    // Інакше стріляємо випадково
    else if (!availableShots.empty()) {
        target = availableShots.back();
        availableShots.pop_back();
        targetFound = true;
    }

    if (!targetFound) {
        cout << "No available targets!" << endl;
        return false;
    }

    int x = target.first;
    int y = target.second;
    cout << "Computer shoots at (" << x << "," << y << ")" << endl;

    bool hit = playerBoard->shoot(x, y);

    if (hit) {
        cout << "Computer hit!" << endl;

        // Перевіряємо, чи потоплений корабель
        if (playerBoard->isSunk(x, y)) {
            cout << "Computer sunk your ship!" << endl;
            // Очищаємо всі цілі, пов'язані з цим кораблем
            successfulHits.clear();
            nextTargets.clear();
        }
        else {
            // Додаємо успішне влучання
            successfulHits.push_back({ x, y });

            // Якщо це друге влучання підряд, визначаємо напрямок
            if (successfulHits.size() >= 2) {
                updateTargetsBasedOnDirection();
            }
            else {
                // Перше влучання — додаємо сусідні клітини
                addAdjacentCellsAsTargets(x, y);
            }
        }
    }
    else {
        cout << "Computer missed!" << endl;
    }

    // Видаляємо використану ціль із доступних пострілів
    removeFromAvailableShots(x, y);

    return hit; // Повертаємо результат пострілу
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

void ComputerPlayer::addAdjacentCellsAsTargets(int x, int y) {
    int size = playerBoard->getSize();

    // Додаємо тільки валідні та ще не використані клітини
    vector<pair<int, int>> candidates = {
        {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}
    };

    for (const auto& candidate : candidates) {
        int cx = candidate.first;
        int cy = candidate.second;

        // Перевіряємо межі
        if (cx >= 0 && cx < size && cy >= 0 && cy < size) {
            // Перевіряємо, що ми ще не стріляли в цю клітину
            auto it = find(availableShots.begin(), availableShots.end(), candidate);
            if (it != availableShots.end()) {
                nextTargets.push_back(candidate);
            }
        }
    }
}

void ComputerPlayer::updateTargetsBasedOnDirection() {
    if (successfulHits.size() < 2) return;

    int x1 = successfulHits[0].first;
    int y1 = successfulHits[0].second;
    int x2 = successfulHits[1].first;
    int y2 = successfulHits[1].second;

    if (x1 == x2) { // Вертикальний напрямок
        if (y1 < y2) {
            nextTargets.push_back({ x1, y2 + 1 });
        }
        else {
            nextTargets.push_back({ x1, y2 - 1 });
        }
    }
    else if (y1 == y2) { // Горизонтальний напрямок
        if (x1 < x2) {
            nextTargets.push_back({ x2 + 1, y1 });
        }
        else {
            nextTargets.push_back({ x2 - 1, y1 });
        }
    }
}

pair<int, int> ComputerPlayer::generateTargetsAroundHit() {
    if (successfulHits.empty()) {
        // Повертаємо випадкову доступну ціль
        if (!availableShots.empty()) {
            pair<int, int> target = availableShots.back();
            availableShots.pop_back();
            return target;
        }
        return { 0, 0 };
    }

    int x = successfulHits.back().first;
    int y = successfulHits.back().second;
    int size = playerBoard->getSize();

    // Перевіряємо сусідні клітини і повертаємо першу доступну
    vector<pair<int, int>> candidates = {
        {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}
    };

    for (const auto& candidate : candidates) {
        int cx = candidate.first;
        int cy = candidate.second;

        if (cx >= 0 && cx < size && cy >= 0 && cy < size) {
            auto it = find(availableShots.begin(), availableShots.end(), candidate);
            if (it != availableShots.end()) {
                availableShots.erase(it);
                return candidate;
            }
        }
    }

    // Якщо немає доступних сусідніх клітин, повертаємо випадкову
    if (!availableShots.empty()) {
        pair<int, int> target = availableShots.back();
        availableShots.pop_back();
        return target;
    }

    return { 0, 0 };
}

void ComputerPlayer::removeFromAvailableShots(int x, int y) {
    auto it = find(availableShots.begin(), availableShots.end(), make_pair(x, y));
    if (it != availableShots.end()) {
        availableShots.erase(it);
    }
}