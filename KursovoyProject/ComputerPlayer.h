#include <vector>
#include <iostream>
#include <algorithm>
#include "Board.h"

using namespace std;

class ComputerPlayer {
private:
    Board* playerBoard;
    vector<pair<int, int>> availableShots;
    vector<pair<int, int>> successfulHits;
    vector<pair<int, int>> nextTargets;

    void addAdjacentCellsAsTargets(int x, int y);
    void updateTargetsBasedOnDirection();
    pair<int, int> generateTargetsAroundHit();
    void removeFromAvailableShots(int x, int y);
    


public:
    ComputerPlayer(Board* board);
    void placeShipsRandomly(Board& board, vector<Ship*>& npcShips);
    bool makeMove();
};