#include "GameObject.h"
#include "MeshComponent.h"
#include "Editor.h"
#pragma comment (lib, "devIL.lib")  
#pragma comment (lib, "ILU.lib") 
#pragma comment (lib, "ILUT.lib")
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
	case Component::Type::MATERIAL:
		if (ret == nullptr)
		{
			ret = new Materialss(this);
			ret->type = _type;
			components.push_back(ret);
			mat = dynamic_cast<Materialss*>(ret);
			OUR_LOG("Material Created!!!");
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
Materialss::Materialss() :Component(nullptr)
{

}
Materialss::Materialss(GameObject* a) : Component(a)
{
	ilInit();
	iluInit();
	ilutInit();
}
void Transform::Update()
{
	UpdateTransform();
}

void Transform::UpdateTransform()
{
	if (rx)
	{
		SetRotation(rotation.x, vec3(1, 0, 0));
		rx = false;
	}
	if (ry)
	{
		SetRotation(rotation.y, vec3(0, 1, 0));
		ry = false;
	}
	if (rz)
	{
		SetRotation(rotation.z, vec3(0, 0, 1));
		rz = false;
	}
	if (updateTransform)
	{
		SetPos(position.x, position.y, position.z);

		Scale(scale.x, scale.y, scale.z);

		updateTransform = false;
	}
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
				if (ImGui::DragFloat("X Rotation", &rotation.x)) rx = true;
				if (ImGui::DragFloat("Y Rotation", &rotation.y)) ry = true;
				if (ImGui::DragFloat("Z Rotation", &rotation.z)) rz = true;
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

bool Materialss::LoadTextures(const char* Filename)
{
	bool ret;
	Meshs* m = new Meshs(nullptr);
	m = dynamic_cast<Meshs*>(parent->GetComponent(Component::Type::MESH));

	ILuint image;
	bool loadTexture = ilLoadImage(Filename);
	ilGenImages(1, &image);//id
	ilBindImage(image);//id

	if (loadTexture)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &m->textureID);//tId
		glBindTexture(GL_TEXTURE_2D, m->textureID);//tiD

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

		m->textureID = ilutGLBindTexImage();
		glBindTexture(GL_TEXTURE_2D, 0);

		OUR_LOG("IMAGE LOADED");
		ret = true;
	}
	else
	{
		OUR_LOG("No image found in this path");
		ret = false;
	}

	ilDeleteImages(1, &m->textureID);//id
	return ret;
}