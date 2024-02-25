#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include "../stb_image.h"
#include <string>

class Texture{

GLuint textureID;
int width = 0;
int height = 0;
int bitDepth = 0;

public:
Texture()=default;
~Texture(){
    ClearTexture();
}

void ClearTexture()
{
    glDeleteTextures(1,&textureID);
    height = 0;
    width = 0;
    bitDepth = 0;
}

void LoadTexture(const std::string& fileLocation)
{
    unsigned char* texData = stbi_load(fileLocation.c_str(),&width,&height,&bitDepth,0);
    if(!texData){
        printf("FAILED TO LOAD TEXTURE DATA");
        return;
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
}

void UseTexture ()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureID);
}

};

