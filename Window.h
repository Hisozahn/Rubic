/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Window.h
 * Author: igor
 *
 * Created on November 12, 2017, 5:06 PM
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <stdexcept>

#include "ShaderProgram.h"

class Window {
public:
    Window() = delete;
    Window(const Window& orig) = delete;
    void operator=(const Window& orig) = delete;
    virtual ~Window();
    static void setGlutDisplayCallback(void(*callback)());
    static void setGlutIdleCallback(void(*callback)());
    static void setClearColor(std::vector<float> colors);
    static void run();
    static void initGlut(int argc, char** argv, GLuint mode, int width, int height, int x, int y, std::string title);
    static void initGlew();
private:
    static void initializeGlutCallbacks();
    static void(*display_callback)();
    static void(*idle_callback)();
    static std::vector<float> clear_colors;
    static bool is_glut_inited;
    static bool is_glew_inited;
};

#endif /* WINDOW_H */

