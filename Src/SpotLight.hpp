
#pragma once
#include "PointLight.hpp"

class SpotLight : public PointLight
{
    glm::vec3 direction = glm::vec3(0.0f,-1.0f,0.0f);
    GLfloat edge = glm::radians(cosf(0.0f));
    bool bIsOn;
    public :
    explicit SpotLight(glm::vec3 RGB, GLfloat intensity, GLfloat diffuseIntensity,
                        glm::vec3 pos,glm::vec3 dir,GLfloat edgeAngle,
                        GLfloat cons,GLfloat lin, GLfloat exp,                            
                        GLuint shadowSize,
                        GLfloat nearP,
                        GLfloat farP):
                PointLight(RGB,intensity,diffuseIntensity,pos,cons,lin,exp,shadowSize,nearP,farP)
                {
                    direction = glm::normalize(dir);
                    edge = cosf(glm::radians(edgeAngle));
                    bIsOn = true;
                }

    void Use(GLuint ambientIntensityLocation, GLuint ambientColourLocation, 
                GLuint positionLocation,GLuint directionLocation, GLuint edgeLocation, GLuint diffuseIntensityLocation,
                GLuint constantLocation,GLuint linearLocation, GLuint exponentLocation)
    {
        if(bIsOn)
        {
            glUniform1f(ambientIntensityLocation,ambientIntensity);
            glUniform1f(diffuseIntensityLocation,diffuseIntensity);
        }
        else
        {
            glUniform1f(ambientIntensityLocation,0.0f);
            glUniform1f(diffuseIntensityLocation,0.0f);
        }
        glUniform3f(ambientColourLocation,colour.x,colour.y,colour.z);
        

        glUniform3f(positionLocation,position.x,position.y,position.z);
        glUniform3f(directionLocation,direction.x,direction.y,direction.z);

        glUniform1f(edgeLocation,edge);

        
        glUniform1f(constantLocation,constant);
        glUniform1f(linearLocation,linear);
        glUniform1f(exponentLocation,exponent);
    }

    void SetTransform(glm::vec3 locPosition,glm::vec3 locDirection){
        position = locPosition;
        direction = locDirection;
    }

    void Toggle(){bIsOn = !bIsOn;}
};