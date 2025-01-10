#include "Map.h"

// Block
Block::Block() : Shape2D(), width(0), height(0) {}

Block::Block(float x, float y, float z, float w, float h) : Shape2D(x, y, z), width(w), height(h) {}
void Block::setWidth(float w) {
    width = w;
}


void Block::setHeight(float h) {
    height = h;
}
void Block::setType(Block::TYPE t) {
    type = t;
}
void Block::setPosition(float x, float y) {
    center[0] = x;
    center[1] = y;
}


float Block::getWidth() const {
    return width;
}
float Block::getHeight() const {
    return height;
}
Block::TYPE Block::getType() const {
    return type;
}


void Block::draw() const {
    glPushMatrix();

    if (type == WALL) {
        draw_wall();
    }
    else if (type == DOT) {
        draw_dot();
    }
    else if (type == BDOT) {
        draw_bdot();
    }
    else if (type == GATE) {
        draw_gate();
    }
    else
        draw_empty();
}
void Block::draw_wall() const {
    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    glColor3f(0.0f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, -height / 2, 0.0f);
    glVertex3f(width / 2, -height / 2, 0.0f);
    glVertex3f(width / 2, height / 2, 0.0f);
    glVertex3f(-width / 2, height / 2, 0.0f);
    glEnd();
    glPopMatrix();
}
void Block::draw_dot() const {
    const int NUM_SEGMENTS = 12;
    const float radius = 3.0f;

    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    glColor3f(0.55f, 0.4f, 0.4f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);

    for (int i = 0; i <= NUM_SEGMENTS; i++) {
        float angle = i * (2.0f * 3.14 / NUM_SEGMENTS);
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();
}
void Block::draw_bdot() const {
    const int NUM_SEGMENTS = 16;
    const float radius = 8.0f;

    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    glColor3f(0.55f, 0.4f, 0.4f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);

    for (int i = 0; i <= NUM_SEGMENTS; i++) {
        float angle = i * (2.0f * 3.14 / NUM_SEGMENTS);
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();
}
void Block::draw_gate() const {
    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    glColor3f(1.0f, 0.72f, 1.0f);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, -height / 8, 0.0f);
    glVertex3f(width / 2, -height / 8, 0.0f);
    glVertex3f(width / 2, height / 8, 0.0f);
    glVertex3f(-width / 2, height / 8, 0.0f);
    glEnd();
    glPopMatrix();
}
void Block::draw_empty() const {
    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, -height / 2, 0.0f);
    glVertex3f(width / 2, -height / 2, 0.0f);
    glVertex3f(width / 2, height / 2, 0.0f);
    glVertex3f(-width / 2, height / 2, 0.0f);
    glEnd();
    glPopMatrix();
}


// Map
Map::Map(const std::array<std::string, NUM_ROW>& mapData) {
    loadFromArray(mapData);
}


void Map::loadFromArray(const std::array<std::string, NUM_ROW>& mapData) {
    numRows = NUM_ROW;
    numCols = NUM_COL;
    remainingDots = 0;
    
    for (int r = 0; r < numRows; ++r) { 
        const string& currentRow = mapData[r];
        
        for (int c = 0; c < numCols; ++c) {
            blocks[r][c] = Block(0, 0, 0, BLOCK_SIZE, BLOCK_SIZE);
            float x = LEFT_BOUNDARY + c * BLOCK_SIZE;
            float y = TOP_BOUNDARY - r * BLOCK_SIZE;
            blocks[r][c].setPosition(x, y);
            
            char cell = mapData[r][c];
            
            switch(cell) {
                case '#':
                    blocks[r][c].setType(Block::WALL);
                    break;
                case 'n':
                    blocks[r][c].setType(Block::EMPTY);
                    break;
                case 'o':
                    blocks[r][c].setType(Block::DOT);
                    remainingDots++;
                    break;
                case 'b':
                    blocks[r][c].setType(Block::BDOT);
                    remainingDots++;
                    break;
                case 'g':
                    blocks[r][c].setType(Block::GATE);
                    break;
                default:
                    blocks[r][c].setType(Block::EMPTY);
            }
        }
    }
}


const Block& Map::getBlock(int r, int c) const {
    return blocks[r][c];
}
Block& Map::getBlock(int r, int c) {
    return blocks[r][c];
}


int Map::getNumRows() const {
    return numRows;
}
int Map::getNumCols() const {
    return numCols;
}
int Map::getRemainingDots() const {
    return remainingDots;
}


void Map::decreaseDots() {
    remainingDots--;
}


void Map::draw() const {
    for (const auto& row : blocks) {
        for (const auto& block : row) {
            block.draw();
        }
    }
}