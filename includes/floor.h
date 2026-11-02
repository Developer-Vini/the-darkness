#ifndef FLOOR_H
#define FLOOR_H

#include "camera.h"

void initFloor(void);
void drawFloor(Mat4 model, Mat4 view, Mat4 projection);
void destroyFloor(void);

#endif