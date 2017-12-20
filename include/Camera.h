#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <vector>

// Default camera values
const float SENSITIVTY =  0.5f;
const float ZOOM       =  45.0f;
const float RADIUS     =  7.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Constructor with vectors
    Camera(glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : MouseSensitivity(SENSITIVTY), Zoom(ZOOM), Radius(RADIUS)
    {
        Position = glm::vec3(0.0f, 0.0f, Radius);
        WorldUp = up;
    }
    
    Camera(float radius, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : MouseSensitivity(SENSITIVTY), Zoom(ZOOM), Radius(radius)
    {
        Position = glm::vec3(0.0f, 0.0f, Radius);
        WorldUp = up;
    }
    

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(Position, glm::vec3{0,0,0}, WorldUp);//glm::vec3{Up.x, Up.y, z});
    }
    
    float GetZoom() const {
        return Zoom;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        static float x = 90.0f;
        static float y = 0.0f;
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
        x += xoffset;
        y -= yoffset;
        
        if (y > 70.0f)
            y = 70.0f;
        if (y < -70.0f)
            y = -70.0f;
        
        Position.x = cos(glm::radians(x)) * cos(glm::radians(y)) * Radius;
        Position.y = sin(glm::radians(y)) * Radius;
        Position.z = sin(glm::radians(x)) * cos(glm::radians(y)) * Radius;
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }
    glm::vec3 getPosition() const { return Position; }
    float getRadius() const { return Radius; }
    glm::mat4 getProjection(float ratio) const {
        return glm::perspective(glm::radians(Zoom), ratio, nearPlane, farPlane);
    }
    std::string getSide() {
        const std::vector<std::pair<std::string, glm::vec3> > orientations = {
            {"Front", glm::vec3 {0.0f, 0.0f, 1.0f}},
            {"Right", glm::vec3 {1.0f, 0.0f, 0.0f}},
            {"Back", glm::vec3 {0.0f, 0.0f, -1.0f}},
            {"Left", glm::vec3 {-1.0f, 0.0f, 0.0f}},
            {"Top", glm::vec3 {0.0f, 1.0f, 0.0f}},
            {"Bottom", glm::vec3 {0.0f, -1.0f, 0.0f}}
        };
        glm::vec3 cPos = glm::normalize(Position);
        float minDistance = 2.0f;
        std::string side;
        for (auto orient : orientations) {
            float distance = glm::distance(orient.second, cPos);
            if (distance < minDistance) {
                minDistance = distance;
                side = orient.first;
            }
        }
        return side;
    }

private:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 WorldUp;
    // Camera options
    float MouseSensitivity;
    float Zoom;
    float Radius;
    static constexpr float nearPlane = 0.1f;
    static constexpr float farPlane = 45.0f;
};
#endif