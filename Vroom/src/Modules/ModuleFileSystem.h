#pragma once
#include <vector>
#include <string>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "il.h"
#pragma comment (lib, "External/Assimp/x86-Release/assimp-vc142-mt.lib")

#include "glew.h"

#include <assimp/Importer.hpp>      // C++ importer interface  // Output data structure
#include "MathGeoLib.h"
#include <gl/GL.h>
#include <gl/GLU.h>

struct Vertex
{
    float3 m_pos;
    float2 m_tex;
    float3 m_normal;

    Vertex() {}

    Vertex(const float3& pos, const float2& tex, const float3& normal)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = normal;
    }

    Vertex(const float3& pos, const float2& tex)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = float3(0.0f, 0.0f, 0.0f);
    }
};

class Mesh
{
public:
    Mesh();

    ~Mesh();

    bool LoadMesh(const std::string& Filename);

    void Render();
    GLuint texture;
    bool LoadTexture(const std::string& Filename);
    GLuint meshTextureID;
    void Init(const std::vector<float3>& Vertices,const std::vector<float2>& textCord,
        const std::vector<unsigned int>& Indices);
    GLuint textureID;
    GLubyte checkerImage[640][426][4];

private:
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    GLuint VB;
    GLuint TB;
    GLuint IB;
    std::vector<Mesh> mEntries;
    unsigned int numIndices;
    unsigned int materialIndex;
    //bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF



   //std::vector<Texture*> m_Textures;
};