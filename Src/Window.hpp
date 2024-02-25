#pragma once

#include <stdio.h>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window{

    GLFWwindow* mainWindow; 
    GLint winWidth = 800;
    GLint winHeight = 600;
    GLint bufferWidth;
    GLint bufferHeight;

    // mouse input
    GLfloat xChange = 0.0f;
    GLfloat yChange = 0.0f;
    GLfloat xLast;
    GLfloat yLast;
    bool IsMouseFirstMove;

    void CreateCallbacks(){
        glfwSetCursorPosCallback(mainWindow,HandleMouse);
    }

    static void HandleMouse(GLFWwindow* win,double xPos,double yPos){
        Window* TheWin = static_cast<Window*>(glfwGetWindowUserPointer(win));
        
        if(TheWin->IsMouseFirstMove){
            TheWin->xLast = xPos;
            TheWin->yLast = yPos;
            TheWin->IsMouseFirstMove = false;
        }
        TheWin->xChange = xPos - TheWin->xLast;
        TheWin->yChange = TheWin->yLast - yPos;
        TheWin->xLast = xPos;
        TheWin->yLast = yPos;
       // printf("yaw: %.6f, pitch: %.6f",TheWin->xChange,TheWin->yChange);
    }

    public:

    Window()=default;
    explicit Window(GLint winWidth, GLint winHeight):winWidth(winWidth),winHeight(winHeight){}       
    
    int Initialise(){
        if (!glfwInit()){
            printf("Error Initialising GLFW");
		    glfwTerminate();
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
        
        CreateCallbacks();
        //disable cursor.
        //glfwSetInputMode(mainWindow,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
       
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
        glfwSetWindowUserPointer(mainWindow,this);
        return 0;
    }

    GLfloat GetBufferWidth() const {return bufferWidth;}
    GLfloat GetBufferHeight() const {return bufferHeight;}
    bool GetShouldClose() const {return glfwWindowShouldClose(mainWindow);}
    GLFWwindow* GetWindow() const {return mainWindow;}
    void SwapBuffers(){ glfwSwapBuffers(mainWindow); }

    GLfloat GetYChange(){
        GLfloat change = yChange;
        yChange = 0.0f;
        return change;
    }

     GLfloat GetXChange(){
        GLfloat change = xChange;
        xChange = 0.0f;
        return change;
    }

    ~Window(){
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
    }
};