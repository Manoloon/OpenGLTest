#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>

class Light{
    glm::vec3 colour = {1.0f,1.0f,1.0f};
    GLfloat ambientIntensity = 1.0f;

public:
    explicit Light(glm::vec3 RGB, GLfloat intensity):colour(RGB),ambientIntensity(intensity){}

    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation)
    {
        glUniform3f(ambientColourLocation,colour.x,colour.y,colour.z);
        glUniform1f(ambientIntensityLocation,ambientIntensity);
    }

};