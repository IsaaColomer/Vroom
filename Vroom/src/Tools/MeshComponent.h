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
	GLuint texture;
	bool LoadTexture(const std::string& Filename);
	GLuint meshTextureID;
	void Init(const std::vector<float3>& Vertices, const std::vector<float2>& textCord,
		const std::vector<unsigned int>& Indices);
	GLuint textureID;
	GLubyte checkerImage[640][426][4];
	bool InitFromScene(const aiScene* pScene, const char* Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	GLuint VB;
	GLuint TB;
	GLuint IB;
	GLuint UB;
	std::vector<Meshs> mEntries;
	std::vector<Materialss*> m_Textures;
	std::vector<const aiMesh*> activeMeshes;
	unsigned int numIndices;
	unsigned int materialIndex;
	bool InitMaterials(const aiScene* pScene, const char* Filename);
	void Clear();
};