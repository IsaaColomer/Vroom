#include "MeshComponent.h"
#include"Globals.h"
#include <assert.h>
#include "Primitive.h"
Meshs::Meshs() : Component(nullptr) {}
Meshs::Meshs(GameObject* _m) : Component(_m)
{
    vertexBuffer = 0;
    indexBuffer = 0;
    textureBuffer = 0;
    UB = 0;
    numIndices = 0;
    materialIndex = INVALID_MATERIAL;
};

Meshs::~Meshs()
{
    if (VB != 0)
    {
        glDeleteBuffers(1, &vertexBuffer);
    }

    if (IB != 0)
    {
        glDeleteBuffers(1, &indexBuffer);
    }
    if (TB != 0)
    {
        glDeleteBuffers(1, &textureBuffer);
    }
    if (UB != 0)
    {
        glDeleteBuffers(1, &UB);
    }
}

void Meshs::Init()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCoords.size()*3, &vertexCoords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size()*2, &texCoords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshIndexes.size(), &meshIndexes[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Meshs::Clear()
{
    //for (unsigned int i = 0; i < mTextures.size(); i++) {
    //    SAFE_DELETE(mTextures[i]);
    //}
}

bool Meshs::LoadMesh(const char* Filename)
{
    bool ret = false;
    const aiScene* scene = aiImportFile(Filename, aiProcessPreset_TargetRealtime_MaxQuality);

    if (scene)
    {
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            InitFromScene(scene->mMeshes[i]);
            ret = true;
        }
    }
    else
    {
        OUR_LOG("Error loading image");
    }

    return ret;
}

void Meshs::Render()
{
    Transform* t = new Transform(nullptr);
    t = dynamic_cast<Transform*>(parent->GetComponent(Component::Type::TRANSFORM));

    glPushMatrix();
    glMultMatrixf(t->transform.M);

    glBindTexture(GL_TEXTURE_2D, textureBuffer);

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, NULL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textureID);


    if (texCoords.size() > 0)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
        glTexCoordPointer(2, GL_FLOAT, 0, NULL);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glDrawElements(GL_TRIANGLES, meshIndexes.size(), GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (texCoords.size() > 0)  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}
void Meshs::InitFromScene(const aiMesh* paiMesh)
{
    unsigned int verticesNum = 0;
    unsigned int indicesNum = 0;

    materialIndex = paiMesh->mMaterialIndex;
    meshIndexes.reserve(paiMesh->mNumFaces * 3);

    verticesNum += paiMesh->mNumVertices;
    indicesNum += meshIndexes.size();

    InitMesh(paiMesh);

    Init();

    OUR_LOG("Textures Initialized Correctly");
}

void Meshs::InitMesh(const aiMesh* paiMeshs)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < paiMeshs->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMeshs->mVertices[i]);
        const aiVector3D* pTexCoord = paiMeshs->HasTextureCoords(0) ? &(paiMeshs->mTextureCoords[0][i]) : &Zero3D;

        vertexCoords.push_back(vec3(pPos->x, pPos->y, pPos->z));
        texCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));
    }

    for (unsigned int i = 0; i < paiMeshs->mNumFaces; i++) {
        const aiFace& Face = paiMeshs->mFaces[i];
        meshIndexes.push_back(Face.mIndices[0]);
        meshIndexes.push_back(Face.mIndices[1]);
        meshIndexes.push_back(Face.mIndices[2]);
    }
}


void Meshs::Update()
{
    Render();
}