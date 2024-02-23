#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

void Texture::ClearTexture()
{
    glDeleteTextures(1,&textureID);
    height = 0;
    width = 0;
    bitDepth = 0;
}

Texture::~Texture()
{
     ClearTexture();
}

void Texture::LoadTexture(const char* fileLocation)
{
    unsigned char* texData = stbi_load(fileLocation,&width,&height,&bitDepth,0);
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

void Texture::UseTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureID);
}
