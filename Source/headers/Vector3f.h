#pragma once

class Vector3f
{
public:
    Vector3f();
    Vector3f(float x, float y, float z);

    void setPos(float x, float y, float z);

    float& operator[](const int i);
    float operator[](const int i) const;
    
private:
    float pos[3];
};

Vector3f operator+(const Vector3f& v1, const Vector3f& v2);
Vector3f operator-(const Vector3f& v1, const Vector3f& v2);
Vector3f operator*(const float s, const Vector3f& vec3);
Vector3f operator/(const Vector3f& vec3, const float s);
float dotProduct(const Vector3f& v1, const Vector3f& v2);
