#pragma once

#include <GL\glew.h>
#include <stdio.h>

class ShadowMap
{
    protected:
    GLuint FBO = 0;
    GLuint shadowMap = 0;
    GLuint sWidth, sHeight;

    public:
    ShadowMap()
    {
        FBO = 0;
        shadowMap = 0;
    }
    virtual bool Init(unsigned int Width,unsigned int Height)
    {
        sWidth = Width;
        sHeight = Height;

        
        glGenTextures(1,&shadowMap);
        glBindTexture(GL_TEXTURE_2D,shadowMap);

        glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,Width,Height,0,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);
        
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // NEAREST
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER); //MIRRORED, CLAMP_TO_EDGE, CLAMP_TO_BORDER
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);

        float clampColor[] = {1.0f,1.0f,1.0f,1.0f};
        glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,clampColor);
        
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER,FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,shadowMap,0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Error in ShadowMap::Init: Framebuffer incomplete (status: 0x%x)\n", status);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        return true;
    }

    virtual void Write()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    }

    virtual void Read(GLenum textUnit)
    {
        glActiveTexture(textUnit);
        glBindTexture(GL_TEXTURE_2D,shadowMap);
    }

    GLuint GetShadowMapWidth() const {return sWidth;}
    GLuint GetShadowMapHeight() const {return sHeight;}

    ~ShadowMap()
    {
        if(FBO)
        {
            glDeleteFramebuffers(1,&FBO);
        }
        if(shadowMap)
        {
            glDeleteTextures(1,&shadowMap);
        }
    }
};