#pragma once
#include "MathGeoLib.h"
#include "ModuleFileSystem.h"
#include <vector>
#include <string>

enum ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL,
};

class GameObject;
class Component
{
public:
	Component();
	~Component();
	virtual void Update();
	virtual void Enable();
	virtual void Disable();

public:

	ComponentType type;
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
	Meshs();
	~Meshs();

public:
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
	GameObject(std::string name) : name(name) {}
	~GameObject();

	void Update();
	Component* CreateComponent(ComponentType type);

public:
	bool active;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> gameObjects;
	GameObject* parent;

};