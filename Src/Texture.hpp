#pragma once
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG

#include <GL/glew.h>
#include "../stb_image.h"
#include <iostream>
#include <string>

class Texture{

GLuint textureID=0;
int width = 0;
int height = 0;
int bitDepth = 0;
const char* fileLocation;

public:
Texture(const char* fileLocation):fileLocation(fileLocation){};
~Texture()
{
    ClearTexture();
}

void ClearTexture()
{
    glDeleteTextures(1,&textureID);
    height = 0;
    width = 0;
    bitDepth = 0;
    fileLocation = "";
}

bool LoadTexture()
{
    unsigned char* texData = stbi_load(fileLocation,&width,&height,&bitDepth,0);
    if(!texData){
        printf(" FAILED TO LOAD TEXTURE DATA : %s \n",fileLocation);
        std::cout << stbi_failure_reason() << std::endl;
        return false;
    }

    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D,textureID);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT); //MIRRORED, CLAMP_TO_EDGE, CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // NEAREST
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(texData);
    return true;
}

bool LoadTextureWithAlpha()
{
 unsigned char* texData = stbi_load(fileLocation,&width,&height,&bitDepth,0);
    if(stbi_failure_reason()){
        printf(" FAILED TO LOAD TEXTURE DATA : %s \n",fileLocation);
        std::cout << stbi_failure_reason() << std::endl;
        return false;
    }

    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D,textureID);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT); //MIRRORED, CLAMP_TO_EDGE, CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // NEAREST
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(texData);
    return true;
}

void UseTexture ()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,textureID);
}

};

