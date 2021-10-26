#include "GameObject.h"

Meshs::Meshs(const std::string& Filename)
{
    VB = 0;
    IB = 0;
    numIndices = 0;
    materialIndex = INVALID_MATERIAL;

    LoadMesh(Filename);
};

Meshs::~Meshs()
{
    if (VB != 0)
    {
        glDeleteBuffers(1, &VB);
    }

    if (IB != 0)
    {
        glDeleteBuffers(1, &IB);
    }
    if (TB != 0)
    {
        glDeleteBuffers(1, &TB);
    }
}

void Meshs::Init(const std::vector<float3>& Vertices, const std::vector<float2>& textCord,
    const std::vector<unsigned int>& Indices)
{
    numIndices = Indices.size();

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &TB);
    glBindBuffer(GL_ARRAY_BUFFER, TB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &Indices[0], GL_STATIC_DRAW);

    struct aiLogStream stream;
    stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
    aiAttachLogStream(&stream);
}

void Meshs::Clear()
{
    //for (unsigned int i = 0; i < mTextures.size(); i++) {
    //    SAFE_DELETE(mTextures[i]);
    //}
}

bool Meshs::LoadTexture(const std::string& Filename)
{

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
            checkerImage[i][j][0] = (GLubyte)c;
            checkerImage[i][j][1] = (GLubyte)c;
            checkerImage[i][j][2] = (GLubyte)c;
            checkerImage[i][j][3] = (GLubyte)255;
        }
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
        0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

    // ---- Check for error ----
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

bool Meshs::LoadMesh(const std::string& Filename)
{
    // Release the previously loaded Meshs (if it exists)
    Clear();

    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = aiImportFile(Filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

    if (pScene != nullptr) {
        Ret = InitFromScene(pScene, Filename);
        aiReleaseImport(pScene);
    }
    else
    {
       //OUR_LOG("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
    }

    return Ret;
}

bool Meshs::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
    mEntries.resize(pScene->mNumMeshes);
    //m_Textures.resize(pScene->mNumMaterials);

    // Initialize the Meshses in the scene one by one
    for (unsigned int i = 0; i < mEntries.size(); i++) {
        const aiMesh* paiMeshs = pScene->mMeshes[i];
        InitMesh(i, paiMeshs);
    }
    return true;
}

void Meshs::InitMesh(unsigned int Index, const aiMesh* paiMeshs)
{
    mEntries[Index].materialIndex = paiMeshs->mMaterialIndex;

    std::vector<float3> Vertices;
    std::vector<float2> texCord;
    std::vector<unsigned int> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < paiMeshs->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMeshs->mVertices[i]);
        const aiVector3D* pNormal = &(paiMeshs->mNormals[i]);
        const aiVector3D* pTexCoord = paiMeshs->HasTextureCoords(0) ? &(paiMeshs->mTextureCoords[0][i]) : &Zero3D;

        Vertices.push_back(float3(pPos->x, pPos->y, pPos->z));
        texCord.push_back(float2(pTexCoord->x, pTexCoord->y));
    }

    for (unsigned int i = 0; i < paiMeshs->mNumFaces; i++) {
        const aiFace& Face = paiMeshs->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    mEntries[Index].Init(Vertices, texCord, Indices);
}

void Meshs::Render()
{
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    ////-- Buffers--//
    //glBindBuffer(GL_ARRAY_BUFFER, VB);
    //glVertexPointer(3, GL_FLOAT, 0, NULL);

    //glBindBuffer(GL_ARRAY_BUFFER, TB);
    //glTexCoordPointer(2, GL_FLOAT, 0, NULL);

    //glBindTexture(GL_TEXTURE_2D, texture.textureID);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

    ////-- Draw --//
    //glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

    ////-- UnBind Buffers--//
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);

    ////--Disables States--//
    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0; i < mEntries.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, mEntries[i].VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float3), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (const GLvoid*)20);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEntries[i].TB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEntries[i].VB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEntries[i].IB);

        const unsigned int MaterialIndex = mEntries[i].materialIndex;

        glDrawElements(GL_TRIANGLES, mEntries[i].numIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindTexture(GL_TEXTURE_2D, 0);
}