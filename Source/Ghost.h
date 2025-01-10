#pragma once

#include "Player.h"
#include <iostream>

class Ghost : public Player {
public:
    enum TYPE { BLINKY, PINKY, INKY, CLYDE };
    enum STATE { CHASE, SCATTER, FRIGHTENED, EATEN };

    Ghost(float r, TYPE t, STATE s);
    void setColor(const Vector3f& c);
    void setState(STATE s);
    void setUsingGate(bool using_gate);

    TYPE getType() const;
    STATE getState() const;
    int getTargetX() const;
    int getTargetY() const;
    std::pair<int, int> getScatterTarget() const;
    bool getUsingGate() const;

    void updateTarget(const Player& pacman, const Ghost* blinky = nullptr);

    virtual void draw() const;
    virtual void draw_shape() const;

    int stateTimer = 0;

private:
    TYPE type;
    STATE state;
    Vector3f color;
    int target[2];
    static const std::pair<int, int> scatterTargets[5];
    bool usingGate = true;

    std::pair<int, int> getChaseTarget(const Player& pacman, const Ghost* blinky) const;
    std::pair<int, int> calculatePinkyTarget(const Player& pacman) const;
    std::pair<int, int> calculateInkyTarget(const Player& pacman, const Ghost* blinky) const;
    std::pair<int, int> calculateClydeTarget(const Player& pacman) const;
};
