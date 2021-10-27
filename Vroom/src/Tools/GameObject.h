#pragma once
#include "MathGeoLib.h"
#include "Application.h"
#include "Globals.h"
#include <vector>
#include <string>
#include <vector>
#include <string>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "il.h"
#include "ModuleFileSystem.h"
#pragma comment (lib, "External/Assimp/x86-Release/assimp-vc142-mt.lib")

#include "glew.h"

#include <assimp/Importer.hpp>      // C++ importer interface  // Output data structure
#include "MathGeoLib.h"
#include <gl/GL.h>
#include <gl/GLU.h>

enum ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL,
	NONE,
};

class GameObject;
class Component
{
public:
	Component() : active(true), parent(nullptr), type(ComponentType::NONE) {}
	Component(GameObject* letsgo) : active(true), parent(letsgo), type(ComponentType::NONE) {}
	virtual ~Component() {}
	//virtual void Update();
	//virtual void Enable();
	//virtual void Disable();

	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }
	virtual bool IsEnabled() { return active; }
	GameObject* GetOwner() { return parent; }

	enum class Type
	{
		NONE,
		TRANSFORM,
	};

	ComponentType type;

protected:

	bool active;
	GameObject* parent;

};

class Transform : public Component
{
public:
	Transform();
	~Transform();

public:
	aiVector3D  position;
	aiVector3D  scale;
	aiQuaternion rotation;
};

class Meshs : public Component
{
public:
	Meshs() {}
	Meshs(const std::string& Filename);
	~Meshs();

public:
	bool LoadMesh(const std::string& Filename);
	void Render();
	GLuint texture;
	bool LoadTexture(const std::string& Filename);
	GLuint meshTextureID;
	void Init(const std::vector<float3>& Vertices, const std::vector<float2>& textCord,
		const std::vector<unsigned int>& Indices);
	GLuint textureID;
	GLubyte checkerImage[640][426][4];
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	GLuint VB;
	GLuint TB;
	GLuint IB;
	std::vector<Meshs> mEntries;
	unsigned int numIndices;
	unsigned int materialIndex;
	//bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();
};

class Material : public Component
{
public:
	Material();
	~Material();

public:
};

class GameObject
{
public:
	GameObject() {}
	GameObject(const char* Name, GameObject* Parent, int uid = -1) :name(Name), parent(Parent), uid(uid) {}

	~GameObject();

	void Update();
	
	Component* CreateComponent(ComponentType type);
	Component* GetComponent(ComponentType xdtype)
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			if (components.at(i)->type == xdtype)
			{
				return components.at(i);
			}
		}

		return nullptr;
	}

	void Enable() { active = true; }
	void Disable() { active = false; }
	bool IsEnabled() { return active; }

public:
	bool active;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> gameObjects;
	int uid;
	GameObject* parent;
#define INVALID_MATERIAL 0xFFFFFFFF
};