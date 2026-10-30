#pragma once
#include <memory>
#include <stdexcept>
#include "Light.hpp"

class DirectionalLight : public Light
{

   glm::vec3 direction = {0.0f,-1.0f,0.0f};
   std::unique_ptr<ShadowMap> shadowMap;

public:
    // RGB,intensity,directionLocation,diffuseIntensity,shadow map width, shadow map height)
    explicit DirectionalLight(  glm::vec3 colourRGB, 
                                GLfloat _intensity,
                                glm::vec3 _direction, 
                                GLfloat _diffuseIntensity,
                                GLuint _shadowWidth,
                                GLuint _shadowHeight)
    : Light(colourRGB,_intensity,_diffuseIntensity,_shadowWidth,_shadowHeight),direction(_direction),shadowMap{std::make_unique<ShadowMap>()}
    {
        lightProjection = glm::ortho(-20.0f,20.0f,-20.0f,20.0f,0.1f,100.0f);
    }
    //ambientIntensityLocation, GLuint ambientColourLocation, GLuint directionLocation, GLuint diffuseIntensityLocation
    void Use(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint directionLocation, GLuint diffuseIntensityLocation)
    {
        glUniform3f((GLint)ambientColourLocation,colour.x,colour.y,colour.z);
        glUniform1f((GLint)ambientIntensityLocation,ambientIntensity);

        glUniform3f((GLint)directionLocation,direction.x,direction.y,direction.z);
        glUniform1f((GLint)diffuseIntensityLocation,diffuseIntensity);
    }

    glm::mat4 CalculateLightTransform()
    {
        return lightProjection * glm::lookAt(-direction,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    }

    void InitShadowMap() override
    {
        if (shadowWidth == 0 || shadowHeight == 0)
        {
            throw std::invalid_argument("Shadow map has no height or width!!!");
        }
        shadowMap->Init(shadowWidth,shadowHeight);
    }
    const std::unique_ptr<ShadowMap>& GetShadowMap() const {return shadowMap;}
};