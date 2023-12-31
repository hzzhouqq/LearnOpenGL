#pragma once
#include "common_system.h"

class CommonSystemLocal : public CommonSystem
{
public:

	CommonSystemLocal();
	virtual ~CommonSystemLocal();

	virtual void Error(const char* fmt, ...);

protected:
	enum
	{
		MAX_PRINT_MSG_SIZE = 255
	};

	char m_errorMessage[MAX_PRINT_MSG_SIZE];
};

extern CommonSystemLocal commonSystemLocal;
