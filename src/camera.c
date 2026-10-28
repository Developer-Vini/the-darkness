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

    sqrt(result);

    printf("result %f", result);

    return result;
}

