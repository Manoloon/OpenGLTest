#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>

class Light{
    glm::vec3 colour = {1.0f,1.0f,1.0f};
    GLfloat ambientIntensity = 1.0f;

    glm::vec3 direction = {0.0f,-1.0f,0.0f};
    GLfloat diffuseIntensity = 0.0f;

public:
    explicit Light(glm::vec3 RGB, GLfloat intensity,glm::vec3 directionLocation, GLfloat diffuseIntensity):colour(RGB),ambientIntensity(intensity),
                        direction(directionLocation),diffuseIntensity(diffuseIntensity){}

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint directionLocation, GLuint diffuseIntensityLocation)
    {
        glUniform3f(ambientColourLocation,colour.x,colour.y,colour.z);
        glUniform1f(ambientIntensityLocation,ambientIntensity);

        glUniform3f(directionLocation,direction.x,direction.y,direction.z);
        glUniform1f(diffuseIntensityLocation,diffuseIntensity);
    }

};