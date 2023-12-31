#include "common_system_local.h"

#include <cstdio>
#include <stdarg.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

CommonSystemLocal	commonSystemLocal;
CommonSystem* commonSystem = &commonSystemLocal;

// CommonSystemLocal
CommonSystemLocal::CommonSystemLocal()
{
	m_errorMessage[0] = '\0';
}

CommonSystemLocal::~CommonSystemLocal()
{

}

void CommonSystemLocal::Error(const char* fmt, ...)
{
	static const char* errorCaption = { "Error" };
	va_list		argptr;
	const size_t errSize = sizeof(m_errorMessage);

	va_start(argptr, fmt);
	::vsnprintf(m_errorMessage, errSize - 1, fmt, argptr);
	m_errorMessage[errSize - 1] = '\0';
	va_end(argptr);

	std::cout << "Error: " << m_errorMessage << std::endl;
}
