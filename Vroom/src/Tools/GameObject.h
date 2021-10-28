#pragma once
#include "MathGeoLib.h"
#include "Application.h"
#include "Globals.h"
#include <vector>
#include <string>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "il.h"
#include "ModuleFileSystem.h"
#include "glmath.h"
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
	Component() : active(true), parent(nullptr), type(Type::NONE) {}
	Component(GameObject* letsgo) : active(true), parent(letsgo), type(Type::NONE) {}
	virtual ~Component() {}
	virtual void Update() {}
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
		MESH,
	};

	Type type;

protected:

	bool active;
	GameObject* parent;


};

class Transform : public Component
{
public:
	Transform();
	Transform(GameObject* gm);
	virtual ~Transform();

	void Update() override;
	//void InspectorDraw() override;

	void UpdateTransform();

	static inline Type GetType() { return Type::TRANSFORM; };

	void SetPos(float x, float y, float z);
	void SetRotation(float angle, const vec3& u);
	void Scale(float x, float y, float z);


	mat4x4 GetTransform();

public:
	bool updateTransform;

	mat4x4 transform;

	vec3 position, scale, rotation;
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

	
	Component* CreateComponent(Component::Type type);
	Component* GetComponent(Component::Type xdtype)
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

	void Update();
public:
	bool active;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> gameObjects;
	int uid;
	GameObject* parent;
#define INVALID_MATERIAL 0xFFFFFFFF
};