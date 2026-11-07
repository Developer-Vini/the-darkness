#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include "camera.h"

//preciso do vao pra guardar info, ebo guarda as vertices e vbo guardar os indice.
//isso aqui eu to fazendo de um tutorial, nao me lembrava mais disso hkakak
typedef struct{
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLint texture;
    unsigned int indexCount;
} Mesh;

//tambem tem a de modelo, pra representar o modelo
typedef struct {
    Mesh* mashes;
    unsigned int meshCount;
} Model;

int modelLoad(Model* model, const char* path);
void modelDraw(const Model* model, GLuint shaderProgram, Mat4 modelMatrix, Mat4 view, Mat4 projection, Vec3 cameraPosition);
void modelDestroy(Model* model);

GLuint initModelShader(void);
#endif