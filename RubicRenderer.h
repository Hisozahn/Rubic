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

class RubicRenderer {
public:
    RubicRenderer(ShaderProgram* program);
    RubicRenderer(const RubicRenderer& orig) = delete;
    void renderScene();
    void operator=(const RubicRenderer& orig) = delete;
    virtual ~RubicRenderer();
private:
    GLuint VBO;
    GLuint IBO;
    std::unordered_map<std::string, GLuint> uniform_variables;
    ShaderProgram* shader_program;
};

#endif /* RUBICRENDERER_H */

