#pragma once 

#include <string.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <ios>
#include <array>
#include <GL/glew.h>
#include <GLM/gtc/type_ptr.hpp>

#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include "Constants.h"

    struct FUniformOmniShadowMap
    {
        GLuint uniformShadowMap;
        GLuint uniformFarPlane;
    };
    struct FUniformDirLight
    {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformDirection;
    };
    struct FUniformPointLights
    {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    };
    struct FUniformSpotLights
    {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformPosition;
        GLuint uniformDirection;
        GLuint uniformEdge;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    };

class Shader{
    int pointLightCount=0;
    int spotLightCount=0;

    GLuint shaderID,uniformProjection,uniformModel,uniformView, uniformEyePosition =0;
    GLuint uniformSpecularIntensity, uniformSpecularShininess = 0;
    GLuint uniformPointLightCount = 0;
    GLuint uniformSpotLightCount = 0;
    GLuint uniformDirLightTransform = 0;
    GLuint uniformDirectionalShadowMap = 0;
    GLuint uniformTexture = 0;

    GLuint uniformOmniLightPos = 0;
    GLuint uniformFarPlane = 0;
    std::array<GLuint,6> uniformLightMatrices;
    std::array<FUniformOmniShadowMap,MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS> uniformOmniShadowMaps;
    std::array<FUniformPointLights,MAX_POINT_LIGHTS> uniformPointLights;
    std::array<FUniformSpotLights,MAX_SPOT_LIGHTS> UniformSpotLights;
    FUniformDirLight uniformDirectionLight;

    void CompileShader(const char* vertexCode, const char* fragmentCode, const char* geometryCode=nullptr)
    {
            shaderID = glCreateProgram();

        if(!shaderID)
        {
            printf("Error creating shader program!\n");
            return;
        }

        AddShader(shaderID,vertexCode,GL_VERTEX_SHADER);
        if(geometryCode != nullptr)
        {
            AddShader(shaderID,geometryCode,GL_GEOMETRY_SHADER);
        }
        AddShader(shaderID,fragmentCode,GL_FRAGMENT_SHADER);


        GLint result = 0;
        // just for log the error
        GLchar eLog[1024] = {0};

        // create the exe on the GPU
        glLinkProgram(shaderID);
        glGetProgramiv(shaderID,GL_LINK_STATUS,&result);

        if(result != GL_TRUE)
        {
            glGetProgramInfoLog(shaderID,sizeof(eLog),NULL,eLog);
            printf("Error Linking program; '%s'\n",eLog);
            return;
        }
    
        uniformProjection = glGetUniformLocation(shaderID,"projection");
        uniformView = glGetUniformLocation(shaderID,"view");
        uniformModel = glGetUniformLocation(shaderID,"model");
        uniformDirectionLight.uniformColour = glGetUniformLocation(shaderID,"dirLight.base.colour");
        uniformDirectionLight.uniformAmbientIntensity = glGetUniformLocation(shaderID,"dirLight.base.ambientIntensity");
        uniformDirectionLight.uniformDirection = glGetUniformLocation(shaderID,"dirLight.direction");
        uniformDirectionLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID,"dirLight.base.diffuseIntensity");
        uniformSpecularIntensity = glGetUniformLocation(shaderID,"material.specularIntensity");
        uniformSpecularShininess = glGetUniformLocation(shaderID,"material.shininess");
        uniformEyePosition = glGetUniformLocation(shaderID,"eyePosition");
        uniformPointLightCount = glGetUniformLocation(shaderID,"pointLightCount");
        uniformSpotLightCount = glGetUniformLocation(shaderID,"spotLightCount");

        uniformTexture = glGetUniformLocation(shaderID,"theTexture");
        uniformDirLightTransform = glGetUniformLocation(shaderID,"dirLightTransform");
        uniformDirectionalShadowMap = glGetUniformLocation(shaderID,"directionalShadowMap");
        
        uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
        uniformFarPlane = glGetUniformLocation(shaderID,"farPlane");

        for(size_t i = 0;i< MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS;i++)
        {
            char locBuff[100] = {'\0'};
            snprintf(locBuff,sizeof(locBuff),"omniShadowMaps[%d].shadowMap",i);
            uniformOmniShadowMaps[i].uniformShadowMap = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"omniShadowMaps[%d].farPlane",i);
            uniformOmniShadowMaps[i].uniformFarPlane = glGetUniformLocation(shaderID,locBuff);
        }

        if(geometryCode != nullptr)
        {
            for(int i = 0;i<6;i++)
            {
                char locBuff[100] = {'\0'};
                snprintf(locBuff,sizeof(locBuff),"lightMatrices[%d]",i);
                uniformLightMatrices[i] = glGetUniformLocation(shaderID,locBuff);
            }
        }  

        for(size_t i = 0; i < MAX_POINT_LIGHTS; i++)
        {
            char locBuff[100] = {'\0'};

            snprintf(locBuff,sizeof(locBuff),"pLights[%d].base.colour",i);
            uniformPointLights[i].uniformColour = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"pLights[%d].base.ambientIntensity",i);
            uniformPointLights[i].uniformAmbientIntensity = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"pLights[%d].position",i);
            uniformPointLights[i].uniformPosition = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"pLights[%d].base.diffuseIntensity",i);
            uniformPointLights[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"pLights[%d].constant",i);
            uniformPointLights[i].uniformConstant = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"pLights[%d].linear",i);
            uniformPointLights[i].uniformLinear = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"pLights[%d].exponent",i);
            uniformPointLights[i].uniformExponent = glGetUniformLocation(shaderID,locBuff);
        }

         for(size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
        {
            char locBuff[100] = {'\0'};

            snprintf(locBuff,sizeof(locBuff),"sLights[%d].base.base.colour",i);
            UniformSpotLights[i].uniformColour = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"sLights[%d].base.base.ambientIntensity",i);
            UniformSpotLights[i].uniformAmbientIntensity = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"sLights[%d].base.position",i);
            UniformSpotLights[i].uniformPosition = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"sLights[%d].base.base.diffuseIntensity",i);
            UniformSpotLights[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"sLights[%d].base.constant",i);
            UniformSpotLights[i].uniformConstant = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"sLights[%d].base.linear",i);
            UniformSpotLights[i].uniformLinear = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"sLights[%d].base.exponent",i);
            UniformSpotLights[i].uniformExponent = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"sLights[%d].direction",i);
            UniformSpotLights[i].uniformDirection = glGetUniformLocation(shaderID,locBuff);

            snprintf(locBuff,sizeof(locBuff),"sLights[%d].edge",i);
            UniformSpotLights[i].uniformEdge = glGetUniformLocation(shaderID,locBuff);
        }
    }

    void AddShader(GLuint theProgram,const char* shaderCode,GLenum shaderType)
    {
        // create an empty shader
        GLuint theShader = glCreateShader(shaderType);
        const GLchar* theCode[1];
        theCode[0]=shaderCode;

        GLint codeLength[1];
        codeLength[0] = strlen(shaderCode);

        glShaderSource(theShader,1,theCode,codeLength);
        glCompileShader(theShader);
        // log part
        GLint result = 0;
        // just for log the error
        GLchar eLog[1024] = {0};

        // create the exe on the GPU
        glGetShaderiv(theShader,GL_COMPILE_STATUS,&result);

        if(!result)
        {
            glGetShaderInfoLog(theShader,sizeof(eLog),NULL,eLog);
            printf("Error compiling the %d shader; '%s'\n",shaderType,eLog);
            return;
        }

        glAttachShader(theProgram,theShader);
    }

    public:
        Shader()
        {
            //TODO : Hace falta un constructor?
            shaderID = 0;
            uniformModel = 0;
            uniformView = 0;
            uniformProjection = 0;
        };

        void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
        {
            std::string vertexString = ReadFile(vertexLocation);
            std::string fragmentString = ReadFile(fragmentLocation);
            const char* vertexCode = vertexString.c_str();
            const char* fragmentCode = fragmentString.c_str();

            CompileShader(vertexCode,fragmentCode);
        }

         void CreateFromFiles(const char* vertexLocation,  const char* geometryLocation, const char* fragmentLocation)
        {
            std::string vertexString = ReadFile(vertexLocation);
            std::string geometryString = ReadFile(geometryLocation);
            std::string fragmentString = ReadFile(fragmentLocation);
           
            const char* vertexCode = vertexString.c_str();
            const char* geometryCode = geometryString.c_str();
            const char* fragmentCode = fragmentString.c_str();

            CompileShader(vertexCode,fragmentCode,geometryCode);
        }

        void CreateFromString(const char* vertexCode, const char* fragmentCode)
        {
            CompileShader(vertexCode,fragmentCode);
        }

        std::string ReadFile(const char* fileLocation)
        {
            std::string content;
            std::ifstream fileStream(fileLocation,std::ios::in);
            if(!fileStream){
                printf("Failed To read %s! file doesnt exists.",fileLocation);
            }
            std::string line = "";
            while (!fileStream.eof()){
                std::getline(fileStream,line);
                content.append(line + "\n");
            }
            fileStream.close();
            return content;  
        }

        GLuint GetProjectionLocation() const { return uniformProjection; }

        GLuint GetViewLocation() const{return uniformView;}

        GLuint GetModelLocation() const { return uniformModel; }

        GLuint GetAmbientColourLocation() const {return uniformDirectionLight.uniformColour;}

        GLuint GetAmbientIntensityLocation() const {return uniformDirectionLight.uniformAmbientIntensity;}

        GLuint GetDiffuseDirectionLocation() const {return uniformDirectionLight.uniformDirection;}

        GLuint GetDiffuseIntensityLocation() const {return uniformDirectionLight.uniformDiffuseIntensity;}

        GLuint GetSpecularIntensityLocation() const {return uniformSpecularIntensity;}

        GLuint GetSpecularShininessLocation() const {return uniformSpecularShininess; }

        GLuint GetEyePositionLocation() const {return uniformEyePosition;}

        GLuint GetOmniLightPositionLocation() const {return uniformOmniLightPos;}

        GLuint GetFarPlaneLocation() const {return uniformFarPlane;}

        void SetDirectionalLight(const std::unique_ptr<DirectionalLight>& dLight)
        {
            dLight->Use(uniformDirectionLight.uniformAmbientIntensity,
                        uniformDirectionLight.uniformColour,
                        uniformDirectionLight.uniformDirection,
                        uniformDirectionLight.uniformDiffuseIntensity);
        }

        void SetPointLights(const std::array<std::unique_ptr<PointLight>,MAX_POINT_LIGHTS>& pLight,unsigned int lightCount,unsigned int textureUnit, unsigned int offset)
        {
            if(lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;
            glUniform1i(uniformPointLightCount,lightCount);

            for(size_t i =0 ; i < lightCount;i++)
            {
                pLight[i]->Use(uniformPointLights[i].uniformAmbientIntensity,
                                uniformPointLights[i].uniformColour,
                                uniformPointLights[i].uniformPosition,
                                uniformPointLights[i].uniformDiffuseIntensity,
                                uniformPointLights[i].uniformConstant,
                                uniformPointLights[i].uniformLinear,
                                uniformPointLights[i].uniformExponent);
                pLight[i]->GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
                glUniform1i(uniformOmniShadowMaps[i+offset].uniformShadowMap,textureUnit + i);
                glUniform1f(uniformOmniShadowMaps[i+offset].uniformFarPlane, pLight[i]->GetFarPlane());
            }
        }

        void SetSpotLights(const std::array<std::unique_ptr<SpotLight>,MAX_SPOT_LIGHTS>& sLight,unsigned int lightCount,unsigned int textureUnit, unsigned int offset)
        {
            if(lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;
            glUniform1i(uniformSpotLightCount,lightCount);
            for(size_t i =0 ; i < lightCount;i++){
                //ambientIntensityLocation,ambientColourLocation,positionLocation,directionLocation,edgeLocation,diffuseIntensityLocation
                //,constantLocation, linearLocation, GLuint exponentLocation
                sLight[i]->Use(UniformSpotLights[i].uniformAmbientIntensity,
                                UniformSpotLights[i].uniformColour,
                                UniformSpotLights[i].uniformPosition,
                                UniformSpotLights[i].uniformDirection,
                                UniformSpotLights[i].uniformEdge,
                                UniformSpotLights[i].uniformDiffuseIntensity,
                                UniformSpotLights[i].uniformConstant,
                                UniformSpotLights[i].uniformLinear,
                                UniformSpotLights[i].uniformExponent);

                sLight[i]->GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
                glUniform1i(uniformOmniShadowMaps[i+offset].uniformShadowMap,textureUnit + i);
                glUniform1f(uniformOmniShadowMaps[i+offset].uniformFarPlane, sLight[i]->GetFarPlane());
            }
        }

        void SetTexture(GLuint textureUnit)
        {
            glUniform1i(uniformTexture,textureUnit);
        }

        void SetDirectionalShadowMap(GLuint textureUnit)
        {
            glUniform1i(uniformDirectionalShadowMap,textureUnit);
        }

        void SetDirLightTransform(glm::mat4* lightTransform)
        {
            glUniformMatrix4fv(uniformDirLightTransform,1,GL_FALSE,glm::value_ptr(*lightTransform));
        }

        void SetLightMatrices(const std::array<glm::mat4,6>& lightsMatrices)
        {
            for(int i = 0;i < 6; i++)
            {
                glUniformMatrix4fv(uniformLightMatrices[i],1,GL_FALSE,glm::value_ptr(lightsMatrices[i]));
            }
        }

        void UseShader()
        {
            glUseProgram(shaderID);
        }
        
        void ClearShader()
        {
            if(shaderID != 0)
            {
                    glDeleteProgram(shaderID);
                    shaderID = 0;
            }
            uniformModel = 0;
            uniformProjection = 0;
        }

        void Validate()
        {
            GLint result = 0;
            GLchar eLog[1024] = {0};

            glValidateProgram(shaderID);
            glGetProgramiv(shaderID,GL_VALIDATE_STATUS, &result);
            if(!result)
            {
                glGetProgramInfoLog(shaderID,sizeof(eLog),NULL,eLog);
                printf("ERROR Validating : %s'\n",eLog);
                return;
            }
        }
};