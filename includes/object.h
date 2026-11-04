#ifndef OBJECT_H
#define OBJECT_H

#include "camera.h"
#include "floor.h"

void initCube(unsigned int shaderProgram);
void drawCube(Mat4 model, Mat4 view, Mat4 projection,unsigned int shaderProgram);
#endif