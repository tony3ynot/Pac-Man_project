#include "headers/CollisionHandler.h"

// Wall, Gate collision
void CollisionHandler::operator()(Player& player, const Map& map) {
    CollisionDetector detector;
    if (detector(player, map) == CollisionDetector::WALL || detector(player, map) == CollisionDetector::GATE) {
        player.setCurrentDirection(Player::NONE);
        player.setVelocity(0.0f, 0.0f, 0.0f);
        return;
    }
}

// Dot collision
void CollisionHandler::operator()(Pacman& pacman, Map& map, Score& score, std::vector<Ghost>& ghosts) {
    int pacmanX = pacman.getXIndex();
    int pacmanY = pacman.getYIndex();
    Block& block = map.getBlock(pacmanY, pacmanX);

    if (block.getType() == Block::DOT) {
        sound.DotSound();
        score.addScore(10);
        block.setType(Block::EMPTY);
        map.decreaseDots();
    }
    else if (block.getType() == Block::BDOT) {
        sound.BdotSound();
        score.addScore(50);
        block.setType(Block::EMPTY);
        map.decreaseDots();

        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        for (auto& ghost : ghosts) {
            ghost.setState(Ghost::FRIGHTENED);
            ghost.stateTimer = currentTime;
            ghost.setCurrentDirection(Player::NONE);
        }
    }
}

// Ghost collision
void CollisionHandler::operator()(Pacman& pacman, Ghost& ghost, Score& score, Life& life) {
    CollisionDetector detector;

    if (detector(pacman, ghost) == CollisionDetector::GHOST) {
        // Eat Ghost
        if (ghost.getState() == Ghost::FRIGHTENED) {
            sound.EatGhostSound();
            pacman.setCollided(true);
            ghost.setState(Ghost::EATEN);
            ghost.setCurrentDirection(Player::NONE);
            ghost.setUsingGate(true);
            score.addScore(400); 
        }
        // Pacman dies
        else if (ghost.getState() == Ghost::SCATTER || ghost.getState() == Ghost::CHASE) {
            sound.DeadSound();
            pacman.setCollided(true);
            life.decreaseLife();
        }
    } 
    else {
        pacman.setCollided(false);  
    }
}
