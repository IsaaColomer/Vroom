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
