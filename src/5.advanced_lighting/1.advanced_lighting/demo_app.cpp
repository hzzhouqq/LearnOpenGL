#include "demo_app.h"
#include <learnopengl/camera.h>

#include <glm/glm.hpp>

DemoApp::DemoApp()
{
	m_Camera = nullptr;
	m_bInited = false;
}

DemoApp::~DemoApp()
{

}

void DemoApp::FreeMemory()
{
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}
}

bool DemoApp::Init()
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

void DemoApp::Shutdown()
{
	FreeMemory();
}

const Camera* DemoApp::GetConstCamera() const
{
	return m_Camera;
}

Camera* DemoApp::GetCamera() const
{
	return m_Camera;
}
