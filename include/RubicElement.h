/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RubicElement.h
 * Author: igor
 *
 * Created on December 16, 2017, 12:53 PM
 */

#ifndef RUBICELEMENT_H
#define RUBICELEMENT_H

#include "Axis.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>

class RubicElement {
public:
    RubicElement(int id, glm::vec3 position);
    int getId() const { return m_id; }
    glm::mat4 getModel() const { return m_model; }
    //sglm::vec3 getPosition() const;
    glm::vec3 getDefaultPosition() const;
    void rotate(float angle, Axis axis);
    void lock(int movement, Axis axis);
    void updateModel();
private:
    int m_id;
    glm::vec3 m_default_pos;
    glm::vec3 m_default_angles;
    glm::mat4 m_model;
    std::pair<float, Axis> currentRotation;
    std::vector<std::pair<int, Axis> > rotationHistory;
};

#endif /* RUBICELEMENT_H */

