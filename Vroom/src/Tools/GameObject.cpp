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
	
	case Component::Type::TRANSFORM:
		if (ret == nullptr)
		{
			ret = new Transform(this);
			ret->type = _type;
			components.push_back(ret);
			OUR_LOG("Transform Created!!!");
		}
		break;
	case Component::Type::MESH:
		if (ret == nullptr)
		{
			ret = new Meshs(this);
			ret->type = _type;
			components.push_back(ret);
			mesh = dynamic_cast<Meshs*>(ret);
			OUR_LOG("Mesh Created!!!");
		}
		break;
	default:
		break;
	}

	return ret;
}

Transform::Transform() : Component(nullptr)
{
	transform = IdentityMatrix;
}

Transform::Transform(GameObject* _gm) : Component(_gm)
{
	transform = IdentityMatrix;
	scale = (1, 1, 1);
}

Transform::~Transform()
{
}

void Transform::Update()
{
	UpdateTransform();
}

void Transform::UpdateTransform()
{
	SetPos(position.x, position.y, position.z);

	SetRotation(rotation.x, vec3(1, 0, 0));
	SetRotation(rotation.y, vec3(0, 1, 0));
	SetRotation(rotation.z, vec3(0, 0, 1));

	Scale(scale.x, scale.y, scale.z);

	updateTransform = false;
}

void Transform::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Transform::SetRotation(float angle, const vec3& u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Transform::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

mat4x4 Transform::GetTransform()
{
	return transform;
}

void GameObject::Update()
{
	for (auto& i : components)
	{
		i->Update();
	}
	for (auto& i : gameObjects)
	{
		i->Update();
	}
}

void Transform::Draw()
{
			if (ImGui::CollapsingHeader("Local Transformation"))
			{
				if (ImGui::DragFloat("Position", &position)) updateTransform = true;
				if (ImGui::SliderFloat3("Rotation", &rotation, -180, 180)) updateTransform = true;
				if (ImGui::DragFloat("X scale", &scale.x)) updateTransform = true;
				if (ImGui::DragFloat("Y scale", &scale.y)) updateTransform = true;
				if (ImGui::DragFloat("Z scale", &scale.z)) updateTransform = true;
				if (scale.x < 0)
				{
					scale.x = 0;
				}
				if (scale.y < 0)
				{
					scale.y = 0;
				}
				if (scale.z < 0)
				{
					scale.z = 0;
				}
				//ImGui::Text("Bounding Box: -not generated-");
				//ImGui::Text("Velocity: 0.00 0.00 0.00 (0.00 m/s)");
			}
}
