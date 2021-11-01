#pragma once
#include "GameObject.h"
#include "glmath.h"
#include <vector>
class Component;
class Transform;
class Meshs : public Component
{
public:
	Meshs();
	Meshs(GameObject* _m);
	~Meshs();

	void Update() override;

public:
	bool LoadMesh(const char*Filename);

	void Render();

	void Init();

	void Draw() override;

	GLubyte checkerImage[640][426][4];
	void InitFromScene(const aiMesh* pScene);
	void InitMesh(const aiMesh* paiMesh);
	GLuint VB;
	GLuint TB;
	GLuint IB;
	GLuint UB;
	std::vector<Meshs> mEntries;
	std::vector<Materialss*> m_Textures;
	std::vector<const aiMesh*> activeMeshes;

	GLuint textureID;
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint indexBuffer;

	unsigned int numIndices;
	unsigned int materialIndex;

	std::vector<vec3> vertexCoords;
	std::vector<vec2> texCoords;
	std::vector<unsigned int> meshIndexes;
	//bool InitMaterials(const aiScene* pScene, const char* Filename);
	void Clear();
};