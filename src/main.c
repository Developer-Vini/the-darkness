#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../includes/camera.h"
#include "../includes/wall.h"
int main(){
    
    int ASPECT = 45.0f;
    int FOV = 1280.0f/720.0f;
    int NEAR = 0.1f;
    int FAR = 100.0f;
    
    if(!glfwInit()){
        printf("error, GLFW was not initialized!");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "tela", NULL, NULL);

    if(!window){
        printf("error, window not initialized");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        printf("failed to initialize GLAD");
        return 1;
    }

    glViewport(0, 0, 800, 600);

    initWall(); 

    
    Camera camera = {
        .position = {0.0f, 1.5f, 3.0f},
        .front = {0.0f, -0.3f, -1.0f},
        .up = {0.0f, 1.0f, 0.0f},
        .right = {1.0f, 0.0f, 0.0f},
    };

    Mat4 model;
    Mat4 view;
    Mat4 projection;


    cameraGetViewMat(&camera, view);
    cameraGetPerspective(FOV, ASPECT, NEAR, FAR, projection);

    while (!glfwWindowShouldClose(window)){

        glClearColor(1.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawWall(model, view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroyWall();
    
    glfwTerminate();
    return 0;

}
