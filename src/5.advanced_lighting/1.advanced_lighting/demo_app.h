#pragma once

class Camera;


class DemoApp
{
public:
	DemoApp();
	~DemoApp();

	bool Init();
	void Shutdown();

	const Camera* GetConstCamera() const;
	Camera* GetCamera() const;
protected:
	void FreeMemory();

	bool m_bInited;
	Camera* m_Camera;
};

