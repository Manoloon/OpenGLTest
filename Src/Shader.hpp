#pragma once 

#include <string.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <ios>
#include <GL/glew.h>

class Shader{

    GLuint shaderID,uniformProjection,uniformModel,uniformView;

    void CompileShader(const char* vertexCode, const char* fragmentCode){
            shaderID = glCreateProgram();

    if(!shaderID){
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shaderID,vertexCode,GL_VERTEX_SHADER);
    AddShader(shaderID,fragmentCode,GL_FRAGMENT_SHADER);

    GLint result = 0;
    // just for log the error
    GLchar eLog[1024] = {0};

    // create the exe on the GPU
    glLinkProgram(shaderID);
    glGetProgramiv(shaderID,GL_LINK_STATUS,&result);

    if(!result){
        glGetProgramInfoLog(shaderID,sizeof(eLog),NULL,eLog);
        printf("Error Linking program; '%s'\n",eLog);
        return;
    }

    
    glValidateProgram(shaderID);
    glGetProgramiv(shaderID,GL_VALIDATE_STATUS,&result);

    if(!result){
        glGetProgramInfoLog(shaderID,sizeof(eLog),NULL,eLog);
        printf("Error Validating program; '%s'\n",eLog);
        return;
    }
    uniformProjection = glGetUniformLocation(shaderID,"projection");
    uniformView = glGetUniformLocation(shaderID,"view");
    uniformModel = glGetUniformLocation(shaderID,"model");
    }

    void AddShader(GLuint theProgram,const char* shaderCode,GLenum shaderType){
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

    if(!result){
        glGetShaderInfoLog(theShader,sizeof(eLog),NULL,eLog);
        printf("Error compiling the %d shader; '%s'\n",shaderType,eLog);
        return;
    }

    glAttachShader(theProgram,theShader);
    }

    public:
    Shader(){
        shaderID = 0;
        uniformModel = 0;
        uniformView = 0;
        uniformProjection = 0;
    };

    ~Shader(){
        ClearShader();
    }

    void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation){
        std::string vertexString = ReadFile(vertexLocation);
        std::string fragmentString = ReadFile(fragmentLocation);
        const char* vertexCode = vertexString.c_str();
        const char* fragmentCode = fragmentString.c_str();

        CompileShader(vertexCode,fragmentCode);
    }

    void CreateFromString(const char* vertexCode, const char* fragmentCode){
        CompileShader(vertexCode,fragmentCode);
    }

    std::string ReadFile(const char* fileLocation){
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

    GLuint GetProjectionLocation() const {
        return uniformProjection;
    }

    GLuint GetViewLocation() const{
        return uniformView;
    }

    GLuint GetModelLocation() const {
        return uniformModel;
    }

    void UseShader(){
        glUseProgram(shaderID);
    }
    
    void ClearShader(){
        if(shaderID != 0){
                glDeleteProgram(shaderID);
                shaderID = 0;
            }
        uniformModel = 0;
        uniformProjection = 0;
        uniformView = 0;
    }
};