#include "GameObject.h"
#include "MeshComponent.h"
#include "Editor.h"

Component* GameObject::CreateComponent(Component::Type _type)
{
	Component* ret = nullptr;
	switch (_type)
	{
	case Component::Type::NONE:
		break;
	case Component::Type::MESH:
		if (ret == nullptr)
		{
			ret = new Meshs();
			ret->type = _type;
			components.push_back(ret);
			OUR_LOG("Mesh Created!!!");
		}
		break;

	case Component::Type::TRANSFORM:
		break;
	default:
		break;
	}

	return ret;
}

void GameObject::Update()
{
	for (auto& i : components)
	{
		i->Update();
		OUR_LOG("b");
	}
	for (auto& i : gameObjects)
	{
		i->Update();
		OUR_LOG("c");
	}
}