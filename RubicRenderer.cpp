/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RubicRenderer.cpp
 * Author: igor
 * 
 * Created on November 15, 2017, 1:31 PM
 */

#include "RubicRenderer.h"
#include "stb_image.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

const glm::vec3 RubicRenderer::lightPosition{20.0f, 18.0f, 20.0f};

void RubicRenderer::createTexture(GLuint* texture, const std::string& path) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        throw std::runtime_error{"Error loading image" + path};
    }
    stbi_image_free(data);
}

void RubicRenderer::createTextures() {
    createTexture(&textureFront, "images/front.jpg");
    createTexture(&textureRight, "images/right.jpg");
    createTexture(&textureBack, "images/back.jpg");
    createTexture(&textureLeft, "images/left.jpg");
    createTexture(&textureTop, "images/top.jpg");
    createTexture(&textureBottom, "images/bottom.jpg");
    createTexture(&textureInside, "images/inside.jpg");
    
}

void RubicRenderer::createVertices() {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    
    float Vertices[] = {
        // Front square
        -1.0f, -1.0f, 1.0f,    0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f,     1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        
        1.0f, -1.0f, 1.0f,     1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,     0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,      1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        // Right-hand square
        1.0f, -1.0f, 1.0f,     0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,    1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,      0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        
        1.0f, -1.0f, -1.0f,    1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,      0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,     1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        // Back-side square
        1.0f, -1.0f, -1.0f,    0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,   1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, -1.0f,     0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        
        -1.0f, -1.0f, -1.0f,   1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, -1.0f,     0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,    1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        // Left-hand square
        -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,    1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,    0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        
        -1.0f, -1.0f, 1.0f,    1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,    0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,     1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        // Top square
        -1.0f, 1.0f, 1.0f,     0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,      1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,     1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        
        -1.0f, 1.0f, 1.0f,     0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,     1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,    0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        // Bottom square
        -1.0f, -1.0f, 1.0f,    0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 1.0f,     1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,    1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
        
        -1.0f, -1.0f, 1.0f,    0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,    1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,  0.0f, -1.0f, 0.0f
    };
    
    glBindVertexArray(cubeVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

RubicRenderer::RubicRenderer(ShaderProgram& cube, const RubicController& rubicController)
    : cubeShader{cube}, controller{rubicController} {
    createVertices();
    createTextures();
}

void RubicRenderer::renderScene() {
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureFront);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureRight);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureBack);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textureLeft);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, textureTop);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, textureBottom);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, textureInside);
    
    cubeShader.useProgram();
    
    cubeShader.setVec3("light.position", lightPosition);
    cubeShader.setVec3("viewPos", controller.getCamera().getPosition());
    // light properties
    cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f); 
    cubeShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
    cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    cubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    cubeShader.setFloat("material.shininess", 64.0f);
    
    glm::mat4 projection = controller.getProjection();
    cubeShader.setFloatMatrix4("projection", projection);  
    glm::mat4 view = controller.getCamera().GetViewMatrix();
    cubeShader.setFloatMatrix4("view", view);
    
    glBindVertexArray(cubeVAO);
    for (glm::mat4 model: controller.getModels()) {
        cubeShader.setFloatMatrix4("model", model);
        cubeShader.setInt("textureFront", 0);
        cubeShader.setInt("textureRight", 1);
        cubeShader.setInt("textureBack", 2);
        cubeShader.setInt("textureLeft", 3);
        cubeShader.setInt("textureTop", 4);
        cubeShader.setInt("textureBottom", 5);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


RubicRenderer::~RubicRenderer() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
}