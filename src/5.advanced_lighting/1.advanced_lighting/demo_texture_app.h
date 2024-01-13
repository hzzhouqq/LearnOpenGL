#pragma once
#include <glad/glad.h>

class Camera;
class Shader;

class DemoTextureApp
{
public:
	enum
	{
		INVALID_GL_OBJECT_ID = 0
	};

	DemoTextureApp();
	~DemoTextureApp();

	bool Init();
	void Shutdown();

	const Camera* GetConstCamera() const;
	Camera* GetCamera() const;

protected:
	bool PrepareFloorRenderResource();
	void FreeMemory();
	void FreeGLResource();

	GLuint CreateTexture2D(char const* path);


	bool m_bInited;
	Camera* m_Camera;

	GLuint m_floorVAO;
	GLuint m_floorVBO;
	GLuint m_floorTexture;

	Shader* m_Shader;
};

