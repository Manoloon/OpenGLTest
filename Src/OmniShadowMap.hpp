#pragma once

#include "ShadowMap.hpp"

class OmniShadowMap : public ShadowMap
{

    public:
    virtual bool Init(unsigned int Width,unsigned int Height)
    {
        sWidth = Width;
        sHeight = Height;

        glGenFramebuffers(1,&FBO);
        glGenTextures(1,&shadowMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP,shadowMap);

        for(int i = 0; i < 6 ; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, sWidth, sHeight, 0, 
                                GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,shadowMap,0);

        // avoid color 
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
        //GL_DRAW_FRAMEBUFFER
        glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    }

    virtual void Read(GLenum textUnit)
    {
        glActiveTexture(textUnit);
        glBindTexture(GL_TEXTURE_CUBE_MAP,shadowMap);
    }
};