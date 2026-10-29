// para calcular o comprimento usa o teorema de pitagoras d = raizquadrada(x2 + y2 + z2)
#include <stdio.h>
#include "../includes/camera.h"
#include <math.h>

Vec3 vec3Sub(Vec3 a, Vec3 b){
    Vec3 result;
    
    result.y = a.y - b.y;
    result.z = a.z - b.z;

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