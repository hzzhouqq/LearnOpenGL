#pragma once
#include "render_system.h"

class qqRenderSystemLocal : public qqRenderSystem
{
public:
	qqRenderSystemLocal();
	virtual ~qqRenderSystemLocal();

	virtual void InitOpenGL();
	virtual void ShutdownOpenGL();
};

extern qqRenderSystemLocal renderSystemLocal;
