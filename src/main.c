#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(void){
    if(!glfwInit()){
        printf("Falha ao iniciar\n");
        return 0;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *janela = glfwCreateWindow(800, 600, "Teste", NULL, NULL);

    if(!janela){
        printf("Falha ao criar a janela\n");
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(janela);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("falha ao carregar o opengl \n");
        return 1;
    }

    while(!glfwWindowShouldClose(janela)){
        if(glfwGetKey(janela, GLFW_KEY_SPACE) == GLFW_PRESS){
            glfwSetWindowShouldClose(janela, 1);
        }

            glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(janela);
            glfwPollEvents();
        }
        
    glfwTerminate();
    return 0;
}