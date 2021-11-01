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

	GLubyte checkerImage[64][64][4];
	void InitFromScene(unsigned int index, const aiMesh* pScene);
	void InitMesh(unsigned int index, const aiMesh* paiMesh);
	void Meshs::InitBuffers(const std::vector<vec3>& vertexCoords, const std::vector<vec2>& texCoords, const std::vector<unsigned int>& meshIndexes, const std::vector<vec3>& normals);
	GLuint VB;
	GLuint TB;
	GLuint IB;
	GLuint UB;
	std::vector<Meshs> mEntries;
	std::vector<Materialss*> mTextures;
	std::vector<const aiMesh*> activeMeshes;

	GLuint textureID;
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint indexBuffer;
	GLuint normalBuffer;
	
	bool showTextures = true;
	const char* pathToFbx;
	unsigned int numIndices;
	unsigned int materialIndex;
	bool showMesh = true;
	bool showNormals = false;

	
	//bool InitMaterials(const aiScene* pScene, const char* Filename);
	void Clear();
};