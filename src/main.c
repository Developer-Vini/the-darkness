#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
