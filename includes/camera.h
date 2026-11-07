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

    float yaw;
    float pitch;
    
    float lastX;
    float lastY;
    int firstMouse;
} Camera;

typedef float Mat4[4][4];


Vec3 vec3Sub(Vec3 a, Vec3 b);
float vec3Length(Vec3 v);
Vec3 vec3nNormal(Vec3 vz);
Vec3 vec3Cross(Vec3 a, Vec3 b);
void cameraUpdateVectors(Camera *camera);
float vec3Dot(Vec3 a, Vec3 b);
void cameraGetViewMat(Camera *camera, Mat4 result);
void cameraGetPerspective(float fov, float aspect, float near, float far, Mat4 result);
void mat4Multiply(Mat4 a, Mat4 b, Mat4 result);
Vec3 vec3Add(Vec3 a, Vec3 b);
Vec3 cameraGetWeaponPosition(Camera *camera, float v1, float v2, float v3);
Vec3 vec3Scal(Vec3 v, float scalar);
void cameraGetRotationMat(Camera *camera, Mat4 result);
void mat4RotateX(Mat4 result, float angle);
#endif