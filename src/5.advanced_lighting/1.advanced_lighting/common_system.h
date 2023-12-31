#pragma once

class CommonSystem
{
public:
	virtual ~CommonSystem() {}

	virtual void Error(const char* fmt, ...) = 0;
};

extern CommonSystem* commonSystem;
