#pragma once 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>

/// @brief Abstract class for Lights
class Light {
    
protected:
    glm::vec3 colour = {1.0f,1.0f,1.0f};
    GLfloat ambientIntensity = 1.0f;
    GLfloat diffuseIntensity = 0.0f;

public:
    explicit Light(glm::vec3 RGB, GLfloat intensity, GLfloat diffuseIntensity):colour(RGB),ambientIntensity(intensity),
                       diffuseIntensity(diffuseIntensity){}

};