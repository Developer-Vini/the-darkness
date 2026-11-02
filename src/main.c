#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../includes/camera.h"
#include "../includes/floor.h"
int main(){
    
    float FOV = 45.0f;
    float ASPECT = 800.0f/600.0f;
    float NEAR = 0.1f;
    float FAR = 100.0f;
    
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

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 800, 600);

    initFloor(); 

    
    Camera camera = {
        .position = {0.0f, 1.5f, 3.0f},
        .front = {0.0f, -0.3f, -1.0f},
        .up = {0.0f, 1.0f, 0.0f},
        .right = {1.0f, 0.0f, 0.0f},
    };

    Mat4 view;
    Mat4 projection;

    Mat4 model = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };


    cameraGetViewMat(&camera, view);
    cameraGetPerspective(FOV, ASPECT, NEAR, FAR, projection);

    while (!glfwWindowShouldClose(window)){

        glClearColor(0.02f, 0.02f, 0.025f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawFloor(model, view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroyFloor();
    
    glfwTerminate();
    return 0;

}
