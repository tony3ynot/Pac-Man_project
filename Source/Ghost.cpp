#include "headers/Ghost.h"

Ghost::Ghost(float r, TYPE t, STATE s) : Player(r), type(t), state(s) {}


void Ghost::setColor(const Vector3f& c) {
    color = c;
}
void Ghost::setState(STATE s) {
    state = s;
}
void Ghost::setUsingGate(bool using_gate) {
    usingGate = using_gate;
}


Ghost::TYPE Ghost::getType() const {
    return type;
}
Ghost::STATE Ghost::getState() const {
    return state;
}
int Ghost::getTargetX() const {
    return target[0];
}
int Ghost::getTargetY() const {
    return target[1];
}
std::pair<int, int> Ghost::getScatterTarget() const {
    return scatterTargets[type];
}
bool Ghost::getUsingGate() const {
    return usingGate;
}


// update target based on the state
void Ghost::updateTarget(const Player& pacman, const Ghost* blinky) {
    float xFromIdx = LEFT_BOUNDARY + idxPos[0] * BLOCK_SIZE;
    float yFromIdx = TOP_BOUNDARY - idxPos[1] * BLOCK_SIZE;
    // ensure the ghost is at the center of the grid
    bool isAtGridCenter = 
        abs(center[0] - xFromIdx) < 0.1f && 
        abs(center[1] - yFromIdx) < 0.1f;

    // 1. use gate first
    if (usingGate && state != EATEN) {
        if (isAtGridCenter) {  
            target[0] = scatterTargets[4].first;
            target[1] = scatterTargets[4].second;

            // finish using gate
            if (idxPos[0] == scatterTargets[4].first && idxPos[1] == scatterTargets[4].second) {
                usingGate = false;
            }
        }
        return;
    }

    // 2. set target for each state
    if (isAtGridCenter) { 
        std::pair<int, int> nextTarget;
        switch (state) {
        case CHASE:
            nextTarget = getChaseTarget(pacman, blinky);
            break;    
        case SCATTER:
            nextTarget = getScatterTarget();
            break;    
        case FRIGHTENED:
            nextTarget = std::make_pair(
                rand() % (NUM_COL - 2) + 1,
                rand() % (NUM_ROW - 2) + 1
            );
            break; 
        case EATEN:
            nextTarget = std::make_pair(14, 14);  
            break;
        }
        target[0] = nextTarget.first;
        target[1] = nextTarget.second;
    }

    // 3. if eaten, get back to ghosts' room (14, 14) & scatter
    if (state == EATEN && idxPos[0] == 14 && idxPos[1] == 14) {
        if (isAtGridCenter) {  
            state = SCATTER;
            stateTimer = glutGet(GLUT_ELAPSED_TIME);
        }
        return;
    }
}


// chase targets
std::pair<int, int> Ghost::getChaseTarget(const Player& pacman, const Ghost* blinky) const {
    switch (type) {
    case BLINKY:
        // blinky's target
        return std::make_pair(pacman.getXIndex(), pacman.getYIndex());
        break;
    case PINKY:
        return calculatePinkyTarget(pacman);
        break;
    case INKY:
        return calculateInkyTarget(pacman, blinky);
        break;
    case CLYDE:
        return calculateClydeTarget(pacman);
        break;
    default:
        break;
    }
}
// scatter targets
const std::pair<int, int> Ghost::scatterTargets[5] = {
    {NUM_COL - 2, 1},           // BLINKY  
    {1, 1},                     // PINKY   
    {NUM_COL - 2, NUM_ROW - 2}, // INKY    
    {1, NUM_ROW - 2},           // CLYDE  
    {14, 11}                    // GATE
};
// pinky's target
std::pair<int, int> Ghost::calculatePinkyTarget(const Player& pacman) const {
    int targetX = pacman.getXIndex();
    int targetY = pacman.getYIndex();
    
    switch (pacman.getCurrentDirection()) {
    case Player::UP:
        targetY -= 4;
        break;
    case Player::DOWN:
        targetY += 4;
        break;
    case Player::LEFT:
        targetX -= 4;
        break;
    case Player::RIGHT:
        targetX += 4;
        break;
    }
    
    return std::make_pair(targetX, targetY);
}
// inky's target
std::pair<int, int> Ghost::calculateInkyTarget(const Player& pacman, const Ghost* blinky) const {
    int targetX = pacman.getXIndex();
    int targetY = pacman.getYIndex();

    switch (pacman.getCurrentDirection()) {
    case Player::UP:
        targetY -= 2;
        break;
    case Player::DOWN:
        targetY += 2;
        break;
    case Player::LEFT:
        targetX -= 2;
        break;
    case Player::RIGHT:
        targetX += 2;
        break;
    }

    int vectorX = targetX - blinky->getXIndex();
    int vectorY = targetY - blinky->getYIndex();

    return std::make_pair(targetX + vectorX, targetY + vectorY);
}
// clyde's target
std::pair<int, int> Ghost::calculateClydeTarget(const Player& pacman) const {
    int dx = pacman.getXIndex() - idxPos[0];
    int dy = pacman.getYIndex() - idxPos[1];
    double distance = sqrt(dx*dx + dy*dy);
    
    if (distance > 8.0) {
        return std::make_pair(pacman.getXIndex(), pacman.getYIndex());
    } 
    else {
        return getScatterTarget();  
    }
}


// draw functions
void Ghost::draw() const {
    glPushMatrix();

    if (state == FRIGHTENED) {
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        int elapsedTime = currentTime - stateTimer;
        int remainingTime = 8000 - elapsedTime;

        if (remainingTime <= 3000) { 
            if ((remainingTime / 100) % 2 == 0) {
                glColor3f(1.0f, 1.0f, 1.0f);  
            } else {
                glColor3f(0.0f, 0.0f, 1.0f);  
            }
        } 
        else {
            glColor3f(0.0f, 0.0f, 1.0f);  
        }
    }
    else if (state == EATEN) {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    else {
        glColor3f(color[0], color[1], color[2]);
    }
    glTranslatef(center[0], center[1], center[2]);

    draw_shape();
    glPopMatrix();
}
void Ghost::draw_shape() const {
    float x1, y1, x2, y2;
    float angle;
    x1 = 0;
    y1 = 0;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);

    for (angle = -90.0f; angle < 90.0f; angle += 0.2f) {
        x2 = x1 + sin(angle * 3.14 / 180.0) * radius;
        y2 = y1 + cos(angle * 3.14 / 180.0) * radius;
        glVertex2f(x2, y2);
    }
    glEnd();

    glScalef(radius*2, radius*2, radius*2);
    
    glBegin(GL_POLYGON);
    glVertex2f(-0.5f, 0.0f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(0.5f, -0.25f);
    glVertex2f(-0.5f, -0.25f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(0.25f, -0.25f);
    glVertex2f(0.5f, -0.25f);
    glVertex2f(0.5f, -0.5f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.25f, -0.25f);
    glVertex2f(0.25f, -0.25f);
    glVertex2f(0.0f, -0.5f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.25f, -0.25f);
    glVertex2f(-0.5f, -0.25f);
    glVertex2f(-0.5f, -0.5f);
    glEnd();
}
