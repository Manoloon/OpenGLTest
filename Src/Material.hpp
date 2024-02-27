#include <GL/glew.h>

class Material{

    GLfloat specularIntensity = 0.0f;
    GLfloat shininess = 0.0f;

    public:
    explicit Material(GLfloat intensity,GLfloat shininess):specularIntensity(intensity),shininess(shininess){}
    
    void Use(GLuint specIntensityLocation,GLuint shininessLocation){
        glUniform1f(specIntensityLocation,specularIntensity);
        glUniform1f(shininessLocation,shininess);
    }
};