#pragma once 
#include <GL/glew.h>

class Mesh{
    GLuint VAO=0;
    GLuint VBO=0;
    GLuint IBO=0;
    GLsizei indexCount=0;

    public:
        Mesh(){
            VAO =0;
            VBO =0;
            IBO =0;
            indexCount =0;
        }

        void CreateMesh(GLfloat* vertices,unsigned int* indices, unsigned int numVertices, unsigned int numIndices){
            indexCount = numIndices;

            unsigned long long VertSize = sizeof(vertices[0]);
            
            glGenVertexArrays(1,&VAO);
            glBindVertexArray(VAO);

            glGenBuffers(1,&IBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices[0]) * numIndices,indices,GL_STATIC_DRAW);

            glGenBuffers(1,&VBO);
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            // static is because we are not moving the vertices on runtime
            glBufferData(GL_ARRAY_BUFFER,VertSize * numVertices,vertices,GL_STATIC_DRAW);
            
                /*
                    3 are the number of data for the vertice : XYZ, 
                    stride : you can have the color of each vertice on the same array ,if so , stride would be 3 , because thats the amount 
                    of data that you want to jump from , -1.f,-1.f,0.f, to 1.f,-1.f,0.f.
                */
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, VertSize * 8,0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,VertSize * 8, (void*)(VertSize * 3));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,VertSize * 8, (void*)(VertSize * 5));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER,0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
            // here we finally unbind the VAO.
            glBindVertexArray(0);
        }
        
        void RenderMesh(){
            // check if vao or ibo arent 0 .
            // bind it with this VAO
            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
            glDrawElements(GL_TRIANGLES,indexCount,GL_UNSIGNED_INT,0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
            // then unbind 
            glBindVertexArray(0);
        }
        
        void ClearMesh(){
            if(IBO != 0){
                glDeleteBuffers(1,&IBO);
                IBO = 0;
            }
             if(VBO != 0){
                glDeleteBuffers(1,&VBO);
                VBO = 0;
            }
             if(VAO != 0){
                glDeleteVertexArrays(1,&VAO);
                VAO = 0;
            }
        }

        ~Mesh(){
            ClearMesh();
        }

};