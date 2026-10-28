// para calcular o comprimento usa o teorema de pitagoras d = raizquadrada(x2 + y2 + z2)
#include <stdio.h>
#include "camera.h"

Vec3 vec3Sub(Vec3 a, Vec3 b){
    Vec3 result;
    
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}
