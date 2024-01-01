#include "image.h"
#include <stb_image.h>

qqImage::qqImage()
{
	m_index = 0;

	m_texNum = TEXTURE_NOT_LOADED;
	m_format = 0;
	m_dataType = 0;

	m_imageWidth = 0;
	m_imageHeight = 0;
}

qqImage::~qqImage()
{

}

const char* qqImage::GetImageName() const
{
	if (m_imageName.size() == 0)
	{
		return nullptr;
	}

	return m_imageName.c_str();
}

size_t qqImage::GetIndex() const
{
	return m_index;
}

void qqImage::Bind(unsigned int texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, m_texNum);
}

imageType_t qqImage::GetType() const
{
	return imageType_t::IT_TEXTURE2D;
}

/*
===============
ActuallyLoadImage

Absolutely every image goes through this path
On exit, the idImage will have a valid OpenGL texture number that can be bound
===============
*/
void qqImage::ActuallyLoadImage(bool fromBackEnd)
{
	//int width, height, nrComponents;
	//unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

	//if (data)
	//{
	//	if (nrComponents == 1)
	//		m_format = GL_RED;
	//	else if (nrComponents == 3)
	//		m_format = GL_RGB;
	//	else if (nrComponents == 4)
	//		m_format = GL_RGBA;
	//	else
	//		m_format = GL_RGBA;

	//	m_imageHeight = height;
	//	m_imageWidth = width;

	//	glBindTexture(GL_TEXTURE_2D, textureID);
	//	glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_format, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	stbi_image_free(data);
	//}
	//else
	//{
	//	commonSystem->Error("Texture failed to load at path: %s.", path);
	//	stbi_image_free(data);
	//}

}
