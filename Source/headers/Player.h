#pragma once

#include "Shape2D.h"
#include "Constants.h"
#define GL_SILENCE_DEPRECATION
#include <GL/freeglut.h>
#include <math.h>

class Player : public Shape2D {
public:
    enum DIRECTION { NONE, LEFT, UP, RIGHT, DOWN };

    Player(float r);

    void setIndexPosition(int x, int y);
    bool isIndexPositionUpdated() const;
    int getXIndex() const;
    int getYIndex() const;
    void setRadius(float r);
    float getRadius() const;

    void setCurrentDirection(DIRECTION d);
    void setNextDirection(DIRECTION d);
    DIRECTION getCurrentDirection() const;
    DIRECTION getNextDirection() const;
    void updateDirection();

    void move();

    virtual void draw() const = 0;
    virtual void draw_shape() const = 0;

protected:
    float radius;
    int idxPos[2];
    bool bInxPosUpdated;

    DIRECTION currDir;
    DIRECTION nextDir;
};