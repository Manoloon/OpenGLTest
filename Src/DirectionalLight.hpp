#pragma once
#include "Light.hpp"

class DirectionalLight : public Light{

    glm::vec3 direction = {0.0f,-1.0f,0.0f};
public:
    // RGB,intensity,directionLocation,diffuseIntensity)
    explicit DirectionalLight(glm::vec3 RGB, GLfloat intensity,glm::vec3 directionLocation, GLfloat diffuseIntensity)
    : Light(RGB,intensity,diffuseIntensity){
        direction = directionLocation;
    }
    //ambientIntensityLocation, GLuint ambientColourLocation, GLuint directionLocation, GLuint diffuseIntensityLocation
    void Use(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint directionLocation, GLuint diffuseIntensityLocation)
    {
        glUniform3f(ambientColourLocation,colour.x,colour.y,colour.z);
        glUniform1f(ambientIntensityLocation,ambientIntensity);

        glUniform3f(directionLocation,direction.x,direction.y,direction.z);
        
        glUniform1f(diffuseIntensityLocation,diffuseIntensity);
    }
};