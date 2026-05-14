#include "GameLogic.h"
#include <cstdlib>
#include <ctime>

GameLogic::GameLogic() : maxShots(20), totalShips(4) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    InitGame();
}

GameLogic::~GameLogic() {}

void GameLogic::InitGame() {
    grid.Clear();
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            grid.AddNode(x, y);
        }
    }
    ResetGame();
}

void GameLogic::ResetGame() {
    // Reset grid
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            GridNode* node = grid.GetNode(x, y);
            if (node) {
                node->state = GridState::Empty;
            }
        }
    }
    
    // Reset ships
    ships.Clear();
    shotsLeft = maxShots;
    shipsSunk = 0;
    state = GameState::Menu;

    RandomizeShips();
}

bool GameLogic::IsValidPlacement(int startX, int startY, int length, bool horizontal) {
    if (horizontal) {
        if (startX + length > 10) return false;
        for (int i = 0; i < length; ++i) {
            GridNode* node = grid.GetNode(startX + i, startY);
            if (!node || node->state != GridState::Empty) return false;
        }
    } else {
        if (startY + length > 10) return false;
        for (int i = 0; i < length; ++i) {
            GridNode* node = grid.GetNode(startX, startY + i);
            if (!node || node->state != GridState::Empty) return false;
        }
    }
    return true;
}

void GameLogic::PlaceShip(int id, int length) {
    bool placed = false;
    while (!placed) {
        int x = std::rand() % 10;
        int y = std::rand() % 10;
        bool horizontal = (std::rand() % 2) == 0;

        if (IsValidPlacement(x, y, length, horizontal)) {
            ships.AddShip(id, length);
            Ship* currentShip = ships.GetShip(id);
            
            for (int i = 0; i < length; ++i) {
                int cx = horizontal ? x + i : x;
                int cy = horizontal ? y : y + i;
                
                GridNode* node = grid.GetNode(cx, cy);
                if (node) {
                    node->state = GridState::Ship;
                }
                currentShip->AddPart(cx, cy);
            }
            placed = true;
        }
    }
}

void GameLogic::RandomizeShips() {
    int shipLengths[] = {1, 2, 3, 4};
    for (int i = 0; i < totalShips; ++i) {
        PlaceShip(i, shipLengths[i]);
    }
}

bool GameLogic::Fire(int x, int y) {
    if (state != GameState::Playing) return false;
    if (shotsLeft <= 0) return false;

    GridNode* node = grid.GetNode(x, y);
    if (!node || node->state == GridState::Hit || node->state == GridState::Miss) {
        return false; // Already fired here
    }

    shotsLeft--;
    bool hit = false;

    if (node->state == GridState::Ship) {
        node->state = GridState::Hit;
        hit = true;
        
        // Find which ship was hit
        Ship* currentShip = ships.GetHead();
        while (currentShip) {
            ShipNode* part = currentShip->partsHead;
            bool foundPart = false;
            while (part) {
                if (part->x == x && part->y == y) {
                    foundPart = true;
                    break;
                }
                part = part->next;
            }
            if (foundPart) {
                currentShip->hits++;
                if (currentShip->hits == currentShip->length) {
                    currentShip->isSunk = true;
                    shipsSunk++;
                }
                break;
            }
            currentShip = currentShip->next;
        }
    } else if (node->state == GridState::Empty) {
        node->state = GridState::Miss;
    }

    UpdateGameState();
    return hit;
}

void GameLogic::UpdateGameState() {
    if (shipsSunk == totalShips) {
        state = GameState::InputName;
    } else if (shotsLeft == 0) {
        state = GameState::GameOver;
    }
}
