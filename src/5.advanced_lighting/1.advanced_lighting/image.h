#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>

typedef enum
{
	TF_LINEAR,
	TF_NEAREST,
	TF_DEFAULT				// use the user-specified r_textureFilter
} textureFilter_t;

typedef enum
{
	TR_REPEAT,
	TR_CLAMP,
	TR_CLAMP_TO_ZERO,		// guarantee 0,0,0,255 edge for projected textures
	TR_CLAMP_TO_ZERO_ALPHA	// guarantee 0 alpha edge for projected textures
} textureRepeat_t;

// How is this texture used?  Determines the storage and color format
typedef enum
{
	TD_SPECULAR,			// may be compressed, and always zeros the alpha channel
	TD_DIFFUSE,				// may be compressed
	TD_DEFAULT,				// generic RGBA texture (particles, etc...)
	TD_BUMP,				// may be compressed with 8 bit lookup
	TD_FONT,				// Font image
	TD_LIGHT,				// Light image
	TD_LOOKUP_TABLE_MONO,	// Mono lookup table (including alpha)
	TD_LOOKUP_TABLE_ALPHA,	// Alpha lookup table with a white color channel
	TD_LOOKUP_TABLE_RGB1,	// RGB lookup table with a solid white alpha
	TD_LOOKUP_TABLE_RGBA,	// RGBA lookup table
	TD_COVERAGE,			// coverage map for fill depth pass when YCoCG is used
	TD_DEPTH,				// depth buffer copy for motion blur
} textureUsage_t;

typedef enum
{
	CF_2D,			// not a cube map
	CF_NATIVE,		// _px, _nx, _py, etc, directly sent to GL
	CF_CAMERA		// _forward, _back, etc, rotated and flipped as needed before sending to GL
} cubeFiles_t;

typedef enum
{
	IT_TEXTURE2D
} imageType_t;

// qqImage
class qqImage
{
	friend class qqImageManager;
public:
	qqImage();
	virtual ~qqImage();

	const char* GetImageName() const;
	size_t GetIndex() const;
	imageType_t GetType() const;

	// Makes this image active on the current GL texture unit.
	// automatically enables or disables cube mapping
	// May perform file loading if the image was not preloaded.
	void Bind(unsigned int texUnit);

	void ActuallyLoadImage(bool fromBackEnd);

	bool IsLoaded() const { return m_texNum != TEXTURE_NOT_LOADED; }
protected:
	static const GLuint TEXTURE_NOT_LOADED = 0xFFFFFFFF;

	std::string m_imageName;
	size_t m_index;

	GLuint m_texNum;
	GLuint m_format;
	GLuint m_dataType;

	unsigned int m_imageWidth;
	unsigned int m_imageHeight;
};

// qqImageManager
class qqImageManager
{
public:
	qqImageManager();
	virtual ~qqImageManager();

	void Init();
	void Shutdown();

	qqImage* ImageFromFile(const char* name,
		textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage,
		cubeFiles_t cubeMap = CF_2D);

	qqImage* AllocImage(const char* name);

	qqImage* GetImageByName(const char* name) const;
	const qqImage* GetConstImageByName(const char* name) const;
	qqImage* GetImageByIndex(size_t index) const;

protected:
	void FreeMemory();

	std::vector<qqImage*> m_vecImages;
};
