#pragma once 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include "ShadowMap.hpp"

/// @brief Abstract class for Lights
class Light {
    
protected:
    glm::vec3 colour{1.0f,1.0f,1.0f};
    GLfloat ambientIntensity = 1.0f;
    GLfloat diffuseIntensity = 0.0f;
    GLuint shadowWidth = 0;
    GLuint shadowHeight = 0;
    // for point light, would be an array of this.
    glm::mat4 lightProjection = glm::mat4(1.0f);
    

public:
    explicit Light( glm::vec3 colourRGB, 
                    GLfloat _intensity, 
                    GLfloat _diffuseIntensity,
                    GLuint _shadowWidth,
                    GLuint _shadowHeight):colour(colourRGB),ambientIntensity(_intensity),
                        diffuseIntensity(_diffuseIntensity),
                        shadowWidth(_shadowWidth),
                        shadowHeight(_shadowHeight)                        
                       {}

    virtual void InitShadowMap()=0;
};