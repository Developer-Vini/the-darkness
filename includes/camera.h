#ifndef CAMERA_H
#define CAMERA_H

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

typedef struct{
    Vec3 position;
    Vec3 front;
    Vec3 up;
    Vec3 right;
} Camera;

typedef float Mat4[4][4];


Vec3 vec3Sub(Vec3 a, Vec3 b);
float vec3Length(Vec3 v);
Vec3 vec3nNormal(Vec3 vz);
Vec3 vec3Cross(Vec3 a, Vec3 b);
void cameraUpdateVectors(Camera *camera);
float vec3Dot(Vec3 a, Vec3 b);
void cameraGetViewMat(Camera *camera, Mat4 result);
#endif