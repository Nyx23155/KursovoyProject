#include "PlayerStats.h"

PlayerStats::PlayerStats(const string& name) : playerName(name), wins(0), losses(0), totalGames(0) {}

void PlayerStats::addWin() {
    wins++;
    totalGames++;
    saveToFile();
}

void PlayerStats::addLoss() {
    losses++;
    totalGames++;
    saveToFile();
}

bool PlayerStats::loadFromFile() {
    if (playerName.empty()) return false;

    string filename = playerName + ".txt";
    ifstream file(filename);

    if (file.is_open()) {
        file >> wins >> losses >> totalGames;
        file.close();
        return true;
    }

    // Если файл не найден, создаем нового игрока
    wins = 0;
    losses = 0;
    totalGames = 0;
    return false;
}

void PlayerStats::saveToFile() const {
    if (playerName.empty()) return;

    string filename = playerName + ".txt";
    ofstream file(filename);

    if (file.is_open()) {
        file << wins << " " << losses << " " << totalGames << endl;
        file.close();
    }
}

void PlayerStats::displayStats() const {
    cout << "========================================" << endl;
    cout << "Player: " << playerName << endl;
    cout << "Total games: " << totalGames << endl;
    cout << "Wins: " << wins << endl;
    cout << "Losses: " << losses << endl;
    if (totalGames > 0) {
        double winRate = (double)wins / totalGames * 100;
        cout << "Win rate: " << winRate << "%" << endl;
    }
    cout << "========================================" << endl;
}