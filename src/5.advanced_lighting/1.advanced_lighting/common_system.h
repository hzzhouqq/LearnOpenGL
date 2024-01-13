#pragma once

#include <string>

class CommonSystem
{
public:
	virtual ~CommonSystem() {}

	// Path functions
	virtual void SetRescourcePath(const char* res) = 0;
	virtual const char* GetRescourcePath() const = 0;
	virtual bool RelativeToAbsolutePath(const char* relativePath, std::string& result) const = 0;

	// trace functions
	virtual void Error(const char* fmt, ...) = 0;
	virtual void Warning(const char* fmt, ...) = 0;

};

extern CommonSystem* commonSystem;
