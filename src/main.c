#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "math.h"

#include "../includes/camera.h"
#include "../includes/floor.h"

//configs
#define CAMERA_FOV 45.0f
#define CAMERA_ASPECT 800.0f / 600.0f
#define CAMERA_NEAR 0.1f
#define CAMERA_FAR 100.0f


void moveCharacter(float speed, float deltaTime, Vec3 direction, Camera *camera);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);

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

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 800, 600);

    initFloor(); 

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    Camera camera = {
        .position = {0.0f, 1.5f, 3.0f},
        .front = {0.0f, -0.0f, -1.0f},
        .up = {0.0f, 1.0f, 0.0f},
        .right = {1.0f, 0.0f, 0.0f},
        .pitch = 0.0f,
        .yaw = -90.0f,
        .lastX = 400.0f,
        .lastY = 300.0f,
        .firstMouse = 1,
    };

    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouseCallback);    


    Mat4 view;
    Mat4 projection;

    Mat4 model = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    cameraGetViewMat(&camera, view);
    cameraGetPerspective(CAMERA_FOV, CAMERA_ASPECT, CAMERA_NEAR, CAMERA_FAR, projection);

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)){
        float deltaTime = glfwGetTime();
        float delta = deltaTime - lastFrame;

        lastFrame = deltaTime;
    Vec3 horizontalFront = {camera.front.x, 0.0f, camera.front.z};
    Vec3 normalizedFront = vec3nNormal(horizontalFront);

    Vec3 RightStep = { camera.right.x, 0.0f, camera.right.z};
    Vec3 normalizedRight = vec3nNormal(RightStep);

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

void mouseCallback(GLFWwindow *window, double xpos, double ypos){
    Camera *camera = glfwGetWindowUserPointer(window);
    
    float cameraX;
    float cameraY;
    float cameraZ;

    float sensi = 0.1f;
    
    if(camera->firstMouse){
        camera->lastX = (float)xpos;
        camera->lastY = (float)ypos;
        camera->firstMouse = 0;
    }

    float offsetX = (float)xpos - camera->lastX;
    float offsetY = camera->lastY - (float)ypos;

    camera->lastX = (float)xpos;
    camera->lastY = (float)ypos;    

    offsetX = offsetX * sensi;
    offsetY = offsetY * sensi;
    
    camera->yaw = camera->yaw + offsetX;
    camera->pitch = camera->pitch + offsetY;
    if(camera->pitch >89.0f){
        camera->pitch = 89.0f;
    }
    if(camera->pitch < -89.0f){
        camera->pitch = -89.0f;
    }
    
    float yawRad = camera->yaw * M_PI / 180.0f;
    float pitchRad = camera->pitch * M_PI / 180.0f;    

    cameraX = camera->front.x = cosf(yawRad) * cosf(pitchRad);
    cameraY = camera->front.y = sinf(pitchRad);
    cameraZ = camera->front.z = sinf(yawRad) * cosf(pitchRad);

    Vec3 movimentCamera = {cameraX, cameraY, cameraZ};

    movimentCamera = vec3nNormal(movimentCamera);

    camera->front = movimentCamera;

    cameraUpdateVectors(camera);

}