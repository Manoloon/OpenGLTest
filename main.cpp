/* OpenGL C++ 20 Glew and Glfw Template */
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string.h>
#include <stdio.h>

const GLint WinWidth = 640;
const GLint WinHeight = 480;

GLuint VAO,VBO,shader;

//  vertex shader
// here we define the shader (this should be doing externally)
static const char* vShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);   \n\
}";

//  Fragment shader
// here we define the shader (this should be doing externally)
static const char* fShader = "                              \n\
#version 330                                                \n\
                                                            \n\
out vec4 colour;                                            \n\
                                                            \n\
void main()                                                 \n\
{                                                           \n\
    colour = vec4(0.2,  1.0,    0.01,    1.0);              \n\
}";

void CreateTriangle(){
    GLfloat vertices[]{
        -1.f,-1.f,0.f,
        1.f,-1.f,0.f,
        0.f,1.f,0.f
    };

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

        glGenBuffers(1,&VBO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        // static is because we are not moving the vertices on runtime
            glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
            /*
                3 are the number of data for the vertice : XYZ, 
                stride : you can have the color of each vertice on the same array ,if so , stride would be 3 , because thats the amount 
                of data that you want to jump from , -1.f,-1.f,0.f, to 1.f,-1.f,0.f.
            */
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER,0);
    // here we finally unbind the VAO.
    glBindVertexArray(0);

}

void addShader(GLuint theProgram,const char* shaderCode,GLenum shaderType){
    // create an empty shader
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0]=shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader,1,theCode,codeLength);
    glCompileShader(theShader);
    // log part
    GLint result = 0;
    // just for log the error
    GLchar eLog[1024] = {0};

    // create the exe on the GPU
    glGetShaderiv(theShader,GL_COMPILE_STATUS,&result);

    if(!result){
        glGetShaderInfoLog(theShader,sizeof(eLog),NULL,eLog);
        printf("Error compiling the %d shader; '%s'\n",shaderType,eLog);
        return;
    }

    glAttachShader(theProgram,theShader);
}

void compileShaders(){
    shader = glCreateProgram();

    if(!shader){
        printf("Error creating shader program!\n");
        return;
    }
    addShader(shader,vShader,GL_VERTEX_SHADER);
    addShader(shader,fShader,GL_FRAGMENT_SHADER);

    GLint result = 0;
    // just for log the error
    GLchar eLog[1024] = {0};

    // create the exe on the GPU
    glLinkProgram(shader);
    glGetProgramiv(shader,GL_LINK_STATUS,&result);

    if(!result){
        glGetProgramInfoLog(shader,sizeof(eLog),NULL,eLog);
        printf("Error Linking program; '%s'\n",eLog);
        return;
    }

    
    glValidateProgram(shader);
    glGetProgramiv(shader,GL_VALIDATE_STATUS,&result);

    if(!result){
        glGetProgramInfoLog(shader,sizeof(eLog),NULL,eLog);
        printf("Error Validating program; '%s'\n",eLog);
        return;
    }
}

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

    // DRAWING
    CreateTriangle();
    compileShaders();
    ///

    while (!glfwWindowShouldClose(mainWindow))
    {
        // inputs events
        glfwPollEvents();
        // clear window (from 0 to 1 RGBA)
        glClearColor(0.3f,0.5f,1.0f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use this shader
        glUseProgram(shader);
        // bind it with this VAO
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        // then unbind 
        glBindVertexArray(0);
        // and close the program
        glUseProgram(0);
        // 2 buffers at this moment.
        glfwSwapBuffers(mainWindow);
    }

    //glfwTerminate();
    return 0;
}