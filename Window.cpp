/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Window.cpp
 * Author: igor
 * 
 * Created on November 12, 2017, 5:06 PM
 */

#include "Window.h"
#include "ShaderProgram.h"

void(*Window::idle_callback)() = nullptr;
std::vector<float> Window::clear_colors{};
void(*Window::display_callback)() = nullptr;

bool Window::is_glut_inited = false;
bool Window::is_glew_inited = false;


void Window::setGlutDisplayCallback(void(*callback)()) {
    display_callback = callback;
}
void Window::setGlutIdleCallback(void(*callback)()) {
    idle_callback = callback;
}
void Window::setClearColor(std::vector<float> colors) {
    clear_colors = colors;
}

void Window::initGlut(int argc, char** argv, GLuint mode, int width, int height, int x, int y, std::string title) {
    if (is_glut_inited) {
        throw std::runtime_error{"Glut is already initialized while initializing glut"};
    }
    if (is_glew_inited) {
        throw std::runtime_error{"Glew is already initialized while initializing glut"};
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(mode);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(x, y);
    
    glutCreateWindow(title.c_str());
    is_glut_inited = true;
}

void Window::initGlew() {
    if (!is_glut_inited) {
        throw std::runtime_error{"Glut is not initialized while initializing glew"};
    }
    if (is_glew_inited) {
        throw std::runtime_error{"Glew is already initialized while initializing glew"};
    }
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        throw std::runtime_error{"Error initializing Glew"};
    }
    is_glew_inited = true;
}

void Window::initializeGlutCallbacks() {
    if (display_callback != nullptr) {
        glutDisplayFunc(display_callback);
    }
    if (idle_callback != nullptr) {
        glutIdleFunc(idle_callback);
    }
}

void Window::run() {
    if (!is_glut_inited) {
        throw std::runtime_error{"Glut is not initialized while running window"};
    }
    if (!is_glew_inited) {
        throw std::runtime_error{"Glew is already initialized while running window"};
    }
    initializeGlutCallbacks();
    if (clear_colors.size() < 4) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }
    else {
        glClearColor(clear_colors[0], clear_colors[1], clear_colors[2], clear_colors[3]);
    }
    glutMainLoop();
}

Window::~Window() {
}

