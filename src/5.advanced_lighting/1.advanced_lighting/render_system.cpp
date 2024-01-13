#include "render_system_local.h"
#include "common_system.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdio.h>

glconfig_t	glConfig;
static bool r_initialized = false;
qqRenderSystemLocal	renderSystemLocal;
qqRenderSystem* renderSystem = &renderSystemLocal;

// qqRenderSystemLocal
qqRenderSystemLocal::qqRenderSystemLocal()
{

}

qqRenderSystemLocal::~qqRenderSystemLocal()
{

}

void qqRenderSystemLocal::InitOpenGL()
{
	// if OpenGL isn't started, start it now
	if (!R_IsInitialized())
	{
		R_InitOpenGL();
		int err = glGetError();
		if (err != GL_NO_ERROR)
		{
			commonSystem->Printf("glGetError() = 0x%x\n", err);
		}
	}
}

void qqRenderSystemLocal::ShutdownOpenGL()
{
	r_initialized = false;
}


void R_InitOpenGL()
{
	commonSystem->Printf("----- R_InitOpenGL -----\n");

	if (R_IsInitialized())
	{
		commonSystem->Error("R_InitOpenGL called while active");
		return;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		commonSystem->Error("Failed to initialize GLAD");
		return;
	}

	// get our config strings
	glConfig.vendor_string = (const char*)glGetString(GL_VENDOR);
	glConfig.renderer_string = (const char*)glGetString(GL_RENDERER);
	glConfig.version_string = (const char*)glGetString(GL_VERSION);
	glConfig.shading_language_string = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	//glConfig.extensions_string = (const char*)glGetString(GL_EXTENSIONS);

	float glVersion = static_cast<float>(atof(glConfig.version_string));
	float glslVersion = static_cast<float>(atof(glConfig.shading_language_string));
	commonSystem->Printf("OpenGL Version: %3.1f\n", glVersion);
	commonSystem->Printf("OpenGL Vendor : %s\n", glConfig.vendor_string);
	commonSystem->Printf("OpenGL GLSL   : %3.1f\n", glslVersion);

	r_initialized = true;
}

bool R_IsInitialized()
{
	return r_initialized;
}


void GL_CheckErrors()
{
	int		err;
	char	s[64];
	int		i;

	// check for up to 10 errors pending
	for (i = 0; i < 10; i++)
	{
		err = glGetError();

		if (err == GL_NO_ERROR)
		{
			return;
		}

		switch (err)
		{
		case GL_INVALID_ENUM:
			::strcpy(s, "GL_INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:
			::strcpy(s, "GL_INVALID_VALUE");
			break;
		case GL_INVALID_OPERATION:
			::strcpy(s, "GL_INVALID_OPERATION");
			break;
		case GL_STACK_OVERFLOW:
			::strcpy(s, "GL_STACK_OVERFLOW");
			break;
		case GL_STACK_UNDERFLOW:
			::strcpy(s, "GL_STACK_UNDERFLOW");
			break;
		case GL_OUT_OF_MEMORY:
			::strcpy(s, "GL_OUT_OF_MEMORY");
			break;
		default:
			::sprintf_s(s, sizeof(s), "%i", err);
			break;
		}

		commonSystem->Printf("GL_CheckErrors: %s\n", s);
	}

}
