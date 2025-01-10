#pragma once

#include "Player.h"

class Pacman : public Player {
public:
    Pacman(float r, bool bCol);
    void setCollided(bool bCol);
    virtual void draw() const;
    virtual void draw_shape() const;

private:
    bool bCollided;
};