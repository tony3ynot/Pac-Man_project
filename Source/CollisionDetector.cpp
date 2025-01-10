#include "headers/CollisionDetector.h"

CollisionDetector::CollisionType CollisionDetector::operator()(const Player& player, const Map& map) const {
    int currX = player.getXIndex();
    int currY = player.getYIndex();

    switch (player.getCurrentDirection()) {
    case Player::LEFT:
        return checkBlock(currX - 1, currY, map, player);
        break;
    case Player::RIGHT:
        return checkBlock(currX + 1, currY, map, player);
        break;
    case Player::UP:
        return checkBlock(currX, currY - 1, map, player);
        break;
    case Player::DOWN:
        return checkBlock(currX, currY + 1, map, player);
        break;
    default:
        return NONE;
        break;
    }
}

// Pacman & Ghost - Map collision 
CollisionDetector::CollisionType CollisionDetector::checkBlock(int x, int y, const Map& map, const Player& player) const {
    if (x < 0 || x >= NUM_COL || y < 0 || y >= NUM_ROW) {
        return NONE;
    }
    if (map.getBlock(y, x).getType() == Block::WALL) {
        return WALL;
    }
    else if (map.getBlock(y, x).getType() == Block::DOT) {
        return DOT;
    }
    else if (map.getBlock(y, x).getType() == Block::BDOT) {
        return BDOT;
    }
    else if (map.getBlock(y, x).getType() == Block::GATE) {
        const Ghost* ghost = dynamic_cast<const Ghost*>(&player);
        if (ghost && ghost->getUsingGate()) {
            return NONE;
        }
        return GATE;
    }
    else return NONE;
}

// Pacman - Ghost collision
CollisionDetector::CollisionType CollisionDetector::operator()(const Pacman& pacman, const Ghost& ghost) {
    float distance = sqrt((pacman.getCenter()[0] - ghost.getCenter()[0]) * (pacman.getCenter()[0] - ghost.getCenter()[0]) + (pacman.getCenter()[1] - ghost.getCenter()[1]) * (pacman.getCenter()[1] - ghost.getCenter()[1]));
    if (distance <= pacman.getRadius() + ghost.getRadius()) {
        return GHOST;
    }
    else return NONE;
}
