#pragma once
#include "Light.hpp"

class DirectionalLight : public Light{

   glm::vec3 direction = {0.0f,-1.0f,0.0f};
public:
    // RGB,intensity,directionLocation,diffuseIntensity,shadow map width, shadow map height)
    explicit DirectionalLight(  glm::vec3 RGB, 
                                GLfloat intensity,
                                glm::vec3 dir, 
                                GLfloat diffuseIntensity,
                                GLuint ShadowWidth,
                                GLuint ShadowHeight)
    : Light(RGB,intensity,diffuseIntensity,ShadowWidth,ShadowHeight),direction(dir)
    {
        lightProjection = glm::ortho(-15.0f,15.0f,-15.0f,15.0f,0.1f,75.0f);
    }
    //ambientIntensityLocation, GLuint ambientColourLocation, GLuint directionLocation, GLuint diffuseIntensityLocation
    void Use(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint directionLocation, GLuint diffuseIntensityLocation)
    {
        glUniform3f(ambientColourLocation,colour.x,colour.y,colour.z);
        glUniform1f(ambientIntensityLocation,ambientIntensity);

        glUniform3f(directionLocation,direction.x,direction.y,direction.z);
        glUniform1f(diffuseIntensityLocation,diffuseIntensity);
    }

    glm::mat4 CalculateLightTransform()
    {
        return lightProjection * glm::lookAt(-direction,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    }
};