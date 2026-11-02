#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../includes/camera.h"
#include "../includes/wall.h"

int main(){
    
    
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
    Mat4 a ={
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };

    Mat4 b = {
        {2, 0, 0, 0},
        {0, 2, 0, 0},
        {0, 0, 2, 0},
        {0, 0, 0, 2}
    };

    Mat4 result;

    mat4Multiply(a, b, result);

    int i;
    int k;

    for(i = 0; i < 4; i++){
        for(k = 0; k < 4; k++){
            printf("%8.2f ", result[i][k]);
        }
        printf("\n");
    }

    while (!glfwWindowShouldClose(window)){

        glClearColor(1.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawWall();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroyWall();
    
    glfwTerminate();
    return 0;

}
