#pragma once
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class PlayerStats {
private:
    string playerName;
    int wins;
    int losses;
    int totalGames;

public:
    PlayerStats(const string& name = "");

    void addWin();
    void addLoss();

    string getName() const { return playerName; }
    int getWins() const { return wins; }
    int getLosses() const { return losses; }
    int getTotalGames() const { return totalGames; }

    bool loadFromFile();
    void saveToFile() const;
    void displayStats() const;
};