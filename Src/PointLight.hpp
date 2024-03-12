/*
    L/ax^2 + bx + c
*/
#pragma once
#include "Light.hpp"

class PointLight : public Light
{
    protected:
    glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
    GLfloat constant = 1.0f;
    GLfloat linear = 0.0f;
    GLfloat exponent = 0.0f;

    public :
    explicit PointLight(    glm::vec3 RGB, 
                            GLfloat intensity, 
                            GLfloat diffuseIntensity,
                            glm::vec3 pos,
                            GLfloat cons,
                            GLfloat lin, 
                            GLfloat exp):
                Light(RGB,intensity,diffuseIntensity,1024,1024)
                {
                    position = pos;
                    constant = cons;
                    linear = lin;
                    exponent = exp;
                }

    void Use(GLuint ambientIntensityLocation, GLuint ambientColourLocation, 
                GLuint positionLocation, GLuint diffuseIntensityLocation,
                GLuint constantLocation,GLuint linearLocation, GLuint exponentLocation)
    {
        glUniform3f(ambientColourLocation,colour.x,colour.y,colour.z);
        glUniform1f(ambientIntensityLocation,ambientIntensity);

        glUniform3f(positionLocation,position.x,position.y,position.z);
        
        glUniform1f(diffuseIntensityLocation,diffuseIntensity);
        glUniform1f(constantLocation,constant);
        glUniform1f(linearLocation,linear);
        glUniform1f(exponentLocation,exponent);
    }
};