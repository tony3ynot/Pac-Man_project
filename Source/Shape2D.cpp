#include "Shape2D.h"

Shape2D::Shape2D() : center(0.0f, 0.0f, 0.0f), velocity(0.0f, 0.0f, 0.0f) {}

Shape2D::Shape2D(float x, float y, float z) : center(x, y, z), velocity(0.0f, 0.0f, 0.0f) {}


void Shape2D::setCenter(float x, float y, float z) {
    center = Vector3f(x, y, z);
}
void Shape2D::setCenter(const Vector3f& c) {
    center = c;
}
Vector3f Shape2D::getCenter() const {
    return center;
}


void Shape2D::setVelocity(float x, float y, float z) {
    velocity = Vector3f(x, y, z);
}
void Shape2D::setVelocity(const Vector3f& v) {
    velocity = v;
}
Vector3f Shape2D::getVelocity() const {
    return velocity;
}


void Shape2D::move() {
    center = center + velocity;
}
