#pragma once
#include <vector>
#include <string>
#include <GL\glew.h>
#include "Constants.h"
#include "Mesh.hpp"
#include "Shader.hpp"

class SkyBox
{
    std::unique_ptr<Mesh> skyMesh;
    std::unique_ptr<Shader> skyShader;
    GLuint textureID;
    GLuint uniformProjection;
    GLuint uniformView;

    void TextureSetup(const std::vector<std::string>& faceLocation)
    {
        glGenTextures(1,&textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width,height,bitDepth;

        for(int i = 0; i < 6; i++)
        {
            unsigned char* texData = stbi_load(faceLocation[i].c_str(),&width,&height,&bitDepth,0);
            if(!texData)
            {
                printf(" FAILED TO LOAD TEXTURE DATA : %s \n",faceLocation[i].c_str());
                return;
            }
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,texData); 
            stbi_image_free(texData);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE); 
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }

    void MeshSetup()
    {
        unsigned int skyboxIndices[] = 
        {
            // front
            0, 1, 2,
            2, 1, 3,
            // right
            2, 3, 5,
            5, 3, 7,
            // back
            5, 7, 4,
            4, 7, 6,
            // left
            4, 6, 0,
            0, 6, 1,
            // top
            4, 0, 5,
            5, 0, 2,
            // bottom
            1, 6, 3,
            3, 6, 7
        };

    float skyboxVertices[] = 
    {
        -1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

        -1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
    };  
        skyMesh = std::make_unique<Mesh>();
        skyMesh->CreateMesh(skyboxVertices,skyboxIndices,64,36);
    }

public:

    SkyBox(const std::vector<std::string>& faceLocation)
    {
        skyShader = std::make_unique<Shader>();
        skyShader->CreateFromFiles("Shaders/skyBox.vert","Shaders/skyBox.frag");
        uniformProjection = skyShader->GetProjectionLocation();
        uniformView = skyShader->GetViewLocation();
        TextureSetup(faceLocation);
        MeshSetup();
    }

    void Draw(glm::mat4 viewMatrix, glm::mat4 projectMatrix)
    {
        // avoid moving around , discard the location data.
        viewMatrix = glm::mat4(glm::mat3(viewMatrix));

        glDepthMask(GL_FALSE);

        skyShader->UseShader();
        glUniformMatrix4fv(uniformProjection,1,GL_FALSE,glm::value_ptr(projectMatrix));
        glUniformMatrix4fv(uniformView,1,GL_FALSE,glm::value_ptr(viewMatrix));

        // as only have 1 sampler cube we can use texture0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);
        skyShader->Validate();

        skyMesh->RenderMesh();

        glDepthMask(GL_TRUE);
    }
};