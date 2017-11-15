#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Window.h"
#include "RubicRenderer.h"

static RubicRenderer* renderer_ptr = nullptr;

void renderer_fn_wrapper(){
    if (renderer_ptr != nullptr) {
        renderer_ptr->renderScene();
    }
}

int afterMain(int argc, char** argv) {
    Window::initGlut(argc, argv, GLUT_DOUBLE|GLUT_RGB, 800, 800, 100, 100, "Rubic");
    Window::initGlew();
    
    ShaderProgram* shader_program = new ShaderProgram{};
    shader_program->loadFromDirectory("shaders");
    shader_program->compileShaders();
    shader_program->useProgram();
    
    RubicRenderer renderer(shader_program);
    renderer_ptr = &renderer;
    
    Window::setGlutDisplayCallback(renderer_fn_wrapper);
    Window::setGlutIdleCallback(renderer_fn_wrapper);
    Window::setClearColor({0.0f, 0.0f, 0.0f, 0.0f});
    
    Window::run();
    
    return 0;
}

int main(int argc, char** argv) {
    try {
        return afterMain(argc, argv);
    }
    catch(const std::exception& std_exception) {
        std::cerr << "Uncought exception: " << std_exception.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "Uncought non-standard exception" << std::endl;
        return EXIT_FAILURE;
    }
}