#pragma once

#include "Player.h"
#include "Ghost.h"
#include "Map.h"

class CollisionDetector {
public:
    enum CollisionType { NONE, WALL, DOT, BDOT, GHOST, GATE };
    CollisionType operator()(const Player& player, const Map& map) const;
    CollisionType operator()(const Pacman& pacman, const Ghost& ghost);
private:
    CollisionType checkBlock(int x, int y, const Map& map, const Player& player) const;
};


