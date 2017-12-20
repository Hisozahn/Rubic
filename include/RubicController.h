/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RubicModel.h
 * Author: igor
 *
 * Created on December 16, 2017, 12:44 PM
 */

#ifndef RUBICMODEL_H
#define RUBICMODEL_H

#include <vector>
#include "MovingPart.h"
#include "RubicElement.h"
#include "Camera.h"

class RubicController {
public:
    RubicController(int dimension, float element_size, float gap, float distance, int width, int height);
    std::vector<glm::mat4> getModels() const;
    glm::mat4 getProjection() const;
    void ProcessModelMovement(float xoffset, float yoffset);
    void ProcessRMBPress(double xpos, double ypos);
    void ProcessRMBRelease();
    const Camera& getCamera() const { return camera; }
    void ProcessCameraMovement(double xoffset, double yoffset) { camera.ProcessMouseMovement(xoffset, yoffset);}
    void ProcessMouseScroll(double yoffset) { camera.ProcessMouseScroll(yoffset);}
    void updateRatio(int width, int height) { screenWidth = width; screenHeight = height; }
    float getRatio() const { return (float)screenWidth / (float)screenHeight; }
    void setActivePart(Axis axis, int id) {activePart = getPart(axis, id);}
private:
    MovingPart* selectPartByIndeces(Axis chosen);
    void selectCandidates();
    glm::ivec3 getAxisIndeces(int elementIndex);
    void rearrangeParts(MovingPart& rotatedPart, int movement);
    MovingPart* getPart(Axis axis, int id);
    void initMovingParts();
    RubicElement* getElementById(int id);
    bool intersectCube(glm::vec3 orig, glm::vec3 direction, glm::vec3 position, float lineSize, float& distance);
    int peekElement(glm::vec3 origin, glm::vec3 direction);
    int dimension;
    std::vector<RubicElement> elements;
    std::vector<MovingPart> movingParts;
    int screenWidth;
    int screenHeight;
    Camera camera;
    MovingPart* activePart;
    bool choosing;
    std::pair<std::pair<Axis, bool>, std::pair<Axis, bool> > candidates;
    glm::ivec3 axisIndeces;
    glm::ivec2 totalOffset;
};

#endif /* RUBICMODEL_H */

