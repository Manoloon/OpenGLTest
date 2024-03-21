#pragma once
#include "Light.hpp"

class DirectionalLight : public Light{

   glm::vec3 direction = {0.0f,-1.0f,0.0f};
   std::unique_ptr<ShadowMap> shadowMap;
public:
    // RGB,intensity,directionLocation,diffuseIntensity,shadow map width, shadow map height)
    explicit DirectionalLight(  glm::vec3 RGB, 
                                GLfloat intensity,
                                glm::vec3 dir, 
                                GLfloat diffuseIntensity,
                                GLuint shadowWidth,
                                GLuint shadowHeight)
    : Light(RGB,intensity,diffuseIntensity,shadowWidth,shadowHeight),direction(dir),shadowMap{std::make_unique<ShadowMap>()}
    {
        lightProjection = glm::ortho(-20.0f,20.0f,-20.0f,20.0f,0.1f,100.0f);
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

    virtual void InitShadowMap()
    {
        if (shadowWidth == 0 || shadowHeight == 0)
        {
            throw std::invalid_argument("Shadow map has no height or width!!!");
        }
        shadowMap->Init(shadowWidth,shadowHeight);
    }
    const std::unique_ptr<ShadowMap>& GetShadowMap() const {return shadowMap;}
};