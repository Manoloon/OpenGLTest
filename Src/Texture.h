#pragma once
#include "stb_image.h"
#include <GL/glew.h>

class Texture{

GLuint textureID;
int width = 0;
int height = 0;
int bitDepth = 0;


public:
Texture()=default;
~Texture();

void ClearTexture();
void LoadTexture(const char* fileLocation);
void UseTexture ();
};

