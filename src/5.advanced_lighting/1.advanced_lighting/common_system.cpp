#include "common_system_local.h"

#include <cstdio>
#include <stdarg.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

CommonSystemLocal commonSystemLocal;
CommonSystem* commonSystem = &commonSystemLocal;

// CommonSystemLocal
CommonSystemLocal::CommonSystemLocal()
{
	m_errorMessage[0] = '\0';
	m_resPath[0] = '\0';
	::snprintf(m_resPathTag, sizeof(m_resPathTag), "@res_path@");
}

CommonSystemLocal::~CommonSystemLocal()
{

}

void CommonSystemLocal::Error(const char* fmt, ...)
{
	va_list		argptr;
	const size_t errSize = sizeof(m_errorMessage);

	va_start(argptr, fmt);
	::vsnprintf(m_errorMessage, errSize - 1, fmt, argptr);
	m_errorMessage[errSize - 1] = '\0';
	va_end(argptr);

	std::cout << "Error: " << m_errorMessage << std::endl;
}

void CommonSystemLocal::Printf(const char* fmt, ...)
{
	va_list		argptr;
	const size_t size = sizeof(m_message);

	va_start(argptr, fmt);
	::vsnprintf(m_message, size - 1, fmt, argptr);
	m_message[size - 1] = '\0';
	va_end(argptr);

	std::cout << m_message;
}

void CommonSystemLocal::Warning(const char* fmt, ...)
{
	static const char* warningCaption = { "Warning" };
	va_list		argptr;
	const size_t size = sizeof(m_message);

	va_start(argptr, fmt);
	::vsnprintf(m_message, size - 1, fmt, argptr);
	m_message[size - 1] = '\0';
	va_end(argptr);

	std::cout << "Warning: " << m_message << std::endl;
}

void CommonSystemLocal::SetRescourcePath(const char* res)
{
	::snprintf(m_resPath, sizeof(m_resPath), "%s", res);
}

const char* CommonSystemLocal::GetRescourcePath() const
{
	return m_resPath;
}

bool CommonSystemLocal::RelativeToAbsolutePath(const char* relativePath, std::string& result) const
{
	// @res_path@img/a.jpg ==> c:/doom/res/img/a.jpg

	const char* s1 = ::strstr(relativePath, m_resPathTag);
	if (s1 == nullptr)
	{
		return false;
	}

	char path[MAX_PATH_SIZE];
	::sprintf_s(path, sizeof(path), "%s/%s",
		m_resPath, relativePath + ::strnlen_s(m_resPathTag, sizeof(m_resPathTag)));
	result = path;
	return true;
}
