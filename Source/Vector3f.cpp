#include "Vector3f.h"

Vector3f::Vector3f(){
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 0.0f;
}
Vector3f::Vector3f(float x, float y, float z){
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}


void Vector3f::setPos(float x, float y, float z){
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}


float& Vector3f::operator[](const int i){
    return pos[i];
}
float Vector3f::operator[](const int i)const{
    return pos[i];
}


Vector3f operator+(const Vector3f& v1, const Vector3f& v2){
    return Vector3f(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

Vector3f operator-(const Vector3f& v1, const Vector3f& v2){
    return Vector3f(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

Vector3f operator*(const float s, const Vector3f& vec3){
    return Vector3f(s * vec3[0], s * vec3[1], s * vec3[2]);
}

Vector3f operator/(const Vector3f& vec3, const float s){
    return Vector3f(vec3[0] / s, vec3[1] / s, vec3[2] / s);
}

float dotProduct(const Vector3f& v1, const Vector3f& v2){
    return (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}
