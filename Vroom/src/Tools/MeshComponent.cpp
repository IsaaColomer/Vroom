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
        mEntries.resize(scene->mNumMeshes);
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            const aiMesh* paiMesh = scene->mMeshes[i];
            InitFromScene(i, paiMesh);
            ret = true;
        }
    }
    else
    {
        OUR_LOG("Error loading image");
    }
    pathToFbx = Filename;
    return ret;
}

void Meshs::Draw()
{

}

void Meshs::Render()
{
    Transform* t = dynamic_cast<Transform*>(parent->GetComponent(Component::Type::TRANSFORM));

    Materialss* mg = dynamic_cast<Materialss*>(parent->GetComponent(Component::Type::MATERIAL));

    glPushMatrix();
    glMultMatrixf(t->transform.M);

    for (unsigned int i = 0; i < mEntries.size(); ++i)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, mEntries[i].vertexBuffer);
        glVertexPointer(3, GL_FLOAT, 0, NULL);

        if (showTextures == true && mg != nullptr)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, mEntries[i].textureBuffer);
            glTexCoordPointer(2, GL_FLOAT, 0, NULL);
        }

        if (showNormals == true)
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, mEntries[i].normalBuffer);
            glNormalPointer(GL_FLOAT, 0, NULL);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEntries[i].indexBuffer);

        glDrawElements(GL_TRIANGLES, mEntries[i].numIndices, GL_UNSIGNED_INT, NULL);

    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();


}
void Meshs::InitFromScene(unsigned int index, const aiMesh* paiMesh)
{
    InitMesh(index, paiMesh);

    OUR_LOG("Textures Initialized Correctly");
}

void Meshs::InitBuffers(const std::vector<vec3>& vertexCoords, const std::vector<vec2>& texCoords, const std::vector<unsigned int>& meshIndexes, const std::vector<vec3>& normals)
{
    numIndices = meshIndexes.size();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCoords.size() * 3, &vertexCoords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size() * 2, &texCoords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshIndexes.size(), &meshIndexes[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Meshs::InitMesh(unsigned int index, const aiMesh* paiMeshs)
{
    std::vector<vec3> vertexCoords;
    std::vector<vec2> texCoords;
    std::vector<unsigned int> meshIndexes;
    meshIndexes.reserve(paiMeshs->mNumFaces * 3);
    std::vector<vec3> normals;
    const aiVector3D Zero(0.0f, 0.0f, 0.0f);
    mEntries[index].materialIndex = paiMeshs->mMaterialIndex;

    for (unsigned int i = 0; i < paiMeshs->mNumVertices; i++)
    {
        const aiVector3D* pPos = &(paiMeshs->mVertices[i]);
        const aiVector3D* pTexCoord = paiMeshs->HasTextureCoords(0) ? &(paiMeshs->mTextureCoords[0][i]) : &Zero;

        vertexCoords.push_back(vec3(pPos->x, pPos->y, pPos->z));
        texCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));
    }

    for (unsigned int i = 0; i < paiMeshs->mNumFaces; i++)
    {
        const aiFace& Face = paiMeshs->mFaces[i];
        meshIndexes.push_back(Face.mIndices[0]);
        meshIndexes.push_back(Face.mIndices[1]);
        meshIndexes.push_back(Face.mIndices[2]);
    }

    if (paiMeshs->HasNormals())
    {
        /*for (unsigned int i = 0; i < paiMeshs->mNumVertices; i++)
        {
            const aiVector3D* v = &(paiMeshs->mVertices[i]);
            const aiVector3D& normal = paiMeshs->mNormals[i];
            vec3 a(v->x, v->y, v->z);
            vec3 b(normal.x, normal.y, normal.z);
            //normals.push_back(a);
            //normals.push_back(b+a);
        }*/
        for (int i = 0; i < meshIndexes.size(); i += 3)
        {
            vec3 v1 = vertexCoords[meshIndexes[i]] - vertexCoords[meshIndexes[i + 1]];
            vec3 v2 = vertexCoords[meshIndexes[i]] - vertexCoords[meshIndexes[i + 2]];
            vec3 normal = normalize(cross(v1, v2));

            vec3 centerPos;
            centerPos.x = (vertexCoords[meshIndexes[i]].x + vertexCoords[meshIndexes[i + 1]].x + vertexCoords[meshIndexes[i + 2]].x) / 3;
            centerPos.y = (vertexCoords[meshIndexes[i]].y + vertexCoords[meshIndexes[i + 1]].y + vertexCoords[meshIndexes[i + 2]].y) / 3;
            centerPos.z = (vertexCoords[meshIndexes[i]].z + vertexCoords[meshIndexes[i + 1]].z + vertexCoords[meshIndexes[i + 2]].z) / 3;

            vec3 line = normal + centerPos;
            vec3 c(centerPos.x, centerPos.y, centerPos.z);
            vec3 d(line.x, line.y, line.z);
            normals.push_back(c);
            normals.push_back(d);
        }
    }

    mEntries[index].InitBuffers(vertexCoords, texCoords, meshIndexes, normals);
}


void Meshs::Update()
{
    Render();
}