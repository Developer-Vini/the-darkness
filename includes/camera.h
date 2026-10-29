#ifndef CAMERA_H
#define CAMERA_H

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

Vec3 vec3Sub(Vec3 a, Vec3 b);
float vec3Length(Vec3 v);
Vec3 vec3nNormal(Vec3 vz);
Vec3 vec3Cross(Vec3 a, Vec3 b);

#endif