/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RubicModel.cpp
 * Author: igor
 * 
 * Created on December 16, 2017, 12:44 PM
 */

#include "RubicController.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

RubicController::RubicController(int dim, float element_size, float gap, float distance, int width, int height) 
        : dimension{dim}, camera{distance}, activePart{nullptr}, choosing{false},
        candidates{{Axis::x(), false}, {Axis::y(), true}} {
    if (dimension < 2) {
        throw std::runtime_error{"Error creating Rubic model. Dimension must be greater or equal 2."};
    }
    if (width <= 0 || height <= 0) {
        throw std::runtime_error{"Error creating Rubic model. Screen dimensions must be greater than 0."};
    }
    screenWidth = width;
    screenHeight = height;
    float delta = ( float(dimension - 1) / 2 ) * (element_size + gap);
    
    for (int z = 0; z < dimension; ++z) {
        for (int y = 0; y < dimension; ++y) {
            for (int x = 0; x < dimension; ++x) {
                elements.push_back(RubicElement
                {
                    (z * dimension * dimension) + (y * dimension) + x, 
                    glm::vec3
                    {
                        (x * (element_size + gap) ) - delta,
                        (y * (element_size + gap) ) - delta,
                        (z * (element_size + gap) ) - delta
                    }
                });
            }
        }
    }
    initMovingParts();
    for (auto part : movingParts) {
        //part.print();
    }
}

RubicElement* RubicController::getElementById(int id) {
    for (auto& element : elements) {
        if (element.getId() == id) {
            return &element;
        }
    }
    return nullptr;
}

MovingPart* RubicController::getPart(Axis axis, int id) {
    /*switch (axis.value()) {
        case Axis::X: return movingParts.at(id);
        case Axis::Y: return movingParts.at(dimension + id);
        case Axis::Z: return movingParts.at((2 * dimension) + id);
        default: throw std::runtime_error{"Wrong Axis enumeration value"};
    }*/
    auto it = std::find(std::begin(movingParts), std::end(movingParts)
        , MovingPart{dimension, axis, id});
    return it == std::end(movingParts) ? nullptr : it.base();
}


MovingPart* RubicController::selectPartByIndeces(Axis chosen) {
    switch (chosen.value()) {
        case Axis::X: return getPart(chosen, axisIndeces.x);
        case Axis::Y: return getPart(chosen, axisIndeces.y);
        case Axis::Z: return getPart(chosen, axisIndeces.z);
        default: throw std::runtime_error{"Error selecting part by indeces. Invalid axis."};
    }
}

void RubicController::initMovingParts() {
    for (int y = 0; y < dimension; ++y) {
        std::vector<RubicElement*> v;
        int x = 0;
        for (int i = 0; i < dimension*dimension; ++i) {
            v.push_back(getElementById(y + x));
            x += dimension;
        }
        movingParts.push_back(MovingPart(dimension, Axis::x(), y, v));
    }
    for (int y = 0; y < dimension; ++y) {
        std::vector<RubicElement*> v;
        for (int z = 0; z < dimension; ++z) {
            for (int x = 0; x < dimension; ++x) {
                v.push_back(getElementById(y*dimension + x + z * dimension * dimension));
            }
        }
        movingParts.push_back(MovingPart(dimension, Axis::y(), y, v));
    }
    for (int y = 0; y < dimension; ++y) {
        std::vector<RubicElement*> v;
        for (int x = 0; x < dimension*dimension; ++x) {
            v.push_back(getElementById(y*dimension*dimension + x));
        }
        movingParts.push_back(MovingPart(dimension, Axis::z(), y, v));
    }
    
}

std::vector<glm::mat4> RubicController::getModels() const {
    std::vector<glm::mat4> models;
    for (auto element : elements) {
        models.push_back(element.getModel());
    }
    return models;
}

bool RubicController::intersectCube(glm::vec3 orig, glm::vec3 direction, glm::vec3 position, float lineSize, float& distance) {
    /* 
    Fast Ray-Box Intersection
    by Andrew Woo
    from "Graphics Gems", Academic Press, 1990
    */
    static const int RIGHT = 0;
    static const int LEFT = 1;
    static const int MIDDLE = 2;
    static const int NUMDIM = 3;
    
    bool inside = true;
    char quadrant[NUMDIM];
    register int i;
    int whichPlane;
    double maxT[NUMDIM];
    double candidatePlane[NUMDIM];

    double minB[NUMDIM] = {position.x - lineSize, position.y - lineSize, position.z - lineSize};
    double maxB[NUMDIM] = {position.x + lineSize, position.y + lineSize, position.z + lineSize};		/*box */
    double origin[NUMDIM] = {orig.x, orig.y, orig.z};
    double dir[NUMDIM] = {direction.x, direction.y, direction.z};;	/*ray */
    double coord[NUMDIM];				/* hit point */

    /* Find candidate planes; this loop can be avoided if
    rays cast all from the eye(assume perpsective view) */
    for (i=0; i<NUMDIM; i++)
        if(origin[i] < minB[i]) {
            quadrant[i] = LEFT;
            candidatePlane[i] = minB[i];
            inside = false;
        }else if (origin[i] > maxB[i]) {
            quadrant[i] = RIGHT;
            candidatePlane[i] = maxB[i];
            inside = false;
        }else	{
            quadrant[i] = MIDDLE;
        }

    /* Ray origin inside bounding box */
    if(inside)	{
        for (i=0; i<NUMDIM; i++)
            coord[i] = origin[i];
        return true;
    }


    /* Calculate T distances to candidate planes */
    for (i = 0; i < NUMDIM; i++)
        if (quadrant[i] != MIDDLE && dir[i] !=0.)
            maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
        else
            maxT[i] = -1.;

    /* Get largest of the maxT's for final choice of intersection */
    whichPlane = 0;
    for (i = 1; i < NUMDIM; i++)
        if (maxT[whichPlane] < maxT[i])
            whichPlane = i;

    /* Check final candidate actually inside box */
    if (maxT[whichPlane] < 0.) return false;
    for (i = 0; i < NUMDIM; i++)
        if (whichPlane != i) {
            coord[i] = origin[i] + maxT[whichPlane] *dir[i];
            if (coord[i] < minB[i] || coord[i] > maxB[i])
                        return false;
        } else {
            coord[i] = candidatePlane[i];
        }
    distance = glm::distance(glm::vec3{coord[0], coord[1], coord[2]}, orig);
    return true;
}

int RubicController::peekElement(glm::vec3 origin, glm::vec3 direction) {
    float minDistance = camera.getRadius() * 2;
    int minIndex = -1;
    for (auto& element : elements) {
        float intDistance;
        if (intersectCube(origin, direction, element.getDefaultPosition(), 1.0f, intDistance)) {
            if (intDistance < minDistance) {
                minDistance = intDistance;
                minIndex = element.getId();
            }
        }
    }
    return minIndex;
}

glm::ivec3 RubicController::getAxisIndeces(int elementIndex) {
    return glm::ivec3{
        elementIndex % dimension, 
        (elementIndex / dimension) % dimension,
        elementIndex / dimension / dimension
    };
}

void RubicController::selectCandidates() {
    std::string side = camera.getSide();
    glm::vec3 cPos = glm::normalize(camera.getPosition());
    if (side == "Front") {
        candidates = {{Axis::y(), true}, {Axis::x(), false}};
    }
    else if (side == "Right") {
        candidates = {{Axis::y(), true}, {Axis::z(), true}};
    }
    else if (side == "Back") {
        candidates = {{Axis::y(), true}, {Axis::x(), true}};
    }
    else if (side == "Left") {
        candidates = {{Axis::y(), true}, {Axis::z(), false}};
    }
    else if (side == "Top") {
        if (glm::abs(cPos.z) > glm::abs(cPos.x)) {
            if (cPos.z > 0.0f) { // Top front
                candidates = {{Axis::z(), false}, {Axis::x(), false}};
            }
            else { // Top back
                candidates = {{Axis::z(), true}, {Axis::x(), true}};
            }
        }
        else {
            if (cPos.x > 0.0f) { // Top right
                candidates = {{Axis::x(), false}, {Axis::z(), true}};
            }
            else { // Top left
                candidates = {{Axis::x(), true}, {Axis::z(), false}};
            }
        }
    }
    else if (side == "Bottom") {
        if (glm::abs(cPos.z) > glm::abs(cPos.x)) {
            if (cPos.z > 0.0f) { // Bottom front
                candidates = {{Axis::z(), true}, {Axis::x(), false}};
            }
            else { // Bottom back
                candidates = {{Axis::z(), false}, {Axis::x(), true}};
            }
        }
        else {
            if (cPos.x > 0.0f) { // Bottom right
                candidates = {{Axis::x(), true}, {Axis::z(), true}};
            }
            else { // Bottom left
                candidates = {{Axis::x(), false}, {Axis::z(), false}};
            }
        }
    }
}

void RubicController::ProcessRMBPress(double xpos, double ypos) {
    float x = (2 * xpos / screenWidth) - 1;
    float y = 1 - (2 * ypos / screenHeight);
    
    glm::mat4 proj = camera.getProjection(getRatio());
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f), glm::normalize(-camera.getPosition()), glm::vec3{0.0f, 1.0f, 0.0f});
    
    glm::mat4 invVp = glm::inverse(proj * view);
    glm::vec4 screenPos = glm::vec4(x, y, 1.0f, 1.0f);
    glm::vec4 worldPos = invVp * screenPos;
    
    glm::vec3 ray = glm::normalize(glm::vec3(worldPos));
    int index = peekElement(camera.getPosition(), ray);
    if (index < 0) {
        return;
    }
    choosing = true;
    totalOffset = glm::dvec2(0.0);
    axisIndeces = getAxisIndeces(index);
    selectCandidates();
}

void RubicController::rearrangeParts(MovingPart& rotatedPart, int movement) {
    rotatedPart.turn(movement);
    std::vector<std::vector<RubicElement*> > rows;
    std::vector<std::vector<RubicElement*> > columns;
    for (int i = 0; i < dimension; i++) {
        rows.push_back(rotatedPart.getRow(i));
        columns.push_back(rotatedPart.getColumn(i));
    }
    switch (rotatedPart.getAxis().value()) {
        case Axis::X: {
            for (int i = 0; i < dimension; ++i) {
                getPart(Axis::y(), i)->setColumn(rotatedPart.getId(), columns.at(i), false);
                getPart(Axis::z(), i)->setColumn(rotatedPart.getId(), rows.at(i), false);
            }
        }
        break;
        case Axis::Y: {
            for (int i = 0; i < dimension; ++i) {
                getPart(Axis::x(), i)->setColumn(rotatedPart.getId(), columns.at(i), false);
                getPart(Axis::z(), i)->setRow(rotatedPart.getId(), rows.at(i), false);
            }            
        }
        break;
        case Axis::Z: {
            for (int i = 0; i < dimension; ++i) {
                getPart(Axis::x(), i)->setRow(rotatedPart.getId(), columns.at(i), false);
                getPart(Axis::y(), i)->setRow(rotatedPart.getId(), rows.at(i), false);
            }  
        }
        break;
        default: throw std::runtime_error{"Error rearranging parts. Invalid axis."};
    }
    
}
void RubicController::ProcessRMBRelease() {
    choosing = false;
    totalOffset = glm::dvec2(0.0);
    if (activePart == nullptr) {
        return;
    }
    activePart->cancel();
    activePart = nullptr;
}


void RubicController::ProcessModelMovement(float xoffset, float yoffset) {
    static const float selectionTreshhold = 15.0f;
    static bool positiveDirection = true;
    static bool horizontal = true;
    if (choosing) {
        totalOffset.x += xoffset;
        totalOffset.y += yoffset;
        if (glm::abs(totalOffset.x) > selectionTreshhold) {
            //std::cerr << "First selected\n";
            activePart = selectPartByIndeces(candidates.first.first);
            positiveDirection = candidates.first.second;
            horizontal = true;
            choosing = false;
        }
        else if (glm::abs(totalOffset.y) > selectionTreshhold) {
            //std::cerr << "Second selected\n";
            activePart = selectPartByIndeces(candidates.second.first);
            positiveDirection = candidates.second.second;
            horizontal = false;
            choosing = false;
        }
    }
    else {
        if (activePart == nullptr) {
            return;
        }
        float offset;
        if (horizontal) {
            offset = positiveDirection ? xoffset : -xoffset;
        }
        else {
            offset = positiveDirection ? yoffset : -yoffset;
        }
        int movement = activePart->rotate(offset);
        if (movement != 0) {
            rearrangeParts(*activePart, movement);
            activePart = nullptr;
        }
    }
}
glm::mat4 RubicController::getProjection() const {
    return camera.getProjection( getRatio() );
}