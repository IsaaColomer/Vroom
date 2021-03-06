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
			meshss = dynamic_cast<Meshs*>(ret);
			OUR_LOG("Mesh Created!!!");
		}
		break;
	case Component::Type::MATERIAL:
		if (ret == nullptr)
		{
			ret = new Materialss(this);
			ret->type = _type;
			active = true;
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
void Transform::SetRotation(float angle, const vec3& rotation)
{
	transform.rotate(angle, rotation);
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

void Transform::LookAtO(vec3& p)
{
	p = position;
}
void GameObject::Update()
{
	for (auto& i : components)
	{
		if (i->IsEnabled())
		{
			i->Update();
		}
	}
	for (auto& i : gameObjects)
	{
		if (i->IsEnabled())
		{
			i->Update();
		}
	}
}

void Transform::Draw()
{
	if (ImGui::CollapsingHeader("Local Transformation"))
	{
		ImGui::Text("Position");
		if (ImGui::DragFloat("Xt", &position.x)) updateTransform = true;
		if (ImGui::DragFloat("Yt", &position.y)) updateTransform = true;
		if (ImGui::DragFloat("Zt", &position.z)) updateTransform = true;
		ImGui::Text("Roation");
		if (ImGui::DragFloat("Xr", &rotation.x)) rx = true;
		if (ImGui::DragFloat("Yr", &rotation.y)) ry = true;
		if (ImGui::DragFloat("Zr", &rotation.z)) rz = true;
		ImGui::Text("Scale");
		if (ImGui::DragFloat("Xs", &scale.x)) updateTransform = true;
		if (ImGui::DragFloat("Ys", &scale.y)) updateTransform = true;
		if (ImGui::DragFloat("Zs", &scale.z)) updateTransform = true;
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
	}
}

void Materialss::Draw()
{

}


void Materialss::LoadTextures(const char* Filename)
{
	Meshs* m = new Meshs(nullptr);
	m = dynamic_cast<Meshs*>(parent->GetComponent(Component::Type::MESH));

	ILuint id = 0;
	ilGenImages(1, &id);
	ilBindImage(id);

	bool loaded = ilLoadImage(Filename);

	if (loaded)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		m->textureID = ilutGLBindTexImage();

		glBindTexture(GL_TEXTURE_2D, m->textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, NULL);

		ilDeleteImages(1, &id);
		glBindTexture(GL_TEXTURE_2D, 0);
		pathToImage = Filename;
		inpectorImage = m->textureID;
		tW = ilGetInteger(IL_IMAGE_WIDTH);
		tH = ilGetInteger(IL_IMAGE_HEIGHT);
	}
	else
	{
		OUR_LOG("Error loading the image");
	}
}