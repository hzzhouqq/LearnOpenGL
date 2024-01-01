#include "image.h"

#include "common_system.h"

// qqImageManager
qqImageManager::qqImageManager()
{
	m_vecImages.resize(256, nullptr);
}

qqImageManager::~qqImageManager()
{

}

void qqImageManager::Init()
{

}

void qqImageManager::Shutdown()
{
	FreeMemory();
}

void qqImageManager::FreeMemory()
{
	for (size_t i = 0; i < m_vecImages.size(); i++)
	{
		qqImage* p = m_vecImages[i];
		if (p != nullptr)
		{
			delete p;
		}
	}
	m_vecImages.clear();

}

qqImage* qqImageManager::ImageFromFile(const char* name,
	textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage,
	cubeFiles_t cubeMap/* = CF_2D*/)
{
	qqImage* image = GetImageByName(name);
	if (image && !image->IsLoaded())
	{
		image->ActuallyLoadImage(false);
		return image;
	}

	image = AllocImage(name);
	image->ActuallyLoadImage(false);

	return image;
}

qqImage* qqImageManager::GetImageByName(const char* name) const
{
	const qqImage* image = GetConstImageByName(name);

	return const_cast<qqImage*>(image);
}

const qqImage* qqImageManager::GetConstImageByName(const char* name) const
{
	for (size_t i = 0; i < m_vecImages.size(); i++)
	{
		const qqImage* image = m_vecImages[i];
		const char* imageName = image->GetImageName();

		if (image && imageName != nullptr)
		{
			if (::_stricmp(imageName, name) == 0)
			{
				return image;
			}
		}
	}

	return nullptr;
}

qqImage* qqImageManager::GetImageByIndex(size_t index) const
{
	return nullptr;
}

qqImage* qqImageManager::AllocImage(const char* name)
{
	// Make sure the image is unique
	if (GetConstImageByName(name) != nullptr)
	{
		commonSystem->Error("imageManager: image %s exists, allocImage failed.", name);
		return nullptr;
	}

	// Find empty slot
	int nFoundIndex = -1;
	for (size_t i = 0; i < m_vecImages.size(); i++)
	{
		const qqImage* image = m_vecImages[i];
		if (image == nullptr)
		{
			nFoundIndex = static_cast<int>(i);
			break;
		}
	}
	
	if (nFoundIndex < 0)
	{
		m_vecImages.push_back(nullptr);
		nFoundIndex = static_cast<int>(m_vecImages.size()) - 1;
	}

	// Alloc image and assign to vector
	qqImage* newImage = new qqImage();
	newImage->m_index = nFoundIndex;
	newImage->m_imageName = name;
	m_vecImages[nFoundIndex] = newImage;

	return newImage;
}
