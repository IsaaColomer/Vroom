#include "Globals.h"
#include "Primitive.h"
#include "glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>

// ------------------------------------------------------------
namespace Primitive
{
	Primitive::Primitive() : transform(float4x4::identity), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point), id(0), index(0)
	{}

	// ------------------------------------------------------------
	PrimitiveTypes Primitive::GetType() const
	{
		return type;
	}

	// ------------------------------------------------------------
	void Primitive::Render() const
	{
		glPushMatrix();
		glMultMatrixf(*transform.v);

		if (axis == true)
		{
			// Draw Axis Grid
			glLineWidth(2.0f);

			glBegin(GL_LINES);

			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

			glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
			glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

			glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
			glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
			glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
			glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

			glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

			glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
			glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
			glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
			glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

			glEnd();

			glLineWidth(1.0f);
		}

		glColor3f(color.r, color.g, color.b);

		if (wire)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		InnerRender();

		glPopMatrix();
	}

	// ------------------------------------------------------------
	void Primitive::InnerRender() const
	{
		glPointSize(5.0f);

		glBegin(GL_POINTS);

		glVertex3f(0.0f, 0.0f, 0.0f);

		glEnd();

		glPointSize(1.0f);
	}

	// ------------------------------------------------------------
	void Primitive::SetPos(float x, float y, float z)
	{
		transform.Translate(x, y, z);
	}

	// ------------------------------------------------------------
	void Primitive::SetRotation(float angle, const float3& u)
	{
		transform.RotateAxisAngle(u, angle);
	}

	// ------------------------------------------------------------
	void Primitive::Scale(float x, float y, float z)
	{
		transform.Scale(x, y, z);
	}

	// ============================================
	Cube::Cube() : Primitive(), size(1.0f, 1.0f, 1.0f)
	{
		type = PrimitiveTypes::Primitive_Cube;
	}

	Cube::Cube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
	{
		type = PrimitiveTypes::Primitive_Cube;
	}

	void Cube::InnerRender() const
	{
		glGenBuffers(1, (GLuint*)&(id));
		glBindBuffer(GL_ARRAY_BUFFER, id);

		float sx = size.x * 0.5f;
		float sy = size.y * 0.5f;
		float sz = size.z * 0.5f;

		/*std::vector<float> vertices =
		{
			0.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,
			1.0f,1.0f,0.0f,
			0.0f,0.0f,1.0f,
			1.0f,0.0f,1.0f,
			0.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,
		};*/
		std::vector<float> vertices =
		{
			sx,sy,sz,
			sx,sy,-sz,
			sx,-sy,sz,
			sx,-sy,-sz,
			-sx,sy,sz,
			-sx,sy,-sz,
			-sx,-sy,sz,
			-sx,-sy,-sz,
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(index));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		std::vector<uint> indices =
		{
			2,1,0,
			2,3,1,
			4,5,6,
			5,7,6,
			6,3,2,
			6,7,3,
			0,5,4,
			0,1,5,
			3,5,1,
			3,7,5,
			6,0,4,
			6,2,0,
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size() * 3, &indices[0], GL_STATIC_DRAW);

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		// ... bind and use other buffers
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		glDisableClientState(GL_VERTEX_ARRAY);
		/*
		float sx = size.x * 0.5f;
		float sy = size.y * 0.5f;
		float sz = size.z * 0.5f;

		glBegin(GL_QUADS);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-sx, -sy, sz);
		glVertex3f(sx, -sy, sz);
		glVertex3f(sx, sy, sz);
		glVertex3f(-sx, sy, sz);

		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(sx, -sy, -sz);
		glVertex3f(-sx, -sy, -sz);
		glVertex3f(-sx, sy, -sz);
		glVertex3f(sx, sy, -sz);

		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(sx, -sy, sz);
		glVertex3f(sx, -sy, -sz);
		glVertex3f(sx, sy, -sz);
		glVertex3f(sx, sy, sz);

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-sx, -sy, -sz);
		glVertex3f(-sx, -sy, sz);
		glVertex3f(-sx, sy, sz);
		glVertex3f(-sx, sy, -sz);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-sx, sy, sz);
		glVertex3f(sx, sy, sz);
		glVertex3f(sx, sy, -sz);
		glVertex3f(-sx, sy, -sz);

		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(-sx, -sy, -sz);
		glVertex3f(sx, -sy, -sz);
		glVertex3f(sx, -sy, sz);
		glVertex3f(-sx, -sy, sz);

		glEnd();*/
	}

	// SPHERE ============================================
	Sphere::Sphere() : Primitive(), radius(1.0f)
	{
		type = PrimitiveTypes::Primitive_Sphere;
	}

	Sphere::Sphere(float radius, unsigned int rings, unsigned int sectors) : Primitive(), radius(radius), rings(rings), sectors(sectors)
	{
		type = PrimitiveTypes::Primitive_Sphere;
	}
	constexpr const float M_PI = 3.14f;
	constexpr const float M_PI_2 = 6.28f;
	void Sphere::InnerRender() const
	{
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> texcoords;
		std::vector<unsigned short> indices;

		float const R = 1. / (float)(rings - 1);
		float const S = 1. / (float)(sectors - 1);
		int r, s;

		vertices.resize(rings * sectors * 3);
		normals.resize(rings * sectors * 3);
		texcoords.resize(rings * sectors * 2);
		std::vector<GLfloat>::iterator v = vertices.begin();
		std::vector<GLfloat>::iterator n = normals.begin();
		std::vector<GLfloat>::iterator t = texcoords.begin();
		for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			*t++ = s * S;
			*t++ = r * R;

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;

			*n++ = x;
			*n++ = y;
			*n++ = z;
		}

		indices.resize(rings * sectors * 4);
		std::vector<GLushort>::iterator i = indices.begin();
		for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s + 1);
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = (r + 1) * sectors + s;
		}

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		//glTranslatef(0,0,0);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);
		glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
		glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
		glPopMatrix();
	}


	// CYLINDER ============================================
	Cylinder::Cylinder() : Primitive(), radius(1.0f), height(1.0f)
	{
		type = PrimitiveTypes::Primitive_Cylinder;
	}

	Cylinder::Cylinder(float radius, float height) : Primitive(), radius(radius), height(height)
	{
		type = PrimitiveTypes::Primitive_Cylinder;
	}

	void Cylinder::InnerRender() const
	{
		int M_PI = 0;//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		int n = 30;

		// Cylinder Bottom
		glBegin(GL_POLYGON);

		for (int i = 360; i >= 0; i -= (360 / n))
		{
			float a = i * M_PI / 180; // degrees to radians
			glVertex3f(-height * 0.5f, radius * cos(a), radius * sin(a));
		}
		glEnd();

		// Cylinder Top
		glBegin(GL_POLYGON);
		glNormal3f(0.0f, 0.0f, 1.0f);
		for (int i = 0; i <= 360; i += (360 / n))
		{
			float a = i * M_PI / 180; // degrees to radians
			glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
		}
		glEnd();

		// Cylinder "Cover"
		glBegin(GL_QUAD_STRIP);
		for (int i = 0; i < 480; i += (360 / n))
		{
			float a = i * M_PI / 180; // degrees to radians

			glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
			glVertex3f(-height * 0.5f, radius * cos(a), radius * sin(a));
		}
		glEnd();
	}

	// PYRAMID ==================================================
	Pyramid::Pyramid() : Primitive(), height(1), base(1)
	{
		type = PrimitiveTypes::Primitive_Line;
	}

	Pyramid::Pyramid(float height, float base) : Primitive(), height(height), base(base)
	{
		type = PrimitiveTypes::Primitive_Line;
	}

	void Pyramid::InnerRender() const
	{
		glGenBuffers(1, (GLuint*)&(id));
		glBindBuffer(GL_ARRAY_BUFFER, id);

		float half = base / 2;

		std::vector<float> vertices =
		{
			-half,0,-half,
			half,0,-half,
			half,0,half,
			-half,0,half,
			0,height,0,
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(index));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		std::vector<uint> indices =
		{
			0,1,2,
			2,3,0,
			4,1,0,
			4,2,1,
			4,3,2,
			4,0,3,
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size() * 3, &indices[0], GL_STATIC_DRAW);

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		// ... bind and use other buffers
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	// LINE ==================================================
	Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
	{
		type = PrimitiveTypes::Primitive_Line;
	}

	Line::Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
	{
		type = PrimitiveTypes::Primitive_Line;
	}

	void Line::InnerRender() const
	{
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(destination.x, destination.y, destination.z);

		glEnd();

		glLineWidth(1.0f);
	}

	// PLANE ==================================================
	Plane::Plane() : Primitive(), normal(0, 1, 0), constant(1)
	{
		type = PrimitiveTypes::Primitive_Plane;
	}

	Plane::Plane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
	{
		type = PrimitiveTypes::Primitive_Plane;
	}

	void Plane::InnerRender() const
	{
		glLineWidth(1.0f);

		glBegin(GL_LINES);

		float d = 200.0f;

		for (float i = -d; i <= d; i += 1.0f)
		{
			glVertex3f(i, 0.0f, -d);
			glVertex3f(i, 0.0f, d);
			glVertex3f(-d, 0.0f, i);
			glVertex3f(d, 0.0f, i);
		}

		glEnd();
	}

}