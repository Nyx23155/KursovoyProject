#include <iostream>
#include "ComputerPlayer.h"
#include "Board.h"
#include "Ship.h"

ComputerPlayer::ComputerPlayer(Board* board) {
    playerBoard = board;

    // Ініціалізація всіх можливих пострілів
    // Використовуємо координати (x, y) де x - колонка, y - рядок
    for (int y = 0; y < board->getSize(); y++) {
        for (int x = 0; x < board->getSize(); x++) {
            availableShots.push_back({ x, y });
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

    int x = target.first;  // колонка
    int y = target.second; // рядок

    // Виводимо координати у форматі, що відповідає ігровому полю
    char columnLetter = 'A' + x;
    cout << "Computer shoots at (" << columnLetter << "," << (y + 1) << ")" << endl;

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
            int x = rand() % board.getSize();
            int y = rand() % board.getSize();
            bool horizontal = rand() % 2 == 0;

            // Перевіряємо, чи корабель поміститься на дошці
            if (horizontal && x + ship->getSize() > board.getSize()) {
                continue;
            }
            if (!horizontal && y + ship->getSize() > board.getSize()) {
                continue;
            }

            placed = board.setShip(ship, x, y, horizontal, false);
        }
    }
}

void ComputerPlayer::addAdjacentCellsAsTargets(int x, int y) {
    int size = playerBoard->getSize();

    // Додаємо тільки валідні та ще не використані клітини
    // Перевіряємо 4 сусідні клітини: вгору, вниз, вліво, вправо
    vector<pair<int, int>> candidates = {
        {x - 1, y}, // вліво
        {x + 1, y}, // вправо
        {x, y - 1}, // вгору
        {x, y + 1}  // вниз
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

    // Беремо останні два влучання для визначення напрямку
    int x1 = successfulHits[successfulHits.size() - 2].first;
    int y1 = successfulHits[successfulHits.size() - 2].second;
    int x2 = successfulHits[successfulHits.size() - 1].first;
    int y2 = successfulHits[successfulHits.size() - 1].second;

    // Очищаємо попередні цілі
    nextTargets.clear();

    if (x1 == x2) { // Вертикальний напрямок (той самий стовпець)
        // Додаємо цілі на продовження лінії
        int minY = min(y1, y2);
        int maxY = max(y1, y2);

        // Додаємо ціль вище мінімального y
        if (minY - 1 >= 0) {
            pair<int, int> target = { x1, minY - 1 };
            auto it = find(availableShots.begin(), availableShots.end(), target);
            if (it != availableShots.end()) {
                nextTargets.push_back(target);
            }
        }

        // Додаємо ціль нижче максимального y
        if (maxY + 1 < playerBoard->getSize()) {
            pair<int, int> target = { x1, maxY + 1 };
            auto it = find(availableShots.begin(), availableShots.end(), target);
            if (it != availableShots.end()) {
                nextTargets.push_back(target);
            }
        }
    }
    else if (y1 == y2) { // Горизонтальний напрямок (той самий рядок)
        // Додаємо цілі на продовження лінії
        int minX = min(x1, x2);
        int maxX = max(x1, x2);

        // Додаємо ціль лівіше мінімального x
        if (minX - 1 >= 0) {
            pair<int, int> target = { minX - 1, y1 };
            auto it = find(availableShots.begin(), availableShots.end(), target);
            if (it != availableShots.end()) {
                nextTargets.push_back(target);
            }
        }

        // Додаємо ціль правіше максимального x
        if (maxX + 1 < playerBoard->getSize()) {
            pair<int, int> target = { maxX + 1, y1 };
            auto it = find(availableShots.begin(), availableShots.end(), target);
            if (it != availableShots.end()) {
                nextTargets.push_back(target);
            }
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
        {x - 1, y}, // вліво
        {x + 1, y}, // вправо
        {x, y - 1}, // вгору
        {x, y + 1}  // вниз
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