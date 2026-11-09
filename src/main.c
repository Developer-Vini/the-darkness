#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "math.h"
#include "../includes/camera.h"
#include "../includes/floor.h"
#include "../includes/object.h"
#include "../includes/model.h"

//configs
#define CAMERA_FOV 45.0f
#define CAMERA_ASPECT 800.0f / 600.0f
#define CAMERA_NEAR 0.1f
#define CAMERA_FAR 100.0f
#define MAX_ZOM 20


typedef struct {
    Vec3 position;
    int alive;
} Zombie;

void moveCharacter(float speed, float deltaTime, Vec3 direction, Camera *camera);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
float hitZombie(Vec3 origin, Vec3 dir, Vec3 zombiePos);
void spawnZomb(Zombie *zombies, Vec3 playerPos);

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
    initCube(shaderProgram);
    GLuint modelShaderProgram = initModelShader();

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
    
    
    Mat4 wModel = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    
    Mat4 weaponR;
    Mat4 finalR;
    Mat4 rotation;
    

    cameraGetViewMat(&camera, view);
    cameraGetPerspective(CAMERA_FOV, CAMERA_ASPECT, CAMERA_NEAR, CAMERA_FAR, projection);

    float lastFrame = 0.0f;
    
    Model caixa;
    if(!modelLoad(&caixa, "assets/models/weapon/cod_arma.glb")){
        printf("falha oa carregar");
    }

    Model zumbi;
    srand((unsigned int)time(NULL));

    Zombie zombies[MAX_ZOM] = {0};

    float zumbiSpeed = 3.0f;
    float spawTimer = 0.0f;

    if(!modelLoad(&zumbi, "assets/models/zombie/zombie_crawling.glb")){
        printf("falhou");
    }
    
    float shootTime = 0.0f;
    int score = 0;

    for(int i = 0; i < 3; i++) spawnZomb(zombies, camera.position);

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

        spawTimer -= delta;

        if(spawTimer <= 0.0f){
            spawnZomb(zombies, camera.position);
            spawTimer = 2.0f;
        }
        for(int i = 0; i < MAX_ZOM; i++){
            if(!zombies[i].alive){
                continue;
            }

            Vec3 direction = vec3Sub(camera.position, zombies[i].position);
            direction.y = 0.0f;

            if(vec3Length(direction) > 1.5f){
                direction = vec3nNormal(direction);
                zombies[i].position = vec3Add(zombies[i].position, vec3Scal(direction, zumbiSpeed * delta));
            }
        }


        shootTime -= delta;

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && shootTime <= 0){
            shootTime = 0.25f;

            int i;
            for(i = 0; i < MAX_ZOM; i++){
                if(!zombies[i].alive){
                    continue;
                }
                if(hitZombie(camera.position, camera.front, zombies[i].position) >= 0.0f){
                    zombies[i].alive = 0;
                    score+=1;
                    printf("acertou pontos: %d\n", score);
                    break;
                }
            }
        }



        glClearColor(0.03f, 0.035f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        cameraGetViewMat(&camera, view);
        drawFloor(model, view, projection, camera.position);

        cameraGetRotationMat(&camera, rotation);
        mat4RotateY(weaponR, -90.0f * M_PI / 180.0f);

        mat4Multiply(rotation, weaponR, finalR);
        Vec3 weapon = cameraGetWeaponPosition(&camera, 0.30f, -0.3f, 0.7f);
        mat4Translate(wModel, finalR, weapon);
        
       
        for(int i = 0; i < MAX_ZOM; i++){
            if(!zombies[i].alive){
                continue;
            }

            Vec3 tPlayer = vec3Sub(camera.position, zombies[i].position);

            float yaw = atan2f(tPlayer.x, tPlayer.z);
            float cosseno = cosf(yaw) * 0.01f;
            float seno = sinf(yaw) * 0.01f;

            Mat4 zombiModel = {
                {cosseno, 0, seno, zombies[i].position.x},
                {0,       0.01f, 0, zombies[i].position.y},
                {-seno,   0, cosseno, zombies[i].position.z},
                {0,       0,  0,    1},
            };
            modelDraw(&zumbi, modelShaderProgram, zombiModel, view, projection, camera.position);
        }


        //drawCube(translateModel, view, projection, shaderProgram);
        modelDraw(&caixa, modelShaderProgram, wModel, view, projection, camera.position);
       
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
float hitZombie(Vec3 origin, Vec3 dir, Vec3 zombiePos){
    Vec3 center = {zombiePos.x, zombiePos.y + 1.0f, zombiePos.z};
    Vec3 vectorDistanceCameraZombie = vec3Sub(center, origin);
    
    float vectorDirection = vec3Dot(vectorDistanceCameraZombie, dir);
    
    if(vectorDirection < 0.0f){
        return -1.0f;
    }

    float distanceCameraTwo = vec3Dot(vectorDistanceCameraZombie, vectorDistanceCameraZombie) - vectorDirection * vectorDirection;

    if(distanceCameraTwo > 0.8f * 0.8f){
        return -1.0f;
    }

    return vectorDirection;
}

void spawnZomb(Zombie *zombies, Vec3 playerPos){
    int i;
    for(i = 0; i < MAX_ZOM; i++){
        if(zombies[i].alive){
            continue;
        }
        float angle = ((float)rand() / RAND_MAX) * 6.28f;
        zombies[i].position.x = playerPos.x + cosf(angle) * 18.0f; 
        zombies[i].position.y = 0.0f;
        zombies[i].position.z = playerPos.z + sinf(angle) * 18.0f;
        zombies[i].alive = 1;
        return; 
    }
}