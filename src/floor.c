// as cordenadas sao xx yy e zz
//  VBO - Ele armazena os dados dos vertices na memoria acessivel a GPU. 
//  nesse caso, vou usar ela para guardar os quatro canto desse bixin ai
//  VAO - ja essa, armazena a configuracao de como o opengl vai interpretar os dados dos vrts

#include <stdio.h>
#include <glad/glad.h>
#include "../includes/floor.h"
#include "../includes/camera.h"

static const float vertices[] = {
    -16.0f, 0.0f, -16.0f,
     16.0f, 0.0f, -16.0f,
     16.0f, 0.0f,  16.0f,
    -16.0f, 0.0f,  16.0f
};

static unsigned int VBO, VAO; unsigned int shaderProgram;
static GLint modelLoc;
static GLint viewLoc;
static GLint projectionLoc;
static GLint cameraPos;
//static Mat4 matrix;

void initFloor(void){
  const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos; \n"
    "uniform mat4 model;  \n"
    "uniform mat4 view;  \n"
    "uniform mat4 projection; \n"
    "out vec3 FragPos; \n"
    "void main()\n"
    "{\n"
    "  vec4 worldPosition = model * vec4(aPos, 1.0);\n"
    "  FragPos = worldPosition.xyz;\n"
    "  gl_Position = projection * view * worldPosition;\n"
    "}\0";

    const char *fragmentShaderSource =
    "#version 330 core\n"
    "in vec3 FragPos;\n"
    "uniform vec3 cameraPos;\n"
    "vec3 fogColor = vec3(0.18, 0.18, 0.2);\n"
    "vec3 floorColor = vec3(0.12, 0.14, 0.17);\n"
    "out vec4 outColor;\n"
    "void main()\n"
    "{\n"
    "  float distancia = distance(FragPos, cameraPos);\n"
    "  float fragFactor = smoothstep(5.0, 20.0, distancia);\n"
    "  vec3 mixColor = mix(floorColor, fogColor, distancia);\n"
    "  outColor = vec4(mixColor, 1.0);\n"
    "}\0";

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);

  int sucess;
  char infoLog[512];

  
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);
  if (!sucess){
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR! Compilation failed:\n%s\n", infoLog);
  } 

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &sucess);
  if(!sucess){
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("Fragment shader error:\n%s\n", infoLog);
  }

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
  if(!sucess){
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("shader program error:\n%s\n", infoLog);
  }
  
  modelLoc = glGetUniformLocation(shaderProgram, "model");
  projectionLoc = glGetUniformLocation(shaderProgram, "projection");
  viewLoc = glGetUniformLocation(shaderProgram, "view");
  cameraPos = glGetUniformLocation(shaderProgram, "cameraPos");

  printf("Model: %d\n", modelLoc);
  printf("View: %d\n", viewLoc);
  printf("projection: %d\n", projectionLoc);
  
  //matrix = (Mat4){
  //  {1, 0, 0, 0},
  //  {0, 1, 0, 0},
  //  {0, 0, 1, 0},
  //  {0, 0, 0, 1},
  //};
  
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  
  glEnableVertexAttribArray(0);
}

void drawFloor(Mat4 model, Mat4 view, Mat4 projection, Vec3 cameraPosition){
  glUseProgram(shaderProgram);
  glUniformMatrix4fv(modelLoc, 1, GL_TRUE, &model[0][0]);
  glUniformMatrix4fv(viewLoc, 1, GL_TRUE, &view[0][0]);
  glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, &projection[0][0]);
  glBindVertexArray(VAO);

  glUniform3f(cameraPos, cameraPosition.x, cameraPosition.y, cameraPosition.z);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArray(0);
}


void destroyFloor(void){
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteProgram(shaderProgram);
}