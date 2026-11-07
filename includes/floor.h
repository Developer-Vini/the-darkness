#ifndef FLOOR_H
#define FLOOR_H

#include "camera.h"

extern unsigned int shaderProgram;
void initFloor(void);
void drawFloor(Mat4 model, Mat4 view, Mat4 projection, Vec3 cameraPosition);
void destroyFloor(void);
void getChunckPoistion(Camera *cameraPosition, int*chunkX, int *chunkZ);

#endif