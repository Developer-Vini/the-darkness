#include <stdio.h>
#include "../includes/camera.h"
#include <math.h>

Vec3 vec3Sub(Vec3 a, Vec3 b){
    Vec3 result;
    
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.x = a.x - b.x;

    return result;
}

float vec3Length(Vec3 v){
    float result;

    float x = v.x;
    float y = v.y;
    float z = v.z;

    result = x*x + y*y + z*z;

    result = sqrtf(result);

    return result;
}

Vec3 vec3nNormal(Vec3 vz){

    float vetorX;
    float vetorY;
    float vetorZ;
    
    float lengthVetor = vec3Length(vz); 
    
    if(lengthVetor == 0){
        printf("nao e possivel dividir por zero");
        Vec3 zero = { 0.0f, 0.0f, 0.0f};
        return zero;
    };

    vetorX = vz.x / lengthVetor;
    vetorY = vz.y / lengthVetor;
    vetorZ = vz.z / lengthVetor;

    Vec3 vetoresNormalizados = {vetorX, vetorY, vetorZ};
    
    return vetoresNormalizados;

}

Vec3 vec3Cross(Vec3 a, Vec3 b){
    Vec3 result;

    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);

    return result;
}   

void cameraUpdateVectors(Camera *camera){
    Vec3 rigthC;
    Vec3 Up;

    rigthC = vec3Cross(camera->front, camera->up);
    Vec3 valueNormal = vec3nNormal(rigthC);
    camera->right = valueNormal;
    

    Up = vec3Cross(camera->right, camera->front);
    Vec3 normalizedValue = vec3nNormal(Up);
    camera->up = normalizedValue;
    
    //printf("value normal: %f\n", valueNormal.x);
    //printf("value normal: %f\n", valueNormal.y);
    //printf("value normal: %f\n", valueNormal.z);
}

float vec3Dot(Vec3 a, Vec3 b){
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void cameraGetViewMat(Camera *camera, Mat4 result){
    result[0][0] = camera->right.x;
    result[0][1] = camera->right.y;
    result[0][2] = camera->right.z;
    result[0][3] = -vec3Dot(camera->right, camera->position);

    result[1][0] = camera->up.x;
    result[1][1] = camera->up.y;
    result[1][2] = camera->up.z;
    result[1][3] = -vec3Dot(camera->up, camera->position);

    result[2][0] = -camera->front.x;
    result[2][1] = -camera->front.y;
    result[2][2] = -camera->front.z;
    result[2][3] =  vec3Dot(camera->front, camera->position);

    result[3][0] = 0.0f;
    result[3][1] = 0.0f;
    result[3][2] = 0.0f;
    result[3][3] = 1.0f;
}

void cameraGetPerspective(float fov, float aspect, float near, float far, Mat4 result){
    float angle = 180;
    float PI = M_PI;

    float radian = fov * (PI / angle);
    float calcRadian = radian / 2;
    float f = 1 / (tanf(calcRadian));

    float calcFx = f / aspect;
    float fX = calcFx;


    float A;
    float B;
    A = -(far + near) / (far - near);
    B = -2 * (far * near) / (far - near);

    result[0][0] = fX;
    result[0][1] = 0;
    result[0][2] = 0;
    result[0][3] = 0;

    result[1][0] = 0;
    result[1][1] = f;
    result[1][2] = 0;
    result[1][3] = 0;

    result[2][0] = 0;
    result[2][1] = 0;
    result[2][2] = A;
    result[2][3] = B;

    result[3][0] = 0;
    result[3][1] = 0;
    result[3][2] = -1;
    result[3][3] = 0;
}
