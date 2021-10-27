#pragma once
#include "GameObject.h"
class Component;
class Meshs : public Component
{
public:
	Meshs() {}
	Meshs(const char* Filename);
	~Meshs();

	void Update() override;

public:
	void LoadMesh(const std::string& Filename);
	void Render();
	GLuint texture;
	bool LoadTexture(const std::string& Filename);
	GLuint meshTextureID;
	void Init(const std::vector<float3>& Vertices, const std::vector<float2>& textCord,
		const std::vector<unsigned int>& Indices);
	GLuint textureID;
	GLubyte checkerImage[640][426][4];
	void InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	GLuint VB;
	GLuint TB;
	GLuint IB;
	std::vector<Meshs> mEntries;
	std::vector<const aiMesh*> activeMeshes;
	unsigned int numIndices;
	unsigned int materialIndex;
	//bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();
};