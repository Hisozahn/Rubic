/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShaderProgram.h
 * Author: igor
 *
 * Created on November 12, 2017, 1:06 PM
 */

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <unordered_map>
#include <string>
#include <vector>

class ShaderProgram {
public:
    static const std::vector<std::string> all_uniform_variables;
    void compileShaders();
    void useProgram();
    void addShader(const std::string& pShaderText, GLenum ShaderType);
    void loadFromDirectory(std::string path);
    std::unordered_map<std::string, GLuint> getUniformVariables(std::vector<std::string> names);
    virtual ~ShaderProgram();
    ShaderProgram(const ShaderProgram& orig) = delete;
    void operator=(const ShaderProgram& orig) = delete;
    ShaderProgram();
private:
    GLuint program_handler;
    static ShaderProgram* in_use;
    enum {
        ERROR_MESSAGE_MAX_LENGTH = 1024
    };
};

std::string getFileText(std::string path);
std::string getFileExtension(std::string path);
GLuint getShaderType(std::string path);

#endif /* SHADERPROGRAM_H */

