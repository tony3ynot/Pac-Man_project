#include "headers/Pacman.h"

Pacman::Pacman(float r, bool bCol) : Player(r), bCollided(bCol) {}

void Pacman::setCollided(bool bCol) {
    bCollided = bCol;
}


// draw functions
void Pacman::draw() const {
    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    static DIRECTION lastDirection = LEFT;

    if (currDir != NONE) {
        lastDirection = currDir;
    }

    switch (lastDirection) {
    case LEFT:
        break;
    case UP:
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        break;
    case RIGHT:
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        break;
    case DOWN:
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        break;
    default:
        break;
    }
    draw_shape();
    glEnd();
    glPopMatrix();
}
void Pacman::draw_shape() const {
    float x1, y1, x2, y2;
    float angle;
    double rad = radius;

    x1 = 0;
    y1 = 0;
    glColor3f(1, 1, 0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);

    for (angle = -70.0f; angle < 360.0f; angle += 0.2) {
        if (angle < -70.0f || angle > 240.0f)
            continue;
        else {
            x2 = x1 + sin(angle * 3.14 / 180.0) * rad;
            y2 = y1 + cos(angle * 3.14 / 180.0) * rad;
            glVertex2f(x2, y2);
        }
    }
}
