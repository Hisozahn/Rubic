/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MovingPart.cpp
 * Author: igor
 * 
 * Created on December 17, 2017, 6:19 PM
 */

#include "MovingPart.h"
#include <stdexcept>
#include <iostream>

MovingPart::MovingPart(int dim, Axis ax, int index, std::vector<RubicElement*> elems) : dimension{dim}, axis{ax}, id{index}, elements{elems}, angle{0.0f} {
    if (dimension < 2) {
        throw std::runtime_error{"Error creating Rubic moving part. Dimension must be greater or equal 2."};
    }
    if (id < 0 || dimension <= id) {
        throw std::runtime_error{"Error creating Rubic moving part. Index must be within dimension."};
    }
    for (int i = 0; i < dimension * dimension; ++i) {
        if (elements.at(i) == nullptr) {
            //std::cerr << i << std::endl;
            throw std::runtime_error{"Error creating Rubic moving part. Null element"};
        }
    }
}

MovingPart::MovingPart(int dim, Axis ax, int index) 
        : dimension{dim}, axis{ax}, id{index}, elements(dimension*dimension, nullptr), angle{0.0f} {
    if (dimension < 2) {
        throw std::runtime_error{"Error creating Rubic moving part. Dimension must be greater or equal 2."};
    }
    if (id < 0 || dimension <= id) {
        throw std::runtime_error{"Error creating Rubic moving part. Index must be within dimension."};
    }
}

void MovingPart::print() {
    std::cerr << ((axis.value() == Axis::X) ? "X: " : ( (axis.value() == Axis::Y) ? "Y: " : "Z: "));
    for (auto elem : elements) {
        std::cerr << elem->getId() << " ";
    }
    std::cerr << "\n";
}

int MovingPart::rotate(float ang) {
    int movement = 0;
    angle += ang;
    if (angle >= 90.0f) {
        movement = 1;
        angle = 0.0f;
    }
    if (angle <= -90.0f) {
        movement = -1;
        angle = 0.0f;
    }
    for (auto elem : elements) {
        if (movement != 0) {
            elem->lock(movement, axis);
        } else {
            elem->rotate(angle, axis);
        }
    }
    
    return movement;
}

void MovingPart::cancel() {
    angle = 0.0f;
    for (auto elem : elements) {
        elem->rotate(0.0f, axis);
    }
}


std::vector<RubicElement*> MovingPart::getRow(int rowNum) {
    if (rowNum < 0 || dimension < rowNum) {
        throw std::runtime_error{"Error getting row. Row number is out of bounds"};
    }
    std::vector<RubicElement*> row;
    for (int i = 0; i < dimension; ++i) {
        row.push_back(elements.at((dimension * rowNum) + i));
    }
    return row;
}
std::vector<RubicElement*> MovingPart::getColumn(int colNum) {
    if (colNum < 0 || dimension < colNum) {
        throw std::runtime_error{"Error getting column. Column number is out of bounds"};
    }
    std::vector<RubicElement*> col;
    for (int i = 0; i < dimension; ++i) {
        col.push_back(elements.at((dimension * i) + colNum));
    }
    return col;
}
void MovingPart::setRow(int rowNum, std::vector<RubicElement*> row, bool reverse) {
    if (rowNum < 0 || dimension < rowNum) {
        throw std::runtime_error{"Error setting row. Row number is out of bounds"};
    }
    for (int i = 0; i < dimension; ++i) {
        elements.at((dimension * rowNum) + i) = (reverse) ? row.at(dimension - 1 - i) : row.at(i);
    }
}
void MovingPart::setColumn(int colNum, std::vector<RubicElement*> col, bool reverse) {
    if (colNum < 0 || dimension < colNum) {
        throw std::runtime_error{"Error setting column. Column number is out of bounds"};
    }
    for (int i = 0; i < dimension; ++i) {
        elements.at((dimension * i) + colNum) = (reverse) ? col.at(dimension - 1 - i) : col.at(i);
    }
}
void MovingPart::turn(int movement) {
    std::vector<std::vector<RubicElement*> > rows;
    std::vector<std::vector<RubicElement*> > columns;
    for (int i = 0; i < dimension; i++) {
        rows.push_back(getRow(i));
        columns.push_back(getColumn(i));
    }
    if (axis.value() == Axis::Y) {
        if (movement == 1) {
        for (int i = 0; i < dimension; ++i) {
                setColumn(i, rows.at(i), true);
            }
        }
        else if (movement == -1) {
            for (int i = 0; i < dimension; ++i) {
                setRow(i, columns.at(i), true);
            }        
        }
    }
    else {
        if (movement == 1) {
            for (int i = 0; i < dimension; ++i) {
                setRow(i, columns.at(i), true);
            }
        }
        else if (movement == -1) {
            for (int i = 0; i < dimension; ++i) {
                setColumn(i, rows.at(i), true);
            }        
        }
    }
}