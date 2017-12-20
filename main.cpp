#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"
#include "RubicRenderer.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

// Create game controller
static RubicController controller(3, 2.0f, 0.2f, 15.0f, SCR_WIDTH, SCR_HEIGHT);

// timing
static float deltaTime = 0.0f;	// time between current frame and last frame
static float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rubic", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //glfwSetKeyCallback(window, key_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    {
        // build and compile our shader program
        ShaderProgram ourShader("shaders");
        
        // Setup cube renderer
        RubicRenderer renderer(ourShader, controller);

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // per-frame time logic
            // --------------------
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            // input
            // -----
            processInput(window);

            // clear the scene
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // render the cube
            renderer.renderScene();
            
            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } // ShaderProgram and RubicRenderer will have been cleaned up by this moment

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    controller.updateRatio(width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static bool lastLeftPressed = false;
    static float lastLeftX;
    static float lastLeftY;
    
    static bool lastRightPressed = false;
    static float lastRightX;
    static float lastRightY;
    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (lastLeftPressed) {
            float xoffset = xpos - lastLeftX;
            float yoffset = lastLeftY - ypos; // reversed since y-coordinates go from bottom to top
            lastLeftX = xpos;
            lastLeftY = ypos;
            controller.ProcessCameraMovement(xoffset, yoffset);
        }
        lastLeftX = xpos;
        lastLeftY = ypos;
        
        lastLeftPressed = true;
    }
    else {
        lastLeftPressed = false;
    }
    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (lastRightPressed) {
            float xoffset = xpos - lastRightX;
            float yoffset = lastRightY - ypos; // reversed since y-coordinates go from bottom to top
            lastRightX = xpos;
            lastRightY = ypos;
            controller.ProcessModelMovement(xoffset, yoffset);
        }
        lastRightX = xpos;
        lastRightY = ypos;
        
        lastRightPressed = true;
    }
    else {
        lastRightPressed = false;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            double xpos;
            double ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            controller.ProcessRMBPress(xpos, ypos);
        }
        else if (action == GLFW_RELEASE) {
            controller.ProcessRMBRelease();
        }
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    controller.ProcessMouseScroll(yoffset);
}

/*void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static int id = 0;
    static Axis axis = Axis::x();
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_0: 
                id = 0;
                break;
            case GLFW_KEY_1: 
                id = 1;
                break;
            case GLFW_KEY_2: 
                id = 2;
                break;
            case GLFW_KEY_X:
                axis = Axis::x();
                break;
            case GLFW_KEY_Y:
                axis = Axis::y();
                break;        
            case GLFW_KEY_Z:
                std::cout << "z\n";
                axis = Axis::z();
                break;
            default: break;
        }
    }
    controller.setActivePart(axis, id);
}*/