#include "headers/Player.h"
#include "headers/Ghost.h"

Player::Player(float r) : Shape2D(), radius(r) {
    idxPos[0] = 0;
    idxPos[1] = 0;
    bInxPosUpdated = true;
    currDir = NONE;
    nextDir = NONE;
}


void Player::setIndexPosition(int x, int y) {
    idxPos[0] = x;
    idxPos[1] = y;
    setCenter(LEFT_BOUNDARY + x * BLOCK_SIZE, TOP_BOUNDARY - y * BLOCK_SIZE, 0.0f);
}
bool Player::isIndexPositionUpdated() const {
    return bInxPosUpdated;
}
int Player::getXIndex() const {
    return idxPos[0];
}
int Player::getYIndex() const {
    return idxPos[1];
}
void Player::setRadius(float r) {
    radius = r;
}
float Player::getRadius() const {
    return radius;
}


void Player::setCurrentDirection(DIRECTION d) {
    currDir = d;
    float speed = MOVE_SPEED;
    
    // Ghost speed management
    const Ghost* ghost = dynamic_cast<const Ghost*>(this);
    if (ghost) {
        if (ghost->getState() == Ghost::FRIGHTENED) {
            speed = MOVE_SPEED * 0.7f;
        }
        else if (ghost->getState() == Ghost::EATEN) {
            speed = MOVE_SPEED * 3.0f;
        }
    }

    switch (currDir) {
    case NONE:
        setVelocity(0.0f, 0.0f, 0.0f);
        break;
    case LEFT:
        setVelocity(-speed, 0.0f, 0.0f);
        break;
    case RIGHT:
        setVelocity(speed, 0.0f, 0.0f);
        break;
    case UP:
        setVelocity(0.0f, speed, 0.0f);
        break;
    case DOWN:
        setVelocity(0.0f, -speed, 0.0f);
        break;
    default:
        break;
    }
}
void Player::setNextDirection(DIRECTION d) {
    nextDir = d;
}
Player::DIRECTION Player::getCurrentDirection() const {
    return currDir;
}
Player::DIRECTION Player::getNextDirection() const {
    return nextDir;
}
void Player::updateDirection() {
    setCurrentDirection(nextDir);
}


void Player::move() {
    float xFromIdx = LEFT_BOUNDARY + idxPos[0] * BLOCK_SIZE;
    float yFromIdx = TOP_BOUNDARY - idxPos[1] * BLOCK_SIZE;

    Shape2D::move();
    bInxPosUpdated = false;

    // moving left
    if (velocity[0] < 0.0f) {
        if (center[0] + radius * 2.0f <= xFromIdx) {
            if (idxPos[0] > 0)
                setIndexPosition(idxPos[0] - 1, idxPos[1]);
            else {
                setIndexPosition(NUM_COL - 1, idxPos[1]);
            }
            bInxPosUpdated = true;
        }
    }
    // moving right
    if (velocity[0] > 0.0f) {
        if (center[0] - radius * 2.0f >= xFromIdx) {
            if (idxPos[0] < NUM_COL - 1) {
                setIndexPosition(idxPos[0] + 1, idxPos[1]);
            }
            else {
                setIndexPosition(0, idxPos[1]);
            }
            bInxPosUpdated = true;
        }
    }
    // moving up
    if (velocity[1] > 0.0f) {
        if (center[1] - radius * 2.0f >= yFromIdx) {
            if (idxPos[1] > 0)
                setIndexPosition(idxPos[0], idxPos[1] - 1);

            bInxPosUpdated = true;
        }
    }
    // moving down
    else if (velocity[1] < 0.0f) {
        if (center[1] + radius * 2.0f <= yFromIdx) {
            if (idxPos[1] < NUM_ROW - 1)
                setIndexPosition(idxPos[0], idxPos[1] + 1);
           
            bInxPosUpdated = true;
        }
    }
}
