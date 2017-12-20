/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Axis.h
 * Author: igor
 *
 * Created on December 17, 2017, 10:37 PM
 */

#ifndef AXIS_H
#define AXIS_H

#include <stdexcept>
#include <glm/glm.hpp>

class Axis {
public:
    enum AxisEnum {X, Y, Z};
    Axis(AxisEnum axis) {
        m_axis = axis;
    }
    static Axis x() { static Axis axis{X}; return axis; }
    static Axis y() { static Axis axis{Y}; return axis; }
    static Axis z() { static Axis axis{Z}; return axis; }
    AxisEnum value() const {
        return m_axis;
    }
    glm::vec3 getVector() const {
        switch(m_axis) {
            case X: return glm::vec3(1.0f, 0.0f, 0.0f);
            case Y: return glm::vec3(0.0f, 1.0f, 0.0f);
            case Z: return glm::vec3(0.0f, 0.0f, 1.0f);
            default: throw std::runtime_error{"Invalid axis"};
        }
    }
private:
    AxisEnum m_axis;
};

#endif /* AXIS_H */

