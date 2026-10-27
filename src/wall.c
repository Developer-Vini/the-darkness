// as cordenadas sao xx yy e zz
//  VBO - Ele armazena os dados dos vertices na memoria acessivel a GPU. 
//  nesse caso, vou usar ela para guardar os quatro canto desse bixin ai
//  VAO - ja essa, armazena a configuracao de como o opengl vai interpretar os dados dos vrts

#include <stdio.h>
#include <glad/glad.h>
#include "wall.h"

static const float vertices[] = {
    -0.5f, 0.0f, -0.5f,
    0.5f, 0.0f, -0.5f,
    0.5f, 0.0f, 0.5f,
    -0.5f, 0.0f, 0.5f
};

static unsigned int VBO, VAO, shaderProgram;

void initWall(void){
  const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos; \n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 outColor;\n"
    "void main()\n"
    "{\n"
    "  outColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
    glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
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

void drawWall(void){
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}


void destroyWall(void){
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteProgram(shaderProgram);
}