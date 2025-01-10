#pragma once

#include "Shape2D.h"
#include "Constants.h"
#define GL_SILENCE_DEPRECATION
#include <GL/freeglut.h>
#include <cmath>

class Life {
public:
    Life();
    
    void decreaseLife();
    
    int getLives() const;
    
    bool isDead();

    void draw() const;

private:
    void drawHeart(float x, float y, float size) const;
    
    int remainingLives;
    bool dead;
};
