#pragma once
#include "LinkedLists.h"

enum class GameState {
    Menu,
    Playing,
    GameOver,
    Win,
    InputName,
    Rules,
    Leaderboard
};

class GameLogic {
public:
    GridList grid;
    ShipList ships;
    int shotsLeft;
    int maxShots;
    int shipsSunk;
    int totalShips;
    GameState state;

    GameLogic();
    ~GameLogic();

    void InitGame();
    void ResetGame();
    
    // returns true if it was a hit
    bool Fire(int x, int y);
    void UpdateGameState();

    bool IsValidPlacement(int startX, int startY, int length, bool horizontal);
    void PlaceShip(int id, int length);
    void RandomizeShips();
};
