#pragma once

#include <stdio.h>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window{
    GLFWwindow* mainWindow; 
    GLint winWidth = 640;
    GLint winHeight = 480;
    GLint bufferWidth;
    GLint bufferHeight;

    public:
    Window()=default;
    explicit Window(GLint winWidth, GLint winHeight):winWidth(winWidth),winHeight(winHeight){}       
    
    int Initialise(){
        if (!glfwInit()){
            return -1;
        }

        // setup window properties
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        // core profile = no backward compatibility
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        
        mainWindow = glfwCreateWindow(winWidth, winHeight, "Test World", NULL, NULL);
        
        if (!mainWindow)
        {
            glfwTerminate();
            return -1;
        }
    
        glfwGetFramebufferSize(mainWindow,&bufferWidth,&bufferHeight);
        
        // set context for glew to use (this allow to switch between windows)
        glfwMakeContextCurrent(mainWindow);
        
        // Allow modern extension features
        glewExperimental = GL_TRUE;
        // check for glew to init
        if(glewInit() != GLEW_OK){
            glfwDestroyWindow(mainWindow);
            glfwTerminate();
            return -1;
        }
        // this will determine which triangle can be seen.
        glEnable(GL_DEPTH_TEST);
        // Setup viewport size (THE VIEW)
        glViewport(0,0,bufferWidth,bufferHeight);
            return 0;
    }

    GLfloat GetBufferWidth() const {return bufferWidth;}
    GLfloat GetBufferHeight() const {return bufferHeight;}
    bool GetShouldClose() const {return glfwWindowShouldClose(mainWindow);}
    void SwapBuffers(){ glfwSwapBuffers(mainWindow); }

    ~Window(){
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
    }
};