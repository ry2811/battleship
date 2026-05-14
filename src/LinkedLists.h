#pragma once

enum class GridState {
    Empty,
    Ship,
    Hit,
    Miss
};

struct GridNode {
    int x;
    int y;
    GridState state;
    GridNode* next;

    GridNode(int _x, int _y) : x(_x), y(_y), state(GridState::Empty), next(nullptr) {}
};

class GridList {
private:
    GridNode* head;
    int size;

public:
    GridList();
    ~GridList();

    void AddNode(int x, int y);
    GridNode* GetNode(int x, int y) const;
    void Clear();
};

struct ShipNode {
    int x;
    int y;
    ShipNode* next;

    ShipNode(int _x, int _y) : x(_x), y(_y), next(nullptr) {}
};

struct Ship {
    int id;
    int length;
    int hits;
    bool isSunk;
    ShipNode* partsHead;
    Ship* next;

    Ship(int _id, int _length) : id(_id), length(_length), hits(0), isSunk(false), partsHead(nullptr), next(nullptr) {}
    ~Ship();

    void AddPart(int x, int y);
};

class ShipList {
private:
    Ship* head;

public:
    ShipList();
    ~ShipList();

    void AddShip(int id, int length);
    Ship* GetShip(int id) const;
    Ship* GetHead() const { return head; }
    void Clear();
};
