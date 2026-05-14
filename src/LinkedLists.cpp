#include "LinkedLists.h"

// GridList Implementation
GridList::GridList() : head(nullptr), size(0) {}

GridList::~GridList() {
    Clear();
}

void GridList::AddNode(int x, int y) {
    GridNode* newNode = new GridNode(x, y);
    if (!head) {
        head = newNode;
    } else {
        GridNode* curr = head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = newNode;
    }
    size++;
}

GridNode* GridList::GetNode(int x, int y) const {
    GridNode* curr = head;
    while (curr) {
        if (curr->x == x && curr->y == y) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

void GridList::Clear() {
    GridNode* curr = head;
    while (curr) {
        GridNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
    head = nullptr;
    size = 0;
}

// Ship Implementation
Ship::~Ship() {
    ShipNode* curr = partsHead;
    while (curr) {
        ShipNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
}

void Ship::AddPart(int x, int y) {
    ShipNode* newNode = new ShipNode(x, y);
    if (!partsHead) {
        partsHead = newNode;
    } else {
        ShipNode* curr = partsHead;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = newNode;
    }
}

// ShipList Implementation
ShipList::ShipList() : head(nullptr) {}

ShipList::~ShipList() {
    Clear();
}

void ShipList::AddShip(int id, int length) {
    Ship* newShip = new Ship(id, length);
    if (!head) {
        head = newShip;
    } else {
        Ship* curr = head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = newShip;
    }
}

Ship* ShipList::GetShip(int id) const {
    Ship* curr = head;
    while (curr) {
        if (curr->id == id) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

void ShipList::Clear() {
    Ship* curr = head;
    while (curr) {
        Ship* temp = curr;
        curr = curr->next;
        delete temp;
    }
    head = nullptr;
}
