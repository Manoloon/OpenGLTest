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

#include "Src/Constants.h"

#include "Src/Window.hpp"
#include "Src/Mesh.hpp"
#include "Src/Shader.hpp"
#include "Src/Camera.hpp"
#include "Src/Texture.hpp"
#include "Src/DirectionalLight.hpp"
#include "Src/PointLight.hpp"
#include "Src/Material.hpp"

const float ToRad = 3.14159265f / 180.f;

Window MainWindow;
std::vector<std::shared_ptr<Mesh>> meshList;
std::vector<std::shared_ptr<Shader>> shaderList;

//  vertex shader
static const char* vShader = "Shaders/shader.vert";

//  Fragment shader
static const char* fShader ="Shaders/shader.frag";

static const char* Texture1Loc = "Textures/brick.png";
static const char* Texture2Loc = "Textures/dirt.png";
static const char* Texture3Loc = "Textures/tile.png";

void CalculateAvgNormals(unsigned int* indices,unsigned int indiceCount, GLfloat* vertices, unsigned int verticesCount, 
                        unsigned int vlength, unsigned int normalOffset)
    {
        for(size_t i = 0; i < indiceCount; i+=3)
        {
            // each 3 columns * vlength(8)
            unsigned int in0 = indices[i] * vlength;
            unsigned int in1 = indices[i+1] * vlength;
            unsigned int in2 = indices[i+2] * vlength;
            
            glm::vec3 v1 (
            vertices[in1] - vertices[in0], 
            vertices[in1 + 1] - vertices[in0 + 1],
            vertices[in1 + 2] - vertices[in0 + 2]
            );
            
            glm::vec3 v2(
            vertices[in2] - vertices[in0], 
            vertices[in2 + 1] - vertices[in0 + 1],
            vertices[in2 + 2] - vertices[in0 + 2]
            );
            glm::vec3 normal = glm::cross(v1,v2);
            normal = glm::normalize(normal);
            
            // now we are going to use the vertices normals data.
            in0 += normalOffset; 
            in1 += normalOffset;
            in2 += normalOffset;
            
            vertices[in0] += normal.x;
            vertices[in0 + 1] += normal.y;
            vertices[in0 + 2] += normal.z;
            
            vertices[in1] += normal.x;
            vertices[in1 + 1] += normal.y;
            vertices[in1 + 2] += normal.z;
            
            vertices[in2] += normal.x;
            vertices[in2 + 1] += normal.y;
            vertices[in2 + 2] += normal.z;
        }	
        
        for(size_t i = 0 ; i < verticesCount/vlength; i++)
        {
                unsigned int nOffset = i * vlength + normalOffset;
                glm::vec3 vec(vertices[nOffset],vertices[nOffset+1],vertices[nOffset+2]);
                vec = glm::normalize(vec);
                vertices[nOffset] = vec.x;
                vertices[nOffset+1] = vec.y;
                vertices[nOffset+2] = vec.z;
        }
    }

void CreateObjects()
    {
        unsigned int indices[]
        {
            0,3,1, // one side if pyramid
            1,3,2, // other side 
            2,3,0, // front face
            0,1,2 // base of pyramid
        };

        GLfloat vertices[]
        {
            //x,     y,    z,      U,       V       Nx      Ny      Nz
            -1.f,   -1.f,  -0.6f,  0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
            0.f,    -1.f,  1.f,    0.5f,   0.0f,   0.0f,   0.0f,   0.0f,
            1.f,    -1.f,  -0.6f,  1.0f,   0.0f,   0.0f,   0.0f,   0.0f,
            0.f,    1.f,   0.f,    0.5f,   1.0f,   0.0f,   0.0f,   0.0f
        };

         unsigned int floorIndices[]
        {
            0,2,1, // one side if pyramid
            1,2,3 // other side 
        };

        GLfloat floorVertices[]
        {
//x,     y,    z,      U,       V       Nx      Ny      Nz
            -10.f,   0.f,  -10.0f,  0.0f,   0.0f,   0.0f,   -1.0f,   0.0f,
            10.f,    0.f,  -10.f,   10.0f,   0.0f,   0.0f,   -1.0f,   0.0f,
            -10.f,   0.f,  10.0f,   0.0f,   10.0f,   0.0f,   -1.0f,   0.0f,
            10.f,    0.f,  10.f,    10.0f,   10.0f,   0.0f,   -1.0f,   0.0f
        };

        CalculateAvgNormals(indices,12,vertices,32,8,5);

        std::shared_ptr<Mesh> obj1 = std::make_shared<Mesh>();
        obj1->CreateMesh(vertices,indices,32,12);
        meshList.emplace_back(obj1);

        std::shared_ptr<Mesh> obj2 = std::make_shared<Mesh>();
        obj2->CreateMesh(vertices,indices,32,12);
        meshList.emplace_back(obj2);

        std::shared_ptr<Mesh> boxObj = std::make_shared<Mesh>();
        boxObj->CreateMesh(floorVertices,floorIndices,32,6);
        meshList.emplace_back(boxObj);
    }

void CreateShaders()
    {
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
    CreateObjects();
    CreateShaders();

    std::unique_ptr<Camera> camera = std::make_unique<Camera>();
    
    auto Text1 = std::make_unique<Texture>();
    Text1->LoadTexture(Texture1Loc);
    auto Text2 = std::make_unique<Texture>();
    Text2->LoadTexture(Texture2Loc);
    auto texFloor = std::make_unique<Texture>();
    texFloor->LoadTexture(Texture3Loc);
    
    auto SuperShinyMat = std::make_unique<Material>(1.0f,256);
    auto ShinyMaterial = std::make_unique<Material>(1.0f,32);
    auto DullMaterial = std::make_unique<Material>(0.3f,4);
    // RGB,intensity,directionLocation,diffuseIntensity)
    auto directionalLight = std::make_shared<DirectionalLight>(glm::vec3(1.0f,1.0f,1.0f),
                                             0.3f,glm::vec3(0.0f,-1.0f,-1.0f),0.5f);
    unsigned int pointLightCount =0;
    std::shared_ptr<PointLight> PointLights[MAX_POINT_LIGHTS];
    // RGB,intensity,diffuseIntensity,position,constant,linear,exponent)
    PointLights[0] = std::make_shared<PointLight>(glm::vec3(0.0f,0.0f,1.0f),
                                                 1.0f,0.7f,
                                                 glm::vec3(0.0f,1.0f,-1.0f),
                                                 0.3f,0.2f,0.1f);
    pointLightCount++;
    PointLights[1] = std::make_shared<PointLight>(glm::vec3(0.0f,1.0f,0.0f),0.0f,0.7f,
                                                 glm::vec3(-4.0f,2.0f,0.0f),
                                                    0.7f,0.4f,0.3f);
    pointLightCount++;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformProjection = 0;
    GLuint uniformSpecularIntensity=0;
    GLuint uniformSpecularShininess=0;
    GLuint uniformEyePosition =0;

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
        camera->MouseControl(MainWindow.GetXChange(),MainWindow.GetYChange());
        camera->KeyControl(MainWindow.GetWindow(),deltaTime);
        // clear window (from 0 to 1 RGBA)
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        // we are now clearing the depth buffer too.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->UseShader();
        uniformModel = shaderList[0]->GetModelLocation();
        uniformView = shaderList[0]->GetViewLocation();
        uniformProjection = shaderList[0]->GetProjectionLocation();
        uniformEyePosition = shaderList[0]->GetEyePositionLocation();
        uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
        uniformSpecularShininess = shaderList[0]->GetSpecularShininessLocation();

        shaderList[0]->SetDirectionalLight(directionalLight.get());
        shaderList[0]->SetPointLights(PointLights->get(),pointLightCount);

        glUniformMatrix4fv(uniformProjection,1,GL_FALSE,glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView,1,GL_FALSE,glm::value_ptr(camera->CalculateViewMatrix()));
        glUniform3f(uniformEyePosition,camera->GetPosition().x,camera->GetPosition().y,camera->GetPosition().z);

        // bind the uniform value with the value
        glm::mat4 model(1.0f);

        model = glm::translate(model,glm::vec3(0.0f,0.0f,-4.0f));
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
       
        Text1->UseTexture();
        ShinyMaterial->Use(uniformSpecularIntensity,uniformSpecularShininess);
        meshList[0]->RenderMesh();

        // this is an obj on a the scene.
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(3.0f,0.0f,-3.0f));
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));

        Text2->UseTexture();
        DullMaterial->Use(uniformSpecularIntensity,uniformSpecularShininess);
        meshList[1]->RenderMesh();
        //

        // this is an obj on a the scene.
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(0.0f,-2.0f,0.0f));
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));

        texFloor->UseTexture();
        SuperShinyMat->Use(uniformSpecularIntensity,uniformSpecularShininess);
        meshList[2]->RenderMesh();
        //

        // and close the program
        glUseProgram(0);
        // 2 buffers at this moment.
        MainWindow.SwapBuffers();
    }
    return 0;
}