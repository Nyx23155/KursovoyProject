#include <iostream>
#include "ComputerPlayer.h"
#include "Board.h"
#include "Ship.h"

ComputerPlayer::ComputerPlayer(Board* board) {
    playerBoard = board;

    // ����������� ��� �������� �������
    for (int i = 0; i < board->getSize(); i++) {
        for (int j = 0; j < board->getSize(); j++) {
            availableShots.push_back({ j, i });
        }
    }
    // ��������� ������������ ��� �����������������
    random_shuffle(availableShots.begin(), availableShots.end());
}

bool ComputerPlayer::makeMove() {
    pair<int, int> target;
    bool targetFound = false;

    // �������� ���������� ��������� ���
    if (!nextTargets.empty()) {
        target = nextTargets.back();
        nextTargets.pop_back();
        targetFound = true;
    }
    // ���� � ����� ��������, �������� ��� ������� ���
    else if (!successfulHits.empty()) {
        target = generateTargetsAroundHit();
        targetFound = true;
    }
    // ������ �������� ���������
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

        // ����������, �� ���������� ��������
        if (playerBoard->isSunk(x, y)) {
            cout << "Computer sunk your ship!" << endl;
            // ������� �� ���, ���'���� � ��� ��������
            successfulHits.clear();
            nextTargets.clear();
        }
        else {
            // ������ ������ ��������
            successfulHits.push_back({ x, y });

            // ���� �� ����� �������� �����, ��������� ��������
            if (successfulHits.size() >= 2) {
                updateTargetsBasedOnDirection();
            }
            else {
                // ����� �������� � ������ ����� ������
                addAdjacentCellsAsTargets(x, y);
            }
        }
    }
    else {
        cout << "Computer missed!" << endl;
    }

    // ��������� ����������� ���� �� ��������� �������
    removeFromAvailableShots(x, y);

    return hit; // ��������� ��������� �������
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

    // ������ ����� ����� �� �� �� ���������� ������
    vector<pair<int, int>> candidates = {
        {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}
    };

    for (const auto& candidate : candidates) {
        int cx = candidate.first;
        int cy = candidate.second;

        // ���������� ���
        if (cx >= 0 && cx < size && cy >= 0 && cy < size) {
            // ����������, �� �� �� �� ������� � �� ������
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

    if (x1 == x2) { // ������������ ��������
        if (y1 < y2) {
            nextTargets.push_back({ x1, y2 + 1 });
        }
        else {
            nextTargets.push_back({ x1, y2 - 1 });
        }
    }
    else if (y1 == y2) { // �������������� ��������
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
        // ��������� ��������� �������� ����
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

    // ���������� ����� ������ � ��������� ����� ��������
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

    // ���� ���� ��������� ������ �����, ��������� ���������
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