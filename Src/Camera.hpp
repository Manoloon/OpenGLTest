#pragma once
#include <GLM/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


class Camera{
    glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};
    glm::vec3 forwardVector = {0.0f, 0.0f, -1.0f};
    glm::vec3 upVector = {0.0f,1.0f,0.0f};
    glm::vec3 WorldUpVector = upVector; 
    glm::vec3 rightVector;
    GLfloat pitch = 0.0f;
    GLfloat yaw = -90.0f;
    GLfloat roll = 0.0f;

    GLfloat moveSpeed = 5.0f;
    GLfloat turnSpeed = 0.3f;

    void Update(){
        forwardVector.x = glm::cos(glm::radians(yaw) * glm::cos(glm::radians(pitch)));
        forwardVector.y = glm::cos(glm::radians(pitch));
        forwardVector.z = glm::sin(glm::radians(yaw) * glm::cos(glm::radians(pitch)));
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

        void KeyControl(GLFWwindow* win,GLfloat deltaTime){
            GLfloat velocity = moveSpeed * deltaTime;
            if(glfwGetKey(win,GLFW_KEY_W) == GLFW_PRESS){
                cameraPosition += forwardVector * velocity;
            }
            if(glfwGetKey(win,GLFW_KEY_A) == GLFW_PRESS){
                    cameraPosition -= rightVector * velocity;
            }
            if(glfwGetKey(win,GLFW_KEY_S) == GLFW_PRESS){
                    cameraPosition -= forwardVector * velocity;
            }
            if(glfwGetKey(win,GLFW_KEY_D) == GLFW_PRESS){
                    cameraPosition +=  rightVector * velocity;
            }
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
            return glm::lookAt(cameraPosition, cameraPosition + forwardVector,upVector);
        }
};