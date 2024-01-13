#pragma once
#include "image_opts.h"
//#ifndef GL_GLEXT_VERSION
//#include <gl/glext.h>
//#endif
#include <glad/glad.h>

#include <vector>
#include <string>


#ifndef GL_EXT_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
#endif

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
// 1、加载图片数据时，把图片数据格式映射到GL内部格式。
class qqImage
{
	friend class qqImageManager;
public:
	explicit qqImage(const char* imageName);
	virtual ~qqImage();

	const char* GetName() const;
	size_t GetIndex() const;
	imageType_t GetType() const;

	// Makes this image active on the current GL texture unit.
	// automatically enables or disables cube mapping
	// May perform file loading if the image was not preloaded.
	void Bind(unsigned int texUnit);

	void ActuallyLoadImage(bool fromBackEnd);
	void PurgeImage();
	void SetTexParameters();

	bool IsLoaded() const { return m_texNum != TEXTURE_NOT_LOADED; }

protected:
	static const GLuint TEXTURE_NOT_LOADED = 0xFFFFFFFF;

	void AllocImage();
	void DeriveOpts();

	std::string m_imageName; // 图片名称，图片文件的相对路径名, @res_path@img/a.jpg
	size_t m_index;
	qqImageOpts m_opts; // Parameters that determine the storage method
	GLuint m_texNum; // GL 纹理对象号
	GLuint m_dataFormat;
	GLuint m_dataType;
	GLuint m_internalFormat;

	// Sampler settings
	textureFilter_t m_filter;
	textureRepeat_t m_repeat;
	textureUsage_t m_usage; // Used to determine the type of compression to use
};

// qqImageManager
class qqImageManager
{
public:
	qqImageManager();
	virtual ~qqImageManager();

	void Init();
	void Shutdown();

	//{@Doom3-BFG comment:
	// If the exact combination of parameters has been asked for already, an existing
	// image will be returned, otherwise a new image will be created.
	// Be careful not to use the same image file with different filter / repeat / etc parameters
	// if possible, because it will cause a second copy to be loaded.
	// If the load fails for any reason, the image will be filled in with the default
	// grid pattern.
	// Will automatically execute image programs if needed.
	//@}
	// 从文件创建一个Image对象。成功后会有相应的图片数据和OpenGL纹理对象。
	qqImage* ImageFromFile(const char* name,
		textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage,
		cubeFiles_t cubeMap = CF_2D);
	// 分配一个新的空数据Image对象。
	qqImage* AllocImage(const char* name);

	qqImage* GetImage(const char* name, textureFilter_t filter = TF_DEFAULT,
		textureRepeat_t repeat = TR_REPEAT, textureUsage_t usage = TD_DEFAULT) const;
	const qqImage* GetConstImage(const char* name, textureFilter_t filter = TF_DEFAULT,
		textureRepeat_t repeat = TR_REPEAT, textureUsage_t usage = TD_DEFAULT) const;
	qqImage* GetImageByIndex(size_t index) const;

protected:
	void FreeMemory();

	std::vector<qqImage*> m_vecImages;
};

void R_LoadImageProgram(const char* name, unsigned char** pic, int* width, int* height, textureUsage_t* usage = NULL);
