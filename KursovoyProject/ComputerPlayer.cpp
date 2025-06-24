#include <iostream>
#include "ComputerPlayer.h"
#include "Board.h"
#include "Ship.h"

ComputerPlayer::ComputerPlayer(Board* board) {
    playerBoard = board;

    // ����������� ��� �������� �������
    // ������������� ���������� (x, y) �� x - �������, y - �����
    for (int y = 0; y < board->getSize(); y++) {
        for (int x = 0; x < board->getSize(); x++) {
            availableShots.push_back({ x, y });
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

    int x = target.first;  // �������
    int y = target.second; // �����

    // �������� ���������� � ������, �� ������� �������� ����
    char columnLetter = 'A' + x;
    cout << "Computer shoots at (" << columnLetter << "," << (y + 1) << ")" << endl;

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
            int x = rand() % board.getSize();
            int y = rand() % board.getSize();
            bool horizontal = rand() % 2 == 0;

            // ����������, �� �������� ���������� �� �����
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

    // ������ ����� ����� �� �� �� ���������� ������
    // ���������� 4 ����� ������: �����, ����, ����, ������
    vector<pair<int, int>> candidates = {
        {x - 1, y}, // ����
        {x + 1, y}, // ������
        {x, y - 1}, // �����
        {x, y + 1}  // ����
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

    // ������ ������ ��� �������� ��� ���������� ��������
    int x1 = successfulHits[successfulHits.size() - 2].first;
    int y1 = successfulHits[successfulHits.size() - 2].second;
    int x2 = successfulHits[successfulHits.size() - 1].first;
    int y2 = successfulHits[successfulHits.size() - 1].second;

    // ������� �������� ���
    nextTargets.clear();

    if (x1 == x2) { // ������������ �������� (��� ����� ��������)
        // ������ ��� �� ����������� ��
        int minY = min(y1, y2);
        int maxY = max(y1, y2);

        // ������ ���� ���� ���������� y
        if (minY - 1 >= 0) {
            pair<int, int> target = { x1, minY - 1 };
            auto it = find(availableShots.begin(), availableShots.end(), target);
            if (it != availableShots.end()) {
                nextTargets.push_back(target);
            }
        }

        // ������ ���� ����� ������������� y
        if (maxY + 1 < playerBoard->getSize()) {
            pair<int, int> target = { x1, maxY + 1 };
            auto it = find(availableShots.begin(), availableShots.end(), target);
            if (it != availableShots.end()) {
                nextTargets.push_back(target);
            }
        }
    }
    else if (y1 == y2) { // �������������� �������� (��� ����� �����)
        // ������ ��� �� ����������� ��
        int minX = min(x1, x2);
        int maxX = max(x1, x2);

        // ������ ���� ���� ���������� x
        if (minX - 1 >= 0) {
            pair<int, int> target = { minX - 1, y1 };
            auto it = find(availableShots.begin(), availableShots.end(), target);
            if (it != availableShots.end()) {
                nextTargets.push_back(target);
            }
        }

        // ������ ���� ������ ������������� x
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
        {x - 1, y}, // ����
        {x + 1, y}, // ������
        {x, y - 1}, // �����
        {x, y + 1}  // ����
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