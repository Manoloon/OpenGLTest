#pragma once
#include <GLM/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


class Camera{
    glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};  // Assuming the camera is at (0, 0, 1)
    glm::vec3 forwardVector = {0.0f, 0.0f, -1.0f};  // Assuming the camera is looking at (1, 0, 0)
    glm::vec3 WorldUpVector = {0.0f, 1.0f, 0.0f};  
    glm::vec3 rightVector;
    GLfloat pitch = -90.0f;
    GLfloat yaw = 0.0f;
    GLfloat roll = 0.0f;

    GLfloat moveSpeed = 5.0f;
    GLfloat turnSpeed = 1.0f;

    void Update(){
        forwardVector.x = glm::cos(glm::radians(pitch) * glm::cos(glm::radians(yaw)));
        forwardVector.y = glm::cos(glm::radians(pitch));
        forwardVector.z = glm::cos(glm::radians(pitch) * glm::sin(glm::radians(yaw)));
        forwardVector = glm::normalize(forwardVector);

        rightVector = glm::normalize(glm::cross(forwardVector,WorldUpVector));
        WorldUpVector = glm::normalize(glm::cross(rightVector,forwardVector));    
    }

    public:
        Camera(){Update();}
        explicit Camera(glm::vec3 startPos, GLfloat startMoveSpeed,GLfloat startTurnSpeed):
            cameraPosition(startPos),moveSpeed(startMoveSpeed),turnSpeed(startTurnSpeed){
                Update();
            }

        void KeyControl(GLfloat deltaTime){

        }

        void MouseControl(GLfloat xChange,GLfloat yChange){
            xChange *= turnSpeed;
            yChange *= turnSpeed;

            yaw += xChange;
            pitch += yChange;
            if(pitch > 89.0f) pitch = 89.0f;
            if(pitch < -89.0f) pitch = -89.0f;
            Update();
        }

        glm::mat4 CalculateViewMatrix(){
            return glm::lookAt(cameraPosition, cameraPosition + forwardVector,WorldUpVector);
        }
};