#pragma once
#include "common_system.h"


class CommonSystemLocal : public CommonSystem
{
public:

	CommonSystemLocal();
	virtual ~CommonSystemLocal();

	virtual void Error(const char* fmt, ...);
	virtual void Printf(const char* fmt, ...);
	virtual void Warning(const char* fmt, ...);

	virtual void SetRescourcePath(const char* res);
	virtual const char* GetRescourcePath() const;
	virtual bool RelativeToAbsolutePath(const char* relativePath, std::string& result) const;

protected:
	enum
	{
		MAX_PRINT_MSG_SIZE = 255,
		MAX_PATH_SIZE = 500
	};

	char m_errorMessage[MAX_PRINT_MSG_SIZE];
	char m_message[MAX_PRINT_MSG_SIZE];
	char m_resPath[MAX_PATH_SIZE];
	char m_resPathTag[MAX_PATH_SIZE];
};

extern CommonSystemLocal commonSystemLocal;
