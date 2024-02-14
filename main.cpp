/* OpenGL C++ 20 Glew and Glfw Template */
#include <GL\glew.h>
#include <GLFW\glfw3.h>

const GLint WinWidth = 640;
const GLint WinHeight = 480;
int main()
{  
    if (!glfwInit()){
        return -1;
    }

    // setup window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // core profile = no backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* mainWindow = glfwCreateWindow(WinWidth, WinHeight, "Test World", NULL, NULL);
    
    if (!mainWindow)
    {
        glfwTerminate();
        return -1;
    }
    // get buffer size information (is the part of the window to draw)
    int bufferWidth;
    int bufferHeight;
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
    // Setup viewport size (THE VIEW)
    glViewport(0,0,bufferWidth,bufferHeight);


    while (!glfwWindowShouldClose(mainWindow))
    {
        // inputs events
        glfwPollEvents();
        // clear window (from 0 to 1 RGBA)
        glClearColor(0.3f,0.5f,1.0f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 2 buffers at this moment.
        glfwSwapBuffers(mainWindow);
    }

    //glfwTerminate();
    return 0;
}