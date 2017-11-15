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
#include "ShaderProgram.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

using namespace glm;

RubicRenderer::RubicRenderer(ShaderProgram* program) {
    shader_program = program;
    uniform_variables = shader_program->getUniformVariables(ShaderProgram::all_uniform_variables);
    
    vec3 Vertices[4];
    Vertices[0] = vec3(-1.0f, -1.0f, 0.0f);
    Vertices[1] = vec3(0.0f, -1.0f, 1.0f);
    Vertices[2] = vec3(1.0f, -1.0f, 0.0f);
    Vertices[3] = vec3(0.0f, 1.0f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    unsigned int Indices[] = { 0, 3, 1,
                               1, 3, 2,
                               2, 3, 0,
                               0, 1, 2 };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

void RubicRenderer::renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    static GLuint gWorldLocation = uniform_variables["gWorld"];
    static float scale = 0.0f;
    static mat4 world = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    
    scale += 0.001f;
    world[0][0]=cosf(scale); 
    world[0][2]=-sinf(scale);
    world[2][0]=sinf(scale); 
    world[2][2]=cosf(scale);
    /*world[0][0]=sinf(scale);
    world[1][1]=sinf(scale);
    world[2][2]=sinf(scale);*/
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &world[0][0]);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);

    glutSwapBuffers(); 
}

RubicRenderer::~RubicRenderer() {
    if (shader_program != nullptr) {
        delete shader_program;
    }
    glDeleteBuffers(1, &VBO);
}

