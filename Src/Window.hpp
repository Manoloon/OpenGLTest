#pragma once

#include "Constants.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <stdio.h>

class Window
{

    GLFWwindow *mainWindow;
    GLint winWidth{WIN_WIDTH};
    GLint winHeight{WIN_HEIGHT};
    GLint bufferWidth;
    GLint bufferHeight;

    // mouse input
    GLfloat xChange{0.0f};
    GLfloat yChange{0.0f};
    GLfloat xLast;
    GLfloat yLast;
    bool IsMouseFirstMove;

    void CreateCallbacks()
    {
        glfwSetCursorPosCallback(mainWindow, HandleMouse);
    }

    static void HandleMouse(GLFWwindow *win, double xPos, double yPos)
    {
        Window *TheWin = static_cast<Window *>(glfwGetWindowUserPointer(win));

        if (TheWin->IsMouseFirstMove)
        {
            TheWin->xLast = (GLfloat)xPos;
            TheWin->yLast = (GLfloat)yPos;
            TheWin->IsMouseFirstMove = false;
        }
        TheWin->xChange = (GLfloat)xPos - TheWin->xLast;
        TheWin->yChange = TheWin->yLast - (GLfloat)yPos;
        TheWin->xLast = (GLfloat)xPos;
        TheWin->yLast = (GLfloat)yPos;
    }
public:
    Window(const Window&) =delete;
    Window& operator=(const Window&) =delete;
    
    Window(Window&& other) = default;
    Window& operator=(Window&& other) =default;
    Window() = default;
    
    explicit Window(GLint _winWidth, GLint _winHeight) : winWidth(_winWidth), winHeight(_winHeight)
    {
    }
    ~Window()
    {
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
    }
    int Initialise()
    {
        if (glfwInit() == GLFW_FALSE)
        {
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

        if (mainWindow == nullptr)
        {
            glfwTerminate();
            return -1;
        }

        glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
        // set context for glew to use (this allow to switch between windows)
        glfwMakeContextCurrent(mainWindow);

        CreateCallbacks();
        //disable cursor.
        glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Allow modern extension features
        glewExperimental = GL_TRUE;
        // check for glew to init

        if (glewInit() != GLEW_OK)
        {
            glfwDestroyWindow(mainWindow);
            glfwTerminate();
            return -1;
        }
        // this will determine which triangle can be seen.
        glEnable(GL_DEPTH_TEST);
        // Setup viewport size (THE VIEW)
        glViewport(0, 0, bufferWidth, bufferHeight);
        glfwSetWindowUserPointer(mainWindow, this);

        return 0;
    }

    GLsizei GetBufferWidth() const
    {
        return bufferWidth;
    }
    GLsizei GetBufferHeight() const
    {
        return bufferHeight;
    }
    bool GetShouldClose() const
    {
        return glfwWindowShouldClose(mainWindow);
    }
    GLFWwindow *GetWindow() const
    {
        return mainWindow;
    }
    void SwapBuffers()
    {
        glfwSwapBuffers(mainWindow);
    }

    GLfloat GetYChange()
    {
        GLfloat change = yChange;
        yChange = 0.0f;
        return change;
    }

    GLfloat GetXChange()
    {
        GLfloat change = xChange;
        xChange = 0.0f;
        return change;
    }
};