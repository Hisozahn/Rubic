/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShaderProgram.cpp
 * Author: igor
 * 
 * Created on November 12, 2017, 1:06 PM
 */

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <map>
#include <fstream>
#include <dirent.h>
#include <cstring>
#include <iostream>

#include "ShaderProgram.h"

const std::vector<std::string> ShaderProgram::all_uniform_variables = {"gWorld"};
ShaderProgram* ShaderProgram::in_use = nullptr;

ShaderProgram::ShaderProgram(const std::string &directory) {
    program_handler = glCreateProgram();
    
    if (program_handler == 0) {
        throw std::runtime_error{"Error creating shader program"};
    }
    
    loadFromDirectory(directory);
    
    compileShaders();
}

void ShaderProgram::addShader(const std::string& pShaderText, GLenum ShaderType) {
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        throw std::runtime_error{"Error creating shader type " + std::to_string(ShaderType)};
    }
    const GLchar* p[1];
    p[0] = pShaderText.c_str();
    glShaderSource(ShaderObj, 1, p, NULL);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[ERROR_MESSAGE_MAX_LENGTH];
        glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
        throw std::runtime_error{"Error compiling shader type " + std::to_string(ShaderType) + ":" + InfoLog};
    }

    glAttachShader(program_handler, ShaderObj);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error{"Error attaching shader type " + std::to_string(ShaderType)};
    }
    
    shader_objects.push_back(ShaderObj);
}

std::string getFileExtension(std::string path) {
    size_t i = path.rfind('.', path.length());
    if (i != std::string::npos) {
        return(path.substr(i+1, path.length() - i));
    }
    return "";
}

GLuint getShaderType(std::string path) {
    std::string extension = getFileExtension(path);
    
    if (extension == "vert") {
        return GL_VERTEX_SHADER;
    }
    else if (extension == "geom") {
        return GL_GEOMETRY_SHADER;
    }
    else if (extension == "frag") {
        return GL_FRAGMENT_SHADER;
    }
    else if (extension == "comp") {
        return GL_COMPUTE_SHADER;
    }
    else if (extension == "tesc") {
        return GL_TESS_CONTROL_SHADER;
    }
    else if (extension == "tese") {
        return GL_TESS_EVALUATION_SHADER;
    }
    else {
        throw std::runtime_error{"Error parsing shader type: " + path};
    }
    
}

std::string getFileText(std::string path) {
    std::ifstream shader_stream(path);
    std::string file_text((std::istreambuf_iterator<char>(shader_stream)),
                 std::istreambuf_iterator<char>());
    shader_stream.close();
    return file_text;
}

void ShaderProgram::setFloatMatrix4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(program_handler, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(program_handler, name.c_str()), value);
}

void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(program_handler, name.c_str()), x, y, z);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(program_handler, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setFloat(const std::string &name, float value) { 
    glUniform1f(glGetUniformLocation(program_handler, name.c_str()), value); 
}

void ShaderProgram::loadFromDirectory(const std::string &path) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
        try {
            while ((ent = readdir (dir)) != NULL) {
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                    continue;
                }
                GLuint shader_type = getShaderType(ent->d_name);
                std::string shader_text = getFileText(path + "/" + ent->d_name);
                addShader(shader_text, shader_type);
            }
        }
        catch (...) {
            closedir (dir);
            throw;
        }
    } else {
        throw std::runtime_error{"Error opening shader directory"};
    }
}

void ShaderProgram::compileShaders() {
    GLint success = 0;
    GLchar ErrorLog[ERROR_MESSAGE_MAX_LENGTH] = { 0 };

    glLinkProgram(program_handler);
    glGetProgramiv(program_handler, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_handler, sizeof(ErrorLog), NULL, ErrorLog);
        throw std::runtime_error{std::string{"Error linking shader program: "} + ErrorLog};
    }

    glValidateProgram(program_handler);
    glGetProgramiv(program_handler, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_handler, sizeof(ErrorLog), NULL, ErrorLog);
        throw std::runtime_error{std::string{"Invalid shader program: "} + ErrorLog};
    }
    for (GLuint shader : shader_objects) {
        glDeleteShader(shader);
    }
    shader_objects.clear();
}

void ShaderProgram::useProgram() {
    glUseProgram(program_handler);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << err << std::endl;
        throw std::runtime_error{"Error enabling shader program."};
    }
    in_use = this;
}

std::unordered_map<std::string, GLuint> ShaderProgram::getUniformVariables(std::vector<std::string> names) {
    if (in_use != this) {
        throw std::runtime_error{"Shader program is not in use."};
    }
    std::unordered_map<std::string, GLuint> variables;
    for (std::string name : names) {
        GLuint location = glGetUniformLocation(program_handler, name.c_str());
        if (location == (GLuint)-1) {
            throw std::runtime_error{"Can not find variable :" + name};
        }
        variables.insert({name, location});
    }
    return variables;
}

ShaderProgram::~ShaderProgram() {
    if (in_use == this) {
        glUseProgram(0);
        in_use = nullptr;
    }
    glDeleteProgram(program_handler);
}