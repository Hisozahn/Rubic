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
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    
    float Vertices[] = {
        // Front square
        -1.0f, -1.0f, 1.0f,    0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        
        1.0f, -1.0f, 1.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,     0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,      1.0f, 1.0f,
        // Right-hand square
        1.0f, -1.0f, 1.0f,     0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,      0.0f, 1.0f,
        
        1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,      0.0f, 1.0f,
        1.0f, 1.0f, -1.0f,     1.0f, 1.0f,
        // Back-side square
        1.0f, -1.0f, -1.0f,    0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,   1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,     0.0f, 1.0f,
        
        -1.0f, -1.0f, -1.0f,   1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,     0.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,    1.0f, 1.0f,
        // Left-hand square
        -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,    1.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,    0.0f, 1.0f,
        
        -1.0f, -1.0f, 1.0f,    1.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,    0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,     1.0f, 1.0f,
        // Top square
        -1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,      1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,     1.0f, 1.0f,
        
        -1.0f, 1.0f, 1.0f,     0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,     1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,    0.0f, 1.0f,
        // Bottom square
        -1.0f, -1.0f, 1.0f,    0.0f, 1.0f,
        1.0f, -1.0f, 1.0f,     1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
        
        -1.0f, -1.0f, 1.0f,    0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,
    };
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

RubicRenderer::RubicRenderer(ShaderProgram& program, const RubicController& rubicController)
    : shader_program{program}, controller{rubicController} {
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
    
    shader_program.useProgram();
    
    glm::mat4 projection = controller.getProjection(); //glm::perspective(glm::radians(controller.getCamera().GetZoom()), 1.33f, 0.1f, 100.0f);
    shader_program.setFloatMatrix4("projection", projection);  
    glm::mat4 view = controller.getCamera().GetViewMatrix();
    shader_program.setFloatMatrix4("view", view);
    
    glBindVertexArray(VAO);
    for (glm::mat4 model: controller.getModels()) {
        shader_program.setFloatMatrix4("model", model);
        shader_program.setInt("textureFront", 0);
        shader_program.setInt("textureRight", 1);
        shader_program.setInt("textureBack", 2);
        shader_program.setInt("textureLeft", 3);
        shader_program.setInt("textureTop", 4);
        shader_program.setInt("textureBottom", 5);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


RubicRenderer::~RubicRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}