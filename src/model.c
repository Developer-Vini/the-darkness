#include "../includes/model.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//I wrote this code along with a tutorial (it was written in C++, and I had to translate it in my own way). I wrote in English because it's important and I need you all to know, haha.
//It wasn't everything, just that function, but I managed to understand the logic behind it.
static int processMesh(Mesh* mesh, const struct aiMesh* aiMeshData){
    unsigned int vertexCount = aiMeshData->mNumVertices;

    unsigned int indexCount;


    for(unsigned int i = 0; i < aiMeshData->mNumFaces; i++){
        indexCount = indexCount + aiMeshData->mFaces[i].mNumIndices;
    }

    float* vertices = malloc(vertexCount * sizeof(float));
    unsigned int* indices = malloc(indexCount * sizeof(unsigned int));

    if(!vertices || !indices){
        free(vertices);
        free(indices);
        return 1;
    }

    for(unsigned int i = 0; i < vertexCount; i++){
        struct aiVector3D position = aiMeshData->mVertices[i];
        struct aiVector3D normal = aiMeshData->mNormals[i];
        struct aiVector3D uv = {0};

        if(!aiMeshData->mTextureCoords[0]){
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
        vertices[offset + 6] = uv.y;

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
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    free(vertices);
    free(indices);

    mesh->indexCount = indexCount;
    return 1;
}