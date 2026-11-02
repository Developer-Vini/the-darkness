#ifndef WALL_H
#define WALL_H
#include "camera.h"

void initWall(void);
void drawWall(Mat4 model, Mat4 view, Mat4 projection);
void destroyWall(void);

#endif