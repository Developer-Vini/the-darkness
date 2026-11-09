// as cordenadas sao xx yy e zz
//  VBO - Ele armazena os dados dos vertices na memoria acessivel a GPU. 
//  nesse caso, vou usar ela para guardar os quatro canto desse bixin ai
//  VAO - ja essa, armazena a configuracao de como o opengl vai interpretar os dados dos vrts

#include <stdio.h>
#include <glad/glad.h>
#include "../includes/floor.h"
#include "../includes/camera.h"
#include "../includes/object.h"

#include "../libs/include/stb_image.h"
#include "math.h"

static const float vertices[] = {
    -16.0f, 0.0f, -16.0f,   0.0f, 0.0f,  
     16.0f, 0.0f, -16.0f,   1.0f, 0.0f,
     16.0f, 0.0f,  16.0f,   1.0f, 1.0f,
    -16.0f, 0.0f,  16.0f,   0.0f, 1.0f
};

static unsigned int VBO, VAO; unsigned int shaderProgram, floorTexture;
static GLint modelLoc;
static GLint viewLoc;
static GLint projectionLoc;
static GLint cameraPos;

int valueChunckCalcX;
int valueChunckCalcZ; 
//static Mat4 matrix;

void loadFloorTexture(void){
  int w, h, channels;

  //printf("iniciando\n");

  unsigned char *data = stbi_load("assets/floor.png", &w, &h, &channels, 0);

  printf("terminou\n");
  if(!data){
    //printf("erro ao carregar! %s\n", stbi_failure_reason());
    return;
  }

  //printf("tex carre %dx%d | canais: %d\n", w, h, channels);
  glGenTextures(1, &floorTexture);
  glBindTexture(GL_TEXTURE_2D, floorTexture);
  
  GLenum format = channels == 4 ? GL_RGBA : GL_RGB;

  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);

}

void initFloor(void){
  const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos; \n"
    "layout (location = 1) in vec2 aTexCoord; \n"
    "out vec2 TexCoord; \n"
    "uniform mat4 model;  \n"
    "uniform mat4 view;  \n"
    "uniform mat4 projection; \n"
    "out vec3 FragPos; \n"
    "void main()\n"
    "{\n"
    "  TexCoord = aTexCoord; \n"
    "  vec4 worldPosition = model * vec4(aPos, 1.0);\n"
    "  FragPos = worldPosition.xyz;\n"
    "  gl_Position = projection * view * worldPosition;\n"
    "}\0";

    const char *fragmentShaderSource =
    "#version 330 core\n"
    "in vec3 FragPos;\n"
    "uniform vec3 cameraPos;\n"
    "vec3 fogColor = vec3(0.16, 0.18, 0.20);\n"
    "vec3 floorColor = vec3(1.0, 1.0, 1.0);\n"
    "out vec4 outColor;\n"
    "in vec2 TexCoord;\n"
    "float density = 0.12;\n"
    "uniform sampler2D floorTexture;\n"
    "void main()\n"
    "{\n"
    "  float distancia = distance(FragPos, cameraPos);\n"
    "  float fogFactor = 1.0 - exp(-pow(distancia * density, 2.0));"
    "  vec3 mixColor = texture(floorTexture, TexCoord).rgb * 0.45;\n"
    "  outColor = vec4(mix(mixColor, fogColor, fogFactor), 1.0);\n"
    "}\0";

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);


  loadFloorTexture();
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
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

}

void getChunckPoistion(Camera *cameraPosition, int*chunkX, int *chunkZ){
  *chunkX = (int)floor((cameraPosition->position.x + 16.0f) / 32);
  *chunkZ = (int)floor((cameraPosition->position.z + 16.0f) / 32);

  for(int i = 0; i < 5; i++){
    int x = i;
    x = x - 2;
    valueChunckCalcX = (*chunkX + x) * 32.0f;
    for(int j = 0; j < 5; j++){
      int z = j;
      z = z - 2;
      valueChunckCalcZ = (*chunkZ + z) * 32.0f;
    }
  }
}

void drawFloor(Mat4 model, Mat4 view, Mat4 projection, Vec3 cameraPosition){
  glUseProgram(shaderProgram);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, floorTexture);


  glUniformMatrix4fv(modelLoc, 1, GL_TRUE, &model[0][0]);
  glUniformMatrix4fv(viewLoc, 1, GL_TRUE, &view[0][0]);
  glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, &projection[0][0]);
  GLint textureLoc = glGetUniformLocation(shaderProgram, "floorTexture");
  
  glUniform1i(textureLoc, 0);
  glUniform3f(cameraPos, cameraPosition.x, cameraPosition.y, cameraPosition.z);
  
  int chunckX;
  int chunckZ;
  
  Camera camera;
  camera.position = cameraPosition;
  
  getChunckPoistion(&camera, &chunckX, &chunckZ);
  
  glBindVertexArray(VAO);

  for(int i = 0;  i < 5; i++){
    int x = i - 2;

    float WorX = (chunckX + x) * 32.0f;
    for (int j = 0; j < 5; j++){
      int z = j - 2;
      float WorZ = (chunckZ + z) * 32.0f;

      Vec3 chunkPosition = {
        WorX, 0.0f, WorZ
      };

      Mat4 chunkModel;
      mat4Translate(chunkModel, model, chunkPosition);

      glUniformMatrix4fv(modelLoc, 1, GL_TRUE, &chunkModel[0][0]);

      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
  }
  glBindVertexArray(0);
}


void destroyFloor(void){
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteProgram(shaderProgram);
}
