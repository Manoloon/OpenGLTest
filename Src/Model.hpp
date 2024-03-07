#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Mesh.hpp"
#include "Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
    std::vector<std::shared_ptr<Mesh>> meshList;
    std::vector<std::unique_ptr<Texture>> textureList;
    std::vector<unsigned int> meshToTex;

    public:
    void LoadModel(const std::string& fileName)
    {
        Assimp::Importer importer = Assimp::Importer();
        const aiScene* scene = importer.ReadFile(fileName,aiProcess_Triangulate |
                                        aiProcess_FlipUVs | aiProcess_GenSmoothNormals |
                                        aiProcess_JoinIdenticalVertices); 
        if(!scene)
        {
            printf("Failed to load %s",importer.GetErrorString());
            return;
        }
        LoadNode(scene->mRootNode,scene);
        LoadMaterials(scene);
    }

    void LoadNode(aiNode* node, const aiScene* scene)
    {
        for(size_t i = 0; i< node->mNumMeshes; i++)
        {
            LoadMesh(scene->mMeshes[node->mMeshes[i]],scene);
        }

        for(size_t i =0; i < node->mNumChildren; i++)
        {
            LoadNode(node->mChildren[i],scene);
        }
    }

    void LoadMaterials(const aiScene* scene)
    {
        textureList.resize(scene->mNumMaterials);
        for(size_t i =0; i < scene->mNumMaterials; i++)
        {
            aiMaterial* material = scene->mMaterials[i];
            textureList[i] = nullptr;
            // TODO : refactor this.
            if(material->GetTextureCount(aiTextureType_DIFFUSE))
            {
                aiString path;
                if(material->GetTexture(aiTextureType_DIFFUSE,0,&path) == AI_SUCCESS )
                {
                    // avoiding literal path on the MTL files for the materials. 
                    int idx = std::string(path.data).rfind("\\");
                    std::string filename = std::string(path.data).substr(idx + 1);

                    std::string texPath = std::string("Textures/") + filename;
                    
                    textureList[i] = std::make_unique<Texture>(texPath.c_str());

                    if(!textureList[i]->LoadTexture())
                    {
                        printf("Failed to load texture at: %s\n",texPath);
                        textureList[i] = nullptr;
                    }
                }
            }
            //fallback default texture
            if(!textureList[i])
            {
                textureList[i] = std::make_unique<Texture>("Textures/plain.png");
                textureList[i]->LoadTexture();
            }
        }
    }

    void LoadMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<GLfloat> vertices;
        std::vector<unsigned int> indices;

        for(size_t i = 0; i < mesh->mNumVertices; i++)
        {
            // three first data from the array (x,y,z)
            vertices.insert(std::end(vertices),{mesh->mVertices[i].x,
                                                mesh->mVertices[i].y,
                                                mesh->mVertices[i].z});
            // now we use the UV data 
            //first check that the mesh have a texture.
            if(mesh->mTextureCoords[0])
            {
                vertices.insert(std::end(vertices),{mesh->mTextureCoords[0][i].x,
                                                    mesh->mTextureCoords[0][i].y});
            }
            else
            {
                vertices.insert(std::end(vertices),{0.0f,0.0f});
            }

            // normals : weird thing here is that we need to pass the normals as negative.
            // due to the way the shader handles them.
            vertices.insert(std::end(vertices),{-mesh->mNormals[i].x,
                                                -mesh->mNormals[i].y,
                                                -mesh->mNormals[i].z});
        }
        // faces
        for(size_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(size_t j=0 ; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        // create the mesh and add it to the list.
        auto newMesh = std::make_shared<Mesh>();
        newMesh->CreateMesh(&vertices[0],&indices[0],vertices.size(),indices.size());
        meshList.push_back(newMesh);
        meshToTex.push_back(mesh->mMaterialIndex);
    }

    void RenderModel()
    {
         for(size_t i = 0; i < meshList.size();i++)
         {
            unsigned int materialIndex = meshToTex[i];
            if(materialIndex < textureList.size() && textureList[materialIndex])
            {
                textureList[materialIndex]->UseTexture();
            }
            meshList[i]->RenderMesh();
         }
    }

    void ClearModel()
    {
        for(size_t i = 0; i < meshList.size();i++)
         {
            if(meshList[i])
            {
                meshList[i] = nullptr;
            }
         }

         for(size_t i = 0; i < textureList.size();i++)
         {
            if(textureList[i])
            {
                textureList[i] = nullptr;
            }
         }
    }
};