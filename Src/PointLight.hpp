/*
    L/ax^2 + bx + c
*/
#pragma once
#include <array>

#include "Light.hpp"
#include "OmniShadowMap.hpp"

class PointLight : public Light
{
    protected:
    glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
    GLfloat constant = 1.0f;
    GLfloat linear = 0.0f;
    GLfloat exponent = 0.0f;

    GLfloat farPlane = 0.1f;
    std::unique_ptr<OmniShadowMap> shadowMap;

    public :
    explicit PointLight(    glm::vec3 RGB, 
                            GLfloat intensity, 
                            GLfloat diffuseIntensity,
                            glm::vec3 pos,
                            GLfloat cons,
                            GLfloat lin, 
                            GLfloat exp,
                            GLuint shadowSize,
                            GLfloat nearP,
                            GLfloat farP):
                Light(RGB,intensity,diffuseIntensity,shadowSize,shadowSize),shadowMap{std::make_unique<OmniShadowMap>()}
                {
                    position = pos;
                    constant = cons;
                    linear = lin;
                    exponent = exp;
                    farPlane = farP;
                    float aspectRatio = 1.0f;
                    lightProjection = glm::perspective(glm::radians(90.0f),aspectRatio,nearP,farP);
                }

    void Use(GLuint ambientIntensityLocation, GLuint ambientColourLocation, 
                GLuint positionLocation, GLuint diffuseIntensityLocation,
                GLuint constantLocation,GLuint linearLocation, GLuint exponentLocation)
    {
        glUniform3f(ambientColourLocation,colour.x,colour.y,colour.z);
        glUniform1f(ambientIntensityLocation,ambientIntensity);
        glUniform1f(diffuseIntensityLocation,diffuseIntensity);

        glUniform3f(positionLocation,position.x,position.y,position.z);
        
        glUniform1f(constantLocation,constant);
        glUniform1f(linearLocation,linear);
        glUniform1f(exponentLocation,exponent);
    }

    
    std::array<glm::mat4,6> CalculateLightTransform()
    {
        return
        {
            // +x,-x
            lightProjection * glm::lookAt(position, position + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)),
            lightProjection * glm::lookAt(position, position + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)),
            //+y,-y
            lightProjection * glm::lookAt(position, position + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0)),
            lightProjection * glm::lookAt(position, position + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0)),
            //+z,-z
            lightProjection * glm::lookAt(position, position + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,-1.0,0.0)),
            lightProjection * glm::lookAt(position, position + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,-1.0,0.0))
        };
    }

    GLfloat GetFarPlane() const
    {
        return farPlane;
    }

    glm::vec3 GetPosition() const 
    {
        return position;
    }

    virtual void InitShadowMap()
    {
        shadowMap->Init(shadowHeight);
    }

    const std::unique_ptr<OmniShadowMap>& GetShadowMap() const {return shadowMap;}
};