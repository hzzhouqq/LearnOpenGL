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
	qqImage* image = GetImage(name, filter, repeat, usage);
	if (image && !image->IsLoaded())
	{
		image->ActuallyLoadImage(false);
		return image;
	}

	image = AllocImage(name);
	image->ActuallyLoadImage(false);

	return image;
}

qqImage* qqImageManager::GetImage(const char* name, textureFilter_t filter /*= TF_DEFAULT*/,
	textureRepeat_t repeat/* = TR_REPEAT*/, textureUsage_t usage /*= TD_DEFAULT*/) const
{
	const qqImage* image = GetConstImage(name, filter, repeat, usage);

	return const_cast<qqImage*>(image);
}

/*
// qqImageManager::GetConstImage
//
// 图片管理器中可能存在多个同名的图片对象，因为同一份图片数据的其它参数，
// 例如的filter, repeat, usage 都是有可能不同的。
//
*/
const qqImage* qqImageManager::GetConstImage(const char* name, textureFilter_t filter/*= TF_DEFAULT*/,
	textureRepeat_t repeat/* = TR_REPEAT*/, textureUsage_t usage/*= TD_DEFAULT*/) const
{
	for (size_t i = 0; i < m_vecImages.size(); i++)
	{
		const qqImage* image = m_vecImages[i];
		const char* imageName = image->GetName();

		if (image && imageName != nullptr)
		{
			if (::_stricmp(imageName, name) == 0 && filter == image->m_filter
				&& repeat == image->m_repeat && usage == image->m_usage)
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
	if (GetConstImage(name) != nullptr)
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

	// Alloc image and assign to container
	qqImage* newImage = new qqImage(name);
	newImage->m_index = nFoundIndex;
	newImage->m_imageName = name;
	m_vecImages[nFoundIndex] = newImage;

	return newImage;
}
