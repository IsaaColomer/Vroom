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
vec3 Transform::GetPosition()
{
	vec3 positionT;

	positionT = (position.x, position.y, position.z);
	return positionT;
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
				if (ImGui::DragFloat("X Position", &position.x)) updateTransform = true;
				if (ImGui::DragFloat("Y Position", &position.y)) updateTransform = true;
				if (ImGui::DragFloat("Z Position", &position.z)) updateTransform = true;
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

//Material::Material() : Component(nullptr)
//{
//	//transform = IdentityMatrix;
//}
//
//Material::Material(GameObject* _gm) : Component(_gm)
//{
//	//transform = IdentityMatrix;
//	//scale = (1, 1, 1);
//}

//void Material::LoadTextures(const char* Filename)
//{
//	Material mat;
//	ILuint id;
//	ilGenImages(1, &id);
//	ilBindImage(id);
//
//	if (ilLoadImage(Filename))
//	{
//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//		glGenTextures(1, &tId);
//		glBindTexture(GL_TEXTURE_2D, tId);
//
//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//		glGenTextures(1, &bt);
//		glBindTexture(GL_TEXTURE_2D, bt);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
//
//		//tId = ilutGLBindTexImage();
//
//		glBindTexture(GL_TEXTURE_2D, 0);
//	}
//	else
//	{
//		OUR_LOG("No image found in this path");
//	}
//
//	ilDeleteImages(1, &id);
//}