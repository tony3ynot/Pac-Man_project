#include "headers/Life.h"

Life::Life() : remainingLives(3) {}

void Life::decreaseLife() {
    if (remainingLives > 0) {
        remainingLives--;
        dead = true;
    }
}

int Life::getLives() const {
    return remainingLives;
}

bool Life::isDead() {
    if (dead) {
        dead = false;
        return true;
    }
    return false;
}

void Life::draw() const {
    float size = 10.0f;
    float spacing = 30.0f;
    float startX = -BOUNDARY_X + 40.0f;
    float startY = -BOUNDARY_Y + 40.0f;

    for (int i = 0; i < remainingLives; i++) {
        drawHeart(startX + (i * spacing), startY, size);
    }
}

void Life::drawHeart(float x, float y, float size) const {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);

    for (float angle = 0; angle < 2.0f * 3.14; angle += 0.01f) {
        float t = angle;
        float px = 16 * pow(sin(t), 3);
        float py = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);

        px *= size / 16;
        py *= size / 16;
        glVertex2f(px, py);
    }

    glEnd();
    glPopMatrix();
}
