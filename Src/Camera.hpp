#pragma once
#include <GLM/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera{
    glm::vec3 cameraPosition = {9.0f, 4.0f, 16.0f};
    //glm::vec3 CameraDirection = {0.0f, 0.0f, 0.0f};
    glm::vec3 forwardVector = {0.0f, 0.0f, -1.0f};
    glm::vec3 upVector = {0.0f,1.0f,0.0f};
    glm::vec3 WorldUpVector = {0.0f,1.0f,0.0f}; 
    glm::vec3 rightVector = {1.0f,0.0f,0.0f};
    GLfloat yaw = -90.0f;
    GLfloat pitch = 0.0f;
    GLfloat roll = 0.0f;

    GLfloat moveSpeed = 4.0f;
    GLfloat turnSpeed = 0.5f;

    void Update(){
        forwardVector.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        forwardVector.y = glm::sin(glm::radians(pitch));
        forwardVector.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        forwardVector = glm::normalize(forwardVector);

        rightVector = glm::normalize(glm::cross(forwardVector,upVector));
        WorldUpVector = glm::normalize(glm::cross(rightVector,forwardVector));
        //printf("x: %f, y: %f, z: %f",cameraPosition.x,cameraPosition.y,cameraPosition.z);
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
                cameraPosition -= forwardVector * velocity;
            }
            if(glfwGetKey(win,GLFW_KEY_A) == GLFW_PRESS){
                    cameraPosition += rightVector * velocity;
            }
            if(glfwGetKey(win,GLFW_KEY_S) == GLFW_PRESS){
                    cameraPosition += forwardVector * velocity;
            }
            if(glfwGetKey(win,GLFW_KEY_D) == GLFW_PRESS){
                    cameraPosition -=  rightVector * velocity;
            }
            if(glfwGetKey(win,GLFW_KEY_ESCAPE)==GLFW_PRESS){
                glfwSetWindowShouldClose(win, GL_TRUE);
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

        glm::vec3 GetPosition() const {return cameraPosition;}
        glm::vec3 GetDirection() const {return forwardVector;}
};