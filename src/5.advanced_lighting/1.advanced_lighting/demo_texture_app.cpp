#include "demo_texture_app.h"
#include "common_system.h"

#include <learnopengl/camera.h>
#include <learnopengl/filesystem.h>
#include <stb_image.h>
#include <glm/glm.hpp>

DemoTextureApp::DemoTextureApp()
{
	m_Camera = nullptr;
	m_bInited = false;

	m_floorVAO = INVALID_GL_OBJECT_ID;
	m_floorVBO = INVALID_GL_OBJECT_ID;
	m_floorTexture = INVALID_GL_OBJECT_ID;
	m_Shader = nullptr;
}

DemoTextureApp::~DemoTextureApp()
{

}

void DemoTextureApp::FreeMemory()
{
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}
}

void DemoTextureApp::FreeGLResource()
{
	// unbind opengl resource
	glBindVertexArray(INVALID_GL_OBJECT_ID);
	glBindBuffer(GL_ARRAY_BUFFER, INVALID_GL_OBJECT_ID);

	if (m_floorVAO != INVALID_GL_OBJECT_ID)
	{
		glDeleteVertexArrays(1, &m_floorVAO);
	}
	m_floorVAO = 0;

	if (m_floorVBO != INVALID_GL_OBJECT_ID)
	{
		glDeleteBuffers(1, &m_floorVBO);
	}
	m_floorVBO = 0;

	if (m_floorTexture != INVALID_GL_OBJECT_ID)
	{
		glDeleteTextures(1, &m_floorTexture);
	}
	m_floorTexture = 0;

	if (glGetError() != GL_NO_ERROR)
	{
		commonSystem->Error("DemoTextureApp::FreeGLResource: occur error.");
	}
}

bool DemoTextureApp::Init()
{
	if (m_bInited)
	{
		return false;
	}

	Camera* pCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	m_Camera = pCamera;

	m_bInited = true;
	return true;
}

void DemoTextureApp::Shutdown()
{
	FreeMemory();
}

const Camera* DemoTextureApp::GetConstCamera() const
{
	return m_Camera;
}

Camera* DemoTextureApp::GetCamera() const
{
	return m_Camera;
}

GLuint DemoTextureApp::CreateTexture2D(char const* path)
{
	GLuint textureID = INVALID_GL_OBJECT_ID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		commonSystem->Error("Texture failed to load at path: %s.", path);
		stbi_image_free(data);
	}

	return textureID;
}

bool DemoTextureApp::PrepareFloorRenderResource()
{
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};

	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	if (planeVAO == INVALID_GL_OBJECT_ID ||
		planeVBO == INVALID_GL_OBJECT_ID)
	{
		return false;
	}

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	GLint tex = CreateTexture2D(FileSystem::getPath("resources/textures/wood.png").c_str());
	if (tex == INVALID_GL_OBJECT_ID)
	{
		return false;
	}

	m_floorVAO = planeVAO;
	m_floorVBO = planeVBO;
	m_floorTexture = tex;


	// Shader


	return true;
}


