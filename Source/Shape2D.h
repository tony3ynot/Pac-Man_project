#pragma once

#include "Vector3f.h"

class Shape2D {
public:
    Shape2D();
    Shape2D(float x, float y, float z);

    void setCenter(float x, float y, float z);
    void setCenter(const Vector3f& c);
    Vector3f getCenter() const;

    void setVelocity(float x, float y, float z);
    void setVelocity(const Vector3f& v);
    Vector3f getVelocity() const;

    void move();

    virtual void draw() const = 0;

protected:
    Vector3f center;
    Vector3f velocity;
};

