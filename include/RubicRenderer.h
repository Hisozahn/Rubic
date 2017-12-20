/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RubicRenderer.h
 * Author: igor
 *
 * Created on November 15, 2017, 1:31 PM
 */

#ifndef RUBICRENDERER_H
#define RUBICRENDERER_H

#include "ShaderProgram.h"
#include "RubicController.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

class RubicRenderer {
public:
    RubicRenderer(ShaderProgram& program, const RubicController& rubicController);
    RubicRenderer(const RubicRenderer& orig) = delete;
    void renderScene();
    void operator=(const RubicRenderer& orig) = delete;
    virtual ~RubicRenderer();
private:
    std::vector<glm::vec3> cubePositions = {
        glm::vec3{ 1.0f, 1.0f,  1.0f},
        glm::vec3{ 0.0f, 1.0f,  1.0f},
        glm::vec3{ -1.0f, 1.0f,  1.0f},
        glm::vec3{ 1.0f, 0.0f,  1.0f},
        glm::vec3{ 0.0f, 0.0f,  1.0f},
        glm::vec3{ -1.0f, 0.0f,  1.0f},
        glm::vec3{ 1.0f, -1.0f,  1.0f},
        glm::vec3{ 0.0f, -1.0f,  1.0f},
        glm::vec3{ -1.0f, -1.0f,  1.0f},
        glm::vec3{ 1.0f, 1.0f,  0.0f},
        glm::vec3{ 0.0f, 1.0f,  0.0f},
        glm::vec3{ -1.0f, 1.0f,  0.0f},
        glm::vec3{ 1.0f, 0.0f,  0.0f},
        glm::vec3{ 0.0f, 0.0f,  0.0f},
        glm::vec3{ -1.0f, 0.0f,  0.0f},
        glm::vec3{ 1.0f, -1.0f,  0.0f},
        glm::vec3{ 0.0f, -1.0f,  0.0f},
        glm::vec3{ -1.0f, -1.0f,  0.0f},
        glm::vec3{ 1.0f, 1.0f,  -1.0f},
        glm::vec3{ 0.0f, 1.0f,  -1.0f},
        glm::vec3{ -1.0f, 1.0f,  -1.0f},
        glm::vec3{ 1.0f, 0.0f,  -1.0f},
        glm::vec3{ 0.0f, 0.0f,  -1.0f},
        glm::vec3{ -1.0f, 0.0f,  -1.0f},
        glm::vec3{ 1.0f, -1.0f,  -1.0f},
        glm::vec3{ 0.0f, -1.0f,  -1.0f},
        glm::vec3{ -1.0f, -1.0f,  -1.0f}
    };
    void createVertices();
    void createTextures();
    void createTexture(GLuint* texture, const std::string& path);
    GLuint textureFront;
    GLuint textureRight;
    GLuint textureBack;
    GLuint textureLeft;
    GLuint textureTop;
    GLuint textureBottom;
    GLuint textureInside;
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
    ShaderProgram& shader_program;
    const RubicController& controller;
};

#endif /* RUBICRENDERER_H */

