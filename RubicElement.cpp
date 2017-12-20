/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RubicElement.cpp
 * Author: igor
 * 
 * Created on December 16, 2017, 12:53 PM
 */
#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include "RubicElement.h"


RubicElement::RubicElement(int id, glm::vec3 position) : m_id{id}, m_default_pos{position}, m_default_angles{0.0f, 0.0f, 0.0f}, currentRotation{std::pair<float, Axis>{0.0f,Axis::X}}{
    m_model = glm::translate(glm::mat4(1.0f), position);
}

/*glm::vec3 RubicElement::getPosition() const { 
    
    glm::vec3 position = glm::rotate(m_default_pos, glm::radians(m_default_angles.x), glm::vec3{1.0f, 0.0f, 0.0f});
    position = glm::rotate(position, glm::radians(m_default_angles.y), glm::vec3{0.0f, 1.0f, 0.0f});
    position = glm::rotate(position, glm::radians(m_default_angles.z), glm::vec3{0.0f, 0.0f, 1.0f});

    return position;
}*/

glm::vec3 RubicElement::getDefaultPosition() const {
    return m_default_pos;
}

void RubicElement::rotate(float angle, Axis axis) {
    currentRotation = {angle, axis};
    updateModel();
}

void RubicElement::lock(int movement, Axis axis) {
    currentRotation = {0.0f, Axis::X};
    if (movement != 0) {
        rotationHistory.insert(std::begin(rotationHistory), {movement, axis});
    }
    updateModel();
}

void RubicElement::updateModel() {
    if (currentRotation.first != 0.0f) {
        m_model = glm::rotate(glm::mat4(1.0f), glm::radians(currentRotation.first), currentRotation.second.getVector());
    }
    else {
        m_model = glm::mat4(1.0f);
    }
    for (auto rot : rotationHistory) {
        m_model = glm::rotate(m_model, glm::radians(90.0f * rot.first), rot.second.getVector());
    }
        
    m_model = glm::translate(m_model, m_default_pos);
}

#if 0
void RubicElement::rotate(float angle, Axis axis) {
    glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(m_default_angles.x), glm::vec3{1.0f, 0.0f, 0.0f});
    if (axis.value() == Axis::X) {
        rotX = glm::rotate(rotX, glm::radians(angle), axis.getVector());
    }
    glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(m_default_angles.y), glm::vec3{0.0f, 1.0f, 0.0f});
    if (axis.value() == Axis::Z) {
        rotY = glm::rotate(rotY, glm::radians(angle), axis.getVector());
    }
    glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_default_angles.z), glm::vec3{0.0f, 0.0f, 1.0f});
    if (axis.value() == Axis::Y) {
        rotZ = glm::rotate(rotZ, glm::radians(angle), axis.getVector());
    }
    
    m_model = glm::translate(rotX * rotY * rotZ, m_default_pos);
    /*updateModel();
    m_model = glm::translate(m_model, -m_default_pos);
    std::cerr << "Axis: x: " << axis.getVector().x << "; y: " << axis.getVector().y << "; z: " << axis.getVector().z << std::endl;
    m_model = glm::rotate(m_model, glm::radians(angle), axis.getVector());
    m_model = glm::translate(m_model, m_default_pos);*/
}

void RubicElement::lock(int movement, Axis axis) {
    if (axis.getVector().x != 0) {
        m_default_angles.x += movement * 90.0f;
    }
    if (axis.getVector().y != 0) {
        m_default_angles.y += movement * 90.0f;
    }
    if (axis.getVector().z != 0) {
        m_default_angles.z += movement * 90.0f;
    }
    std::cerr << "Angles, x: " << m_default_angles.x << "; y: " << m_default_angles.y << "; z: " << m_default_angles.z <<  "\n";
    updateModel();
}

void RubicElement::updateModel() {
    m_model = glm::rotate(glm::mat4(1.0f), glm::radians(m_default_angles.x), glm::vec3{1.0f, 0.0f, 0.0f});
    m_model = glm::rotate(m_model, glm::radians(m_default_angles.y), glm::vec3{0.0f, 1.0f, 0.0f});
    m_model = glm::rotate(m_model, glm::radians(m_default_angles.z), glm::vec3{0.0f, 0.0f, 1.0f});
    
    m_model = glm::translate(m_model, m_default_pos);
}
#endif