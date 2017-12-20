/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MovingPart.h
 * Author: igor
 *
 * Created on December 17, 2017, 6:19 PM
 */

#ifndef MOVINGPART_H
#define MOVINGPART_H

#include "RubicElement.h"
#include "Axis.h"
#include <vector>
#include <glm/glm.hpp>

class MovingPart {
public:
    MovingPart(int dimension, Axis axis, int index, std::vector<RubicElement*> elements);
    MovingPart(int dimension, Axis axis, int index);
    int rotate(float angle);
    void cancel();
    void print();
    std::vector<RubicElement*> getRow(int rowNum);
    std::vector<RubicElement*> getColumn(int colNum);
    void setRow(int rowNum, std::vector<RubicElement*> row, bool reverse);
    void setColumn(int colNum, std::vector<RubicElement*> col, bool reverse);
    const Axis getAxis() { return axis; } 
    const int getId() { return id; } 
    void turn(int movement);
    bool operator==(const MovingPart& other) const {
        return other.axis.value() == axis.value() 
            && other.id == id 
            && other.dimension == dimension;
    }
private:
    int dimension;
    Axis axis;
    int id;
    std::vector<RubicElement*> elements;
    float angle;
};

#endif /* MOVINGPART_H */

