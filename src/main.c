#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

float cameraX = 0.0f;
float cameraY = 1.6f;
float cameraZ = 3.0f;

float yaw = -90.0f;
float pitch = 0.0f;

float lastMouseX = 400.0f;
float lastMouseY = 300.0f;
int firstMouse = 1;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(void){
    if(!glfwInit()){
        printf("error, GLFW was not initialized!");
    }

    return 0;
}