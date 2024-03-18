/* OpenGL C++ 20 Glew and Glfw Template */

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <memory>
#include <Assimp/Importer.hpp>
//#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GLM\mat4x4.hpp>

#include "Src/Constants.h"

#include "Src/Model.hpp"
#include "Src/Window.hpp"
#include "Src/Mesh.hpp"
#include "Src/Shader.hpp"
#include "Src/Camera.hpp"
#include "Src/Texture.hpp"
#include "Src/DirectionalLight.hpp"
#include "Src/PointLight.hpp"
#include "Src/SpotLight.hpp"
#include "Src/Material.hpp"

const float ToRad = 3.14159265f / 180.f;

Window MainWindow;
std::vector<std::shared_ptr<Mesh>> meshList;
//std::vector<std::unique_ptr<Shader>> shaderList;

std::unique_ptr<Shader> directionalShadowShader;
std::unique_ptr<Shader> basicShader;
std::unique_ptr<Shader> OmniShadowShader;
//  vertex shader
static const char* vShader = "Shaders/shader.vert";
//  Fragment shader
static const char* fShader ="Shaders/shader.frag";

static const char* Texture1Loc = "Textures/brick.png";
static const char* Texture2Loc = "Textures/dirt.png";

std::unique_ptr<Camera> camera;
std::shared_ptr<Texture> Text1;
std::shared_ptr<Texture> Text2;
std::shared_ptr<Model> uh60Model;
std::unique_ptr<Material>  SuperShinyMat;
std::unique_ptr<Material>  ShinyMaterial;
std::unique_ptr<Material>  DullMaterial;

GLuint uniformModel = 0;
GLuint uniformView = 0;
GLuint uniformProjection = 0;
GLuint uniformSpecularIntensity=0;
GLuint uniformSpecularShininess=0;
GLuint uniformEyePosition =0;
GLuint uniformDirLightTransform = 0;
GLuint uniformOmniLightPos = 0;
GLuint uniformFarPlane = 0;

unsigned int pointLightCount =0;
unsigned int spotLightCount = 0;

 
GLfloat uh60Angle = 0.0f;   
// RGB,intensity,directionLocation,diffuseIntensity,shadow map Width,shadow map height)
std::unique_ptr<DirectionalLight> directionalLight;
std::array<std::unique_ptr<PointLight>,MAX_POINT_LIGHTS> PointLights;
std::array<std::unique_ptr<SpotLight>,MAX_SPOT_LIGHTS> SpotLights;

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
            -4.f,   -4.f,  -4.0f,  0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
            0.f,    -4.f,  4.f,    0.5f,   0.0f,   0.0f,   0.0f,   0.0f,
            4.f,    -4.f,  -4.0f,  2.0f,   0.0f,   0.0f,   0.0f,   0.0f,
            0.f,    4.f,   0.f,    0.5f,   2.0f,   0.0f,   0.0f,   0.0f
        };

         unsigned int floorIndices[]
        {
            0,2,1, // one side if pyramid
            1,2,3 // other side 
        };

        GLfloat floorVertices[]
        {
            //x,     y,    z,      U,       V       Nx      Ny      Nz
            -20.f,   0.f,  -20.0f,   0.0f,   0.0f,   0.0f,   -1.0f,   0.0f,
             20.f,    0.f,  -20.f,   5.0f,  0.0f,   0.0f,   -1.0f,   0.0f,
            -20.f,   0.f,  20.0f,    0.0f,   5.0f,   0.0f,   -1.0f,   0.0f,
             20.f,    0.f,  20.f,    5.0f,  5.0f,   0.0f,   -1.0f,   0.0f
        };

        CalculateAvgNormals(indices,12,vertices,32,8,5);

        std::shared_ptr<Mesh> obj1 = std::make_shared<Mesh>();
        obj1->CreateMesh(vertices,indices,32,12);
        meshList.emplace_back(obj1);

        std::shared_ptr<Mesh> floorObj = std::make_shared<Mesh>();
        floorObj->CreateMesh(floorVertices,floorIndices,32,6);
        meshList.emplace_back(floorObj);
    }

void CreateShaders()
    {
        basicShader = std::make_unique<Shader>();
        basicShader->CreateFromFiles(vShader,fShader);
        //shaderList.emplace_back(std::move(BasicShader));

        directionalShadowShader = std::make_unique<Shader>();
        directionalShadowShader->CreateFromFiles("Shaders/dirShadowmap.vert","Shaders/dirShadowmap.frag");

        OmniShadowShader = std::make_unique<Shader>();
        OmniShadowShader->CreateFromFiles("Shaders/OmniShadowmap.vert","Shaders/OmniShadowmap.geom","Shaders/OmniShadowmap.frag");
    }

// this function is responsible of rendering the entire scene, it setup models, and
// render each object using its respective material and mesh. 
void RenderScene()
{
    // piramid
    glm::mat4 model(1.0f);
    model = glm::translate(model,glm::vec3(0.0f,0.0f,0.0f));
    glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
    Text1->UseTexture();
    ShinyMaterial->Use(uniformSpecularIntensity,uniformSpecularShininess);
    meshList[0]->RenderMesh();

    // piramid 2
    model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(14.0f,0.0f,3.0f));
    model = glm::scale(model,glm::vec3(2.0,2.0,2.0));
    glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
    Text1->UseTexture();
    ShinyMaterial->Use(uniformSpecularIntensity,uniformSpecularShininess);
    meshList[0]->RenderMesh();

    // Floor this is an obj on a the scene.
    model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(0.0f,-2.0f,0.0f));
    glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
    Text2->UseTexture();
    SuperShinyMat->Use(uniformSpecularIntensity,uniformSpecularShininess);
    meshList[1]->RenderMesh();

    uh60Angle += 0.1f;

    if(uh60Angle > 360.0)
    {
        uh60Angle = 0.0f;
    }
    //Model uh60
    model = glm::mat4(1.0f);
    model = glm::rotate(model,-uh60Angle * ToRad,glm::vec3(0.0f,1.0,0.0f));
    model = glm::translate(model,glm::vec3(-8.0f,2.0f,0.0f));
    model = glm::rotate(model,-20 * ToRad,glm::vec3(0.0f,0.0,1.0f));
    model = glm::rotate(model,-90.0f * ToRad,glm::vec3(1.0f,0.0,0.0f));
    model = glm::scale(model,glm::vec3(0.5,0.5,0.5));
    glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
    SuperShinyMat->Use(uniformSpecularIntensity,uniformSpecularShininess);
    uh60Model->RenderModel();
    
     //Model uh60
    model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(-8.0f,-1.2f,0.0f));
    model = glm::rotate(model,-90.0f * ToRad,glm::vec3(1.0f,0.0,0.0f));
    model = glm::scale(model,glm::vec3(0.5,0.5,0.5));
    glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
    SuperShinyMat->Use(uniformSpecularIntensity,uniformSpecularShininess);
    uh60Model->RenderModel();
}

// Render shadows cast by directional light source.
void DirectionalShadowMapPass(const std::unique_ptr<DirectionalLight>& light)
{
    directionalShadowShader->UseShader();
    //set viewport to same dimension as the framebuffer
    glViewport(0,0,light->GetShadowMap()->GetShadowMapWidth(),
                    light->GetShadowMap()->GetShadowMapHeight());
    
    // Setup framebuffer for render shadow maps
    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Calculate the transform matrix for the light source and passes it to the shader.
    uniformModel = directionalShadowShader->GetModelLocation();
    glm::mat4 lightTransform = light->CalculateLightTransform();
    directionalShadowShader->SetDirLightTransform(&lightTransform);
    directionalShadowShader->Validate();
    // Then call RenderScene() to render the scene from the perspective of the ligth source.
    RenderScene();
    // detach after used.
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

// This pass is for the omni shadow map to be render
void OmniShadowMapPass(const std::unique_ptr<PointLight>& light)
{
    glViewport(0,0,light->GetShadowMap()->GetShadowMapWidth(),
                    light->GetShadowMap()->GetShadowMapHeight());
    
    OmniShadowShader->UseShader();
    uniformModel = OmniShadowShader->GetModelLocation();
    uniformOmniLightPos = OmniShadowShader->GetOmniLightPositionLocation();
    uniformFarPlane = OmniShadowShader->GetFarPlaneLocation();
    
    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    glUniform3f(uniformOmniLightPos,light->GetPosition().x,light->GetPosition().y,light->GetPosition().z);
    glUniform1f(uniformFarPlane,light->GetFarPlane());

    OmniShadowShader->SetLightMatrices(light->CalculateLightTransform());
    OmniShadowShader->Validate();

    RenderScene();
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
// This pass is the same but for SPOTLIGHTS
void OmniShadowMapPass(const std::unique_ptr<SpotLight>& light)
{
    OmniShadowShader->UseShader();
    glViewport(0,0,light->GetShadowMap()->GetShadowMapWidth(),
                light->GetShadowMap()->GetShadowMapHeight());

    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = OmniShadowShader->GetModelLocation();
    uniformOmniLightPos = OmniShadowShader->GetOmniLightPositionLocation();
    uniformFarPlane = OmniShadowShader->GetFarPlaneLocation();

    glUniform3f(uniformOmniLightPos,light->GetPosition().x,light->GetPosition().y,light->GetPosition().z);
    glUniform1f(uniformFarPlane,light->GetFarPlane());

    OmniShadowShader->SetLightMatrices(light->CalculateLightTransform());
    OmniShadowShader->Validate();

    RenderScene();
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
// The main rendering pass of the scene.
// Setup the viewport, clears color and depth buffer. 
void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    basicShader->UseShader();
    uniformProjection = basicShader->GetProjectionLocation();
    uniformModel = basicShader->GetModelLocation();
    uniformView = basicShader->GetViewLocation();
   
    uniformEyePosition = basicShader->GetEyePositionLocation();
    uniformSpecularIntensity = basicShader->GetSpecularIntensityLocation();
    uniformSpecularShininess = basicShader->GetSpecularShininessLocation();

    // TODO : Create this function on windows class.
    glViewport(0,0,1366,768);
    // clear window (from 0 to 1 RGBA)
    glClearColor(0.01f,0.01f,0.01f,1.0f);
    // we are now clearing the depth buffer too.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(uniformProjection,1,GL_FALSE,glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView,1,GL_FALSE,glm::value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition,camera->GetPosition().x,camera->GetPosition().y,camera->GetPosition().z);
    
    basicShader->SetDirectionalLight(directionalLight);
    // we need it to run in this order, I know is ugly but thats the way it handled it in shader.frag.
    basicShader->SetPointLights(PointLights,pointLightCount,3,0);
    basicShader->SetSpotLights(SpotLights,spotLightCount, 3 + pointLightCount, pointLightCount);

    glm::mat4 lightTransform = directionalLight->CalculateLightTransform();
    basicShader->SetDirLightTransform(&lightTransform);
    
    // here is the bug / in texture.hpp useTexture we say to use GL_TEXTURE0
    directionalLight->GetShadowMap()->Read(GL_TEXTURE2);
    basicShader->SetTexture(1);
    basicShader->SetDirectionalShadowMap(2);

    glm::vec3 lantern = camera->GetPosition();
    lantern.y -=0.3f;
    SpotLights[0]->SetTransform(lantern,camera->GetDirection());

    basicShader->Validate();
    RenderScene();
}

int main()
{  
    MainWindow = Window();
    MainWindow.Initialise();
        
    // DRAWING
    CreateObjects();
    CreateShaders();
    GLfloat FOV = glm::radians(60.0f); // 60 to radians = 45.0 angle.
    GLfloat aspectRatio = MainWindow.GetBufferWidth() / MainWindow.GetBufferHeight();
    camera = std::make_unique<Camera>();

    Text1 = std::make_unique<Texture>(Texture1Loc);
    Text1->LoadTextureWithAlpha();
    Text2 = std::make_unique<Texture>(Texture2Loc);
    Text2->LoadTextureWithAlpha();
      
    uh60Model = std::make_shared<Model>();
    uh60Model->LoadModel("Models/uh60.obj");

    SuperShinyMat = std::make_unique<Material>(4.0f,256);
    ShinyMaterial = std::make_unique<Material>(1.0f,32);
    DullMaterial = std::make_unique<Material>(0.3f,4);
     // RGB,intensity,direction,diffuseIntensity,shadow map width, shadow map height)
    directionalLight = std::make_unique<DirectionalLight>(glm::vec3(0.7f,0.6f,0.6f),
                                             0.4f,
                                             glm::vec3(0.0f,-15.0f,6.0f),0.3f,
                                             1024,1024);
    directionalLight->InitShadowMap();
    // RGB,intensity,diffuseIntensity,position,constant,linear,exponent,shadowWidth,shadowHeight,nearP,farP)
    PointLights[0] = std::make_unique<PointLight>(glm::vec3(0.0f,0.0f,1.0f),
                                                0.7f,0.4f,
                                                glm::vec3(-2.0f,1.0f,0.0f),
                                                0.3f,0.02f,0.01f,
                                                1024,
                                                0.1f,100.0f);
    PointLights[0]->InitShadowMap();
    pointLightCount++;
    PointLights[1] = std::make_unique<PointLight>(glm::vec3(0.0f,1.0f,0.0f),
                                                1.0f,0.7f,
                                                glm::vec3(3.0f,1.0f,1.0f),
                                                0.3f,0.2f,0.1f,
                                                1024,
                                                0.1f,100.0f);
    PointLights[1]->InitShadowMap();
    pointLightCount++;
    //RGB, intensity, diffuseIntensity, pos, dir, edge, cons, lin,  exp, shadowWidth,shadowHeight,nearP,farP
    // lantern
    SpotLights[0] = std::make_unique<SpotLight>(glm::vec3(1.0f,1.0f,1.0f),
                                                6.0f,1.0f,
                                                glm::vec3(0.0f,-1.5f,0.0f),
                                                glm::vec3(-100.0f,-1.0f,0.0f),
                                                20.0f,
                                                1.0f,0.4f,0.3f,
                                                1024,
                                                0.1f,500.0f);
    SpotLights[0]->InitShadowMap();
    spotLightCount++;
    // heli flashlight
    SpotLights[1] = std::make_unique<SpotLight>(glm::vec3(1.0f,0.0f,0.0f),
                                                10.0f,0.1f,
                                                glm::vec3(4.0f,2.0f,0.0f),
                                                glm::vec3(0.0f,-1.0f,0.0f),
                                                30.0f,
                                                1.0f,0.3f,0.1f,
                                                1024,
                                                0.1f,100.0f);
    SpotLights[1]->InitShadowMap();
    spotLightCount++;
    ///////////////////////////////////////////////////////////////////////////////
    glm::mat4 projection = glm::perspective(FOV,aspectRatio,0.1f,1000.0f);

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
        
        DirectionalShadowMapPass(directionalLight);

        for(int i = 0; i < pointLightCount; i++)
        {
            OmniShadowMapPass(PointLights[i]);
        }

        for(int i = 0; i < spotLightCount; i++)
        {
            OmniShadowMapPass(SpotLights[i]);
        }

        RenderPass(projection,camera->CalculateViewMatrix());

        // 2 buffers at this moment.
        MainWindow.SwapBuffers();
    }
    // and close the program
    basicShader->ClearShader();
    directionalShadowShader->ClearShader();
    OmniShadowShader->ClearShader();
    glUseProgram(0);
    return 0;
}