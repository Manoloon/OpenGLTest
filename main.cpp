/* OpenGL C++ 20 Glew and Glfw Template */

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <memory>

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "GLM/mat4x4.hpp"

#include "Src/Window.hpp"
#include "Src/Mesh.hpp"
#include "Src/Shader.hpp"
#include "Src/Camera.hpp"
#include "Src/Texture.hpp"

const float ToRad = 3.14159265f / 360.f;

Window MainWindow;
std::vector<std::shared_ptr<Mesh>> meshList;
std::vector<std::shared_ptr<Shader>> shaderList;

//  vertex shader
static const char* vShader = "Shaders/shader.vert";

//  Fragment shader
static const char* fShader ="Shaders/shader.frag";

static const char* Texture1Loc = "Textures/brick.png";
static const char* Texture2Loc = "Textures/dirt.png";

void CreateTObjects(){
    unsigned int indices[]{
        0,3,1, // one side if pyramid
        1,3,2, // other side 
        2,3,0, // front face
        0,1,2 // base of pyramid
    };

    GLfloat vertices[]{
        //x,     y,    z,      U,       V
        -1.f,   -1.f,   0.f,    0.0f,   0.0f,
        0.f,    -1.f,   1.f,    0.5f,   0.0f,
        1.f,    -1.f,   0.f,    1.0f,   0.0f,
        0.f,    1.f,    0.f,    0.5f,   1.0f
    };

    std::shared_ptr<Mesh> obj1 = std::make_shared<Mesh>();
    obj1->CreateMesh(vertices,indices,20,12);
    meshList.emplace_back(obj1);

    std::shared_ptr<Mesh> obj2 = std::make_shared<Mesh>();
    obj2->CreateMesh(vertices,indices,20,12);
    meshList.emplace_back(obj2);
}

void CreateShaders(){
    std::shared_ptr<Shader> Shader1 = std::make_shared<Shader>();
    Shader1->CreateFromFiles(vShader,fShader);
    shaderList.emplace_back(Shader1); 
}

int main()
{  
    MainWindow = Window();
    MainWindow.Initialise();

    GLfloat FOV = 45.0f;
    GLfloat aspectRatio = MainWindow.GetBufferWidth() / MainWindow.GetBufferHeight();
     // DRAWING
    CreateTObjects();
    CreateShaders();

    Camera camera;
    
    std::unique_ptr<Texture> Text1 = std::make_unique<Texture>();
    Text1->LoadTexture(Texture1Loc);
    std::unique_ptr<Texture> Text2 = std::make_unique<Texture>();
    Text2->LoadTexture(Texture2Loc);

    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformProjection = 0;
    glm::mat4 projection = glm::perspective(FOV,aspectRatio,0.1f,1000.0f);
    ///
    GLfloat lastFrameTime = 0.0f;
    GLfloat deltaTime = 0.0f;

    while (!MainWindow.GetShouldClose())
    {
        // Calculate delta time
        GLfloat now = glfwGetTime();

        deltaTime = lastFrameTime - now;
        lastFrameTime = now;
        // inputs events
        glfwPollEvents();
        camera.MouseControl(MainWindow.GetXChange(),MainWindow.GetYChange());
        camera.KeyControl(MainWindow.GetWindow(),deltaTime);
        // clear window (from 0 to 1 RGBA)
        glClearColor(0.3f,0.5f,1.0f,1.f);
        // we are now clearing the depth buffer too.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->UseShader();
        uniformModel = shaderList[0]->GetModelLocation();
        uniformView = shaderList[0]->GetViewLocation();
        uniformProjection = shaderList[0]->GetProjectionLocation();

        // bind the uniform value with the value
        glm::mat4 model(1.0f);
        model = glm::translate(model,glm::vec3(2.0f,0.0f,0.1f));
       // model = glm::rotate(model, 45.0f * ToRad, glm::vec3(0.0f,1.0f,0.0f));
        model = glm::scale(model,glm::vec3(0.4f));
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection,1,GL_FALSE,glm::value_ptr(projection));
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(0.0f,0.0f,3.0f));
        model = glm::scale(model,glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));

        glUniformMatrix4fv(uniformView,1,GL_FALSE,glm::value_ptr(camera.CalculateViewMatrix()));
        meshList[1]->RenderMesh();
       
        // and close the program
        glUseProgram(0);
        // 2 buffers at this moment.
        MainWindow.SwapBuffers();
    }
    return 0;
}