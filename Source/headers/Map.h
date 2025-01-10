#pragma once

#include "Pacman.h"
#include "Constants.h"
#include <array>
#define GL_SILENCE_DEPRECATION
#include <GL/freeglut.h>
#include <iostream>
using namespace std;

class Block : public Shape2D
{
public:
    enum TYPE { WALL, DOT, BDOT, GATE, EMPTY };

    Block();
    Block(float x, float y, float z, float w, float h);

    void setWidth(float w);
    void setHeight(float h);
    void setType(TYPE t);
    void setPosition(float x, float y);

    float getWidth() const;
    float getHeight() const;
    TYPE getType() const;

    virtual void draw() const;
    void draw_wall() const;
    void draw_dot() const;
    void draw_bdot() const;
    void draw_gate() const;
    void draw_empty() const;

private:
    float width, height;
    TYPE type;
};


class Map
{
public:
    Map(const std::array<std::string, NUM_ROW>& mapData);
    void loadFromArray(const std::array<std::string, NUM_ROW>& mapData);

    const Block& getBlock(int r, int c) const;
    Block& getBlock(int r, int c);
    int getNumRows() const;
    int getNumCols() const;
    int getRemainingDots() const;
    
    void decreaseDots();

    void draw() const;

private:
    std::array<std::array<Block, NUM_COL>, NUM_ROW>  blocks;
    int numRows = 0;
    int numCols = 0;
    int remainingDots;
};