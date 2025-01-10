#pragma once

#include "CollisionDetector.h"
#include "Score.h"
#include "Life.h"
#include "Sound.h"

class CollisionHandler {
public:
    void operator()(Player& player, const Map& map);
    void operator()(Pacman& pacman, Map& map, Score& score, std::vector<Ghost>& ghosts);
    void operator()(Pacman& pacman, Ghost& ghost, Score& score, Life& life);
private:
    Sound sound;
};