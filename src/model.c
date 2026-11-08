#include "../includes/model.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//I wrote this code along with a tutorial (it was written in C++, and I had to translate it in my own way). I wrote in English because it's important and I need you all to know, haha.
//It wasn't everything, just that function, but I managed to understand the logic behind it.

GLuint initModelShader(void){
    GLuint modelShaderProgram;
 const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos; \n"
    //"layout (location = 1) in vec3 aNormal; \n"
    "layout (location = 2) in vec2 aTexCoord; \n"
    "uniform mat4 model;  \n"
    "uniform mat4 view;  \n"
    "uniform mat4 projection; \n"
    "out vec2 TexCoord; \n"
    "void main()\n"
    "{\n"
    "  TexCoord = aTexCoord;\n"    
    "  gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "}\0";
 const char *fragmentShaderSource =
    "#version 330 core\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D modelTexture;\n"
    "out vec4 outColor;\n"
    "void main()\n"
    "{\n"
    "  outColor = texture(modelTexture, TexCoord);\n"
    "}\0";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

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

    modelShaderProgram = glCreateProgram();
    glAttachShader(modelShaderProgram, vertexShader);
    glAttachShader(modelShaderProgram, fragmentShader);
    glLinkProgram(modelShaderProgram);

    glGetProgramiv(modelShaderProgram, GL_LINK_STATUS, &sucess);
    if(!sucess){
      glGetProgramInfoLog(modelShaderProgram, 512, NULL, infoLog);
      printf("shader program error:\n%s\n", infoLog);
    }

    //glDeleteShader(modelShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return modelShaderProgram;
}


GLuint loadTexture(const unsigned char* image, int w, int h, int channels){
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D,texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    GLenum format = channels == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;

}

static int processMesh(Mesh* mesh, const struct aiMesh* aiMeshData, const struct aiScene* scene, unsigned int materialIndex){
    const struct aiMaterial* material = scene->mMaterials[materialIndex];
    struct aiString texturePath;

    unsigned int textureIndex;

    aiGetMaterialTexture(material, aiTextureType_DIFFUSE, 0, &texturePath, NULL,  NULL,  NULL,  NULL,  NULL,  NULL);    
    
    if(sscanf(texturePath.data, "*%u", &textureIndex) == 1){
        const struct aiTexture* texture = NULL;
        if(textureIndex < scene->mNumTextures){
            texture = scene->mTextures[textureIndex];
        }

        int w, h, channels;

        unsigned char* image = NULL;
        if(texture != NULL){
            image = stbi_load_from_memory((const unsigned char*)texture->pcData, texture->mWidth, &w, &h, &channels, 0);
        }
    
        if(image){
            mesh->texture = loadTexture(image, w, h, channels);
            stbi_image_free(image);
        }

    }
    
    unsigned int vertexCount = aiMeshData->mNumVertices;


    //printf("osso: %u\n", aiMeshData->mNumBones);

    mesh->boneIDs = calloc(vertexCount * MAX_BONE, sizeof(int));
    mesh->weights = calloc(vertexCount * MAX_BONE, sizeof(float));
    
    if(!mesh->boneIDs || !mesh->weights){
        free(mesh->boneIDs);
        free(mesh->weights);

        mesh->boneIDs = NULL;
        mesh->weights = NULL;

        return 0;
    }

    for(unsigned int i = 0; i < aiMeshData->mNumBones; i++){
        const struct aiBone* bone = aiMeshData->mBones[i];
        for(unsigned int j = 0; j<bone->mNumWeights; j++){
            struct aiVertexWeight weight = bone->mWeights[j];
            unsigned int vertexID = weight.mVertexId;
            
            if(vertexID >= vertexCount){
                continue;
            }

            for(int k = 0; k < MAX_BONE; k++){
                unsigned int index = vertexID * MAX_BONE + k;

                if(mesh->weights[index] == 0.0f){
                    mesh->boneIDs[index] = i;
                    mesh->weights[index] = weight.mWeight;
                    break;
                }
            }
        }
    }

    unsigned int indexCount = 0;

    for(unsigned int i = 0; i < aiMeshData->mNumFaces; i++){
        indexCount = indexCount + aiMeshData->mFaces[i].mNumIndices;
    }

    float* vertices = malloc(vertexCount * 8 * sizeof(float));
    unsigned int* indices = malloc(indexCount * sizeof(unsigned int));

    if(!vertices || !indices){
        free(vertices);
        free(indices);
        return 0;
    }

    for(unsigned int i = 0; i < vertexCount; i++){
        struct aiVector3D position = aiMeshData->mVertices[i];
        struct aiVector3D normal = aiMeshData->mNormals[i];
        struct aiVector3D uv = {0};

        if(aiMeshData->mTextureCoords[0]){
            uv = aiMeshData->mTextureCoords[0][i];
        }


        unsigned int offset = i * 8;

        vertices[offset + 0] = position.x;
        vertices[offset + 1] = position.y;
        vertices[offset + 2] = position.z;

        vertices[offset + 3] = normal.x;
        vertices[offset + 4] = normal.y;
        vertices[offset + 5] = normal.z;
        
        vertices[offset + 6] = uv.x;
        vertices[offset + 7] = uv.y;

    }

    unsigned int currentIndex = 0;

    //aqui veem um loop
    for(unsigned int i = 0; i < aiMeshData->mNumFaces; i++){
        struct aiFace face = aiMeshData->mFaces[i];

        for(unsigned int j = 0; j < face.mNumIndices; j++){
            indices[currentIndex++] = face.mIndices[j];
        }
    }

    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

    glBufferData(GL_ARRAY_BUFFER, vertexCount * 8 * sizeof(float), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
   glEnableVertexAttribArray(1); 

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    free(vertices);
    free(indices);

    mesh->indexCount = indexCount;
    return 1;
}

int modelLoad(Model* model, const char* path){
    memset(model, 0, sizeof(Model));
    
    //printf("carregando: %s\n", path);

    const struct aiScene* scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
    
    if(!scene || !scene->mRootNode){
        printf("erro ao importar: %s\n", aiGetErrorString());
        return 0;
    }


    //printf("anim: %u\n", scene->mNumAnimations);

    //for(unsigned int i = 0; i< scene->mNumAnimations; i++){
    //    const struct aiAnimation* animation = scene->mAnimations[i];
    //    printf("anim %u\n", i);
    //    printf("name %s\n", animation->mName.data);
    //    printf("dura %f\n", animation->mDuration);
    //    printf("tick p segun %f\n", animation->mTicksPerSecond);
    //    printf("chann %u\n", animation->mNumChannels);
    //}

    //printf("Meshes: %u\n", scene->mNumMeshes);    
    //printf("Material: %u\n", scene->mNumMaterials);
    //printf("Textures: %u\n", scene->mNumTextures);

    model->meshCount = scene->mNumMeshes;
    model->mashes = calloc(model->meshCount, sizeof(Mesh));
    
    if(scene->mNumMeshes == 0){
        printf("model sme meh");
        aiReleaseImport(scene);
        return 0;
    }

    if(!model->mashes){
        aiReleaseImport(scene);
        model->meshCount = 0;
        return 0;
    }

    for(unsigned int i = 0; i < model->meshCount; i++){
        if(!processMesh(&model->mashes[i],  scene->mMeshes[i], scene, scene->mMeshes[i]->mMaterialIndex)){
            printf("erro ao carregar mesh %u\n", i);
            aiReleaseImport(scene);
            modelDestroy(model);
            return 0;
        }
       //printf("Mesg %u usa material%u\n", i,  (unsigned int)scene->mMeshes[i]->mMaterialIndex);
    }
    
    for(unsigned int i = 0; i < scene->mNumMaterials; i++){
        struct aiMaterial* material = scene->mMaterials[i];
        struct aiString texturePath;
        enum aiReturn result = aiGetMaterialTexture(material, aiTextureType_DIFFUSE, 0, &texturePath, NULL,  NULL,  NULL,  NULL,  NULL,  NULL);    
    
        if(result == aiReturn_SUCCESS){
            printf("material %u: %s\n", i, texturePath.data);
        }
    }
    

    //const struct aiTexture* texture = scene->mTextures[0];

    //int w, h, channels;

    //unsigned char* image = stbi_load_from_memory(
    //    (const unsigned char*)texture->pcData, texture->mWidth, &w, &h, &channels, 0
    //);

    //if(image){
    //    printf("img crrgd: %d x %d\n", w, h); 
    //    printf("chnls: %d\n", channels); 
    //    stbi_image_free(image);
    //}else{
    //    printf("err: %s\n", stbi_failure_reason());
    //}


    //printf("larg: %u\n", texture->mWidth);
    //printf("alt: %u\n", texture->mHeight);
    //printf("form: %u\n", texture->achFormatHint);

    aiReleaseImport(scene);
    printf("modelo carregado!");
    return 1;

}

void modelDraw(const Model* model, GLuint shaderProgram, Mat4 modelMatrix, Mat4 view, Mat4 projection, Vec3 cameraPosition ){
    
    GLint modelLoc;
    GLint viewLoc;
    GLint projectionLoc;
    GLint cameraPos;
    GLint textureLoc;

    glUseProgram(shaderProgram);

    modelLoc = glGetUniformLocation(shaderProgram, "model");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    projectionLoc =  glGetUniformLocation(shaderProgram, "projection");
    cameraPos = glGetUniformLocation(shaderProgram, "cameraPos");
    textureLoc = glGetUniformLocation(shaderProgram, "modelTexture");
    
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, &modelMatrix[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_TRUE, &view[0][0]);   
    glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, &projection[0][0]);
    
    glUniform1i(textureLoc, 0);

    glUniform3f(cameraPos, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    
    for(unsigned int i = 0; i < model->meshCount; i++){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model->mashes[i].texture);
        glBindVertexArray(model->mashes[i].VAO);
        glDrawElements(GL_TRIANGLES, model->mashes[i].indexCount, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
    
}

void modelDestroy(Model* model){
    for(unsigned int i = 0; i < model->meshCount; i++){
        glDeleteVertexArrays(1, &model->mashes[i].VAO);
        glDeleteBuffers(1, &model->mashes[i].VBO);
        free(model->mashes[i].boneIDs);
        free(model->mashes[i].weights);
        glDeleteBuffers(1, &model->mashes[i].EBO);
    }
    free(model->mashes);

    model->mashes = NULL;
    model->meshCount = 0;
}
