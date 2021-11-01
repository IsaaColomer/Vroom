#pragma once
#include "MathGeoLib.h"
#include "Globals.h"
#include <vector>
#include <string>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "ModuleFileSystem.h"
#include "glmath.h"
#include "imgui.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#pragma comment (lib, "External/Assimp/x86-Release/assimp-vc142-mt.lib")

#include "glew.h"

#include <assimp/Importer.hpp>      // C++ importer interface  // Output data structure
#include "MathGeoLib.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "il.h"
#include "ilut.h"
enum ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL,
	NONE,
};

class GameObject;
class Meshs;
class Materialss;
class Component
{
public:
	Component() :  parent(nullptr), type(Type::NONE){}
	Component(GameObject* letsgo) :active(true), parent(letsgo), type(Type::NONE){}
	virtual ~Component() {}
	virtual void Update() {}
	//virtual void Enable();
	//virtual void Disable();

	virtual void Draw() {}
	virtual void LookAtO(vec3& p) {}

	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }
	virtual bool IsEnabled() { return true; }
	GameObject* GetOwner() { return parent; }



	enum class Type
	{
		NONE,
		TRANSFORM,
		MESH,
		MATERIAL,
	};

	Type type;

protected:
	GameObject* parent;
	bool active;
};


class GameObject
{
public:
	GameObject() {}
	GameObject(const char* Name, GameObject* Parent, int uid = -1, bool active = true) :name(Name), parent(Parent), uid(uid), active(true) {}

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
	Meshs* mesh;
	Materialss* mat;

#define INVALID_MATERIAL 0xFFFFFFFF
};

class Materialss : public Component
{
public:
	Materialss();
	Materialss(GameObject* a);
	Materialss(int, const char* Filename) {}

	~Materialss() {}
	void LoadTextures(const char* Filename);

	void Draw() override;
private:
	GLuint tId;
	GLuint bt;
	GLubyte checkerImage[64][64][4];

public:
	const char* pathToImage;
};
class Transform : public Component
{
public:
	Transform();
	Transform(GameObject* gm);
	virtual ~Transform();

	void Draw() override;
	void Update() override;
	void LookAtO(vec3& p) override;
	//void InspectorDraw() override;

	void UpdateTransform();

	static inline Type GetType() { return Type::TRANSFORM; };

	void SetPos(float x, float y, float z);
	void SetRotation(float angle, const vec3& u);
	void Scale(float x, float y, float z);


	mat4x4 GetTransform();
public:
	bool updateTransform;

	bool rx = false;
	bool ry = false;
	bool rz = false;

	mat4x4 transform;

	vec3 position;
	vec3 scale;
	vec3 rotation;
};
