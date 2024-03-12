#pragma once 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Memory>
#include <GLM/gtc/matrix_transform.hpp>
#include "ShadowMap.hpp"

/// @brief Abstract class for Lights
class Light {
    
protected:
    glm::vec3 colour = {1.0f,1.0f,1.0f};
    GLfloat ambientIntensity = 1.0f;
    GLfloat diffuseIntensity = 0.0f;
    GLuint shadowWidth = 0;
    GLuint shadowHeight = 0;
    // for point light, would be an array of this.
    glm::mat4 lightProjection = glm::mat4(1.0f);
    std::unique_ptr<ShadowMap> shadowMap;

public:
    explicit Light( glm::vec3 RGB, 
                    GLfloat intensity, 
                    GLfloat diffuseIntensity,
                    GLuint shadowWidth,
                    GLuint shadowHeight):colour(RGB),ambientIntensity(intensity),
                        diffuseIntensity(diffuseIntensity),
                        shadowWidth(shadowWidth),
                        shadowHeight(shadowHeight),
                        shadowMap{std::make_unique<ShadowMap>()}
                       {}

    void InitShadowMap()
    {
        if (shadowWidth == 0 || shadowHeight == 0)
        {
            throw std::invalid_argument("Shadow map has no height or width!!!");
        }
        shadowMap->Init(shadowWidth,shadowHeight);
    }

    const std::unique_ptr<ShadowMap>& GetShadowMap() const {return shadowMap;}
};