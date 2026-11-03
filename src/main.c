#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../includes/camera.h"
#include "../includes/floor.h"

void moveCharacter(float speed, float deltaTime, Vec3 direction, Camera *camera);

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


    Vec3 horizontalFront = {camera.front.x, 0.0f, camera.front.z};
    Vec3 normalizedFront = vec3nNormal(horizontalFront);

    Vec3 RightStep = { camera.right.x, 0.0f, 0.0f};
    Vec3 normalizedRight = vec3nNormal(RightStep);

    cameraGetViewMat(&camera, view);
    cameraGetPerspective(FOV, ASPECT, NEAR, FAR, projection);

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)){
        float deltaTime = glfwGetTime();
        float delta = deltaTime - lastFrame;

        lastFrame = deltaTime;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        moveCharacter(3.0f, delta, normalizedFront, &camera);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        Vec3 backwardFront = vec3Scal(normalizedFront, -1.0f);
        moveCharacter(3.0f, delta, backwardFront, &camera);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        Vec3 rightInver = vec3Scal(normalizedRight, -1);
        moveCharacter(3.0f, delta, rightInver, &camera);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        moveCharacter(3.0f, delta, normalizedRight, &camera);
    }


        glClearColor(0.02f, 0.02f, 0.025f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        cameraGetViewMat(&camera, view);
        drawFloor(model, view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroyFloor();
    
    glfwTerminate();
    return 0;

}

void moveCharacter(float speed, float deltaTime, Vec3 direction, Camera *camera){
    float disctance = speed * deltaTime;
    Vec3 mult = vec3Scal(direction, disctance);
    Vec3 positioUpdate = vec3Add(camera->position, mult);
    camera->position = positioUpdate;
}
