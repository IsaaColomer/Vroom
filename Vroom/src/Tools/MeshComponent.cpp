#include "MeshComponent.h"
#include"Globals.h"
#include <assert.h>
#include "Primitive.h"
Meshs::Meshs() : Component(nullptr) {}
Meshs::Meshs(GameObject* _m) : Component(_m)
{
    VB = 0;
    IB = 0;
    TB = 0;
    UB = 0;
    numIndices = 0;
    materialIndex = INVALID_MATERIAL;
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
    if (UB != 0)
    {
        glDeleteBuffers(1, &UB);
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

    glGenBuffers(1, &UB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, UB);
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
bool Meshs::InitMaterials(const aiScene* pScene, const char* Filename)
{
    // Extract the directory part from the file name

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                m_Textures[i] = new Materialss(GL_TEXTURE_2D, Filename);

                if (m_Textures[i]->LoadTextures(Filename))
                {
                    printf("Error loading texture '%s'\n", Filename);
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
                else {
                    printf("Loaded texture '%s'\n", Filename);
                }
            }
        }
    }

    return Ret;
}

//bool Meshs::LoadTexture(const std::string& Filename)
//{
//
//    for (int i = 0; i < 64; i++) {
//        for (int j = 0; j < 64; j++) {
//            int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
//            checkerImage[i][j][0] = (GLubyte)c;
//            checkerImage[i][j][1] = (GLubyte)c;
//            checkerImage[i][j][2] = (GLubyte)c;
//            checkerImage[i][j][3] = (GLubyte)255;
//        }
//    }
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
//        0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
//
//    // ---- Check for error ----
//    glBindTexture(GL_TEXTURE_2D, 0);
//    return true;
//}

bool Meshs::LoadMesh(const char* Filename)
{
    // Release the previously loaded Meshs (if it exists)
    Clear();
    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = aiImportFile(Filename, aiProcessPreset_TargetRealtime_MaxQuality);
    if (pScene) {
    //    Ret = InitFromScene(pScene, Filename);
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename, Importer.GetErrorString());
    }

    return Ret;
}

bool Meshs::InitFromScene(const aiScene* pScene, const char* Filename)
{
    mEntries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    // Initialize the Meshses in the scene one by one
    for (unsigned int i = 0; i < mEntries.size(); i++) {
        const aiMesh* paiMeshs = pScene->mMeshes[i];
        activeMeshes.push_back(paiMeshs);
        InitMesh(i, paiMeshs);
    }

    return InitMaterials(pScene, Filename);
}

void Meshs::InitMesh(unsigned int Index, const aiMesh* paiMeshs)
{
    mEntries[Index].materialIndex = paiMeshs->mMaterialIndex;

    std::vector<float2> texCord;
    std::vector<float3> Vertices;
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
    Transform* t = new Transform(nullptr);    
    t = dynamic_cast<Transform*>(parent->GetComponent(Component::Type::TRANSFORM));

    Materialss* m = new Materialss(nullptr);
    m = dynamic_cast<Materialss*>(parent->GetComponent(Component::Type::MATERIAL));

    glPushMatrix();
    glMultMatrixf(t->transform.M);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0; i < mEntries.size(); i++) {
        glTexCoordPointer(2, GL_FLOAT, 0, NULL);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBindBuffer(GL_ARRAY_BUFFER, mEntries[i].VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float3), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (const GLvoid*)20);

        glDrawElements(GL_TRIANGLES, mEntries[i].numIndices, GL_UNSIGNED_BYTE, NULL);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEntries[i].TB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEntries[i].VB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEntries[i].IB);

        const unsigned int MaterialIndex = mEntries[i].materialIndex;

        if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
           // m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
        }
        glDrawElements(GL_TRIANGLES, mEntries[i].numIndices, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

void Meshs::Update()
{
    Render();
}