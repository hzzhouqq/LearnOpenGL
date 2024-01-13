#include "image.h"
#include "render_system.h"
#include "common_system.h"

#include <stb_image.h>
#include <assert.h>
#include <algorithm>


int BitsForFormat(textureFormat_t format)
{
	switch (format)
	{
		case FMT_NONE:		return 0;
		case FMT_RGBA8:		return 32;
		case FMT_XRGB8:		return 32;
		case FMT_RGB565:	return 16;
		case FMT_L8A8:		return 16;
		case FMT_ALPHA:		return 8;
		case FMT_LUM8:		return 8;
		case FMT_INT8:		return 8;
		case FMT_DXT1:		return 4;
		case FMT_DXT5:		return 8;
		case FMT_DEPTH:		return 32;
		case FMT_X16:		return 16;
		case FMT_Y16_X16:	return 32;
		default:
			assert(0);
			return 0;
	}
}

void R_LoadImageProgram(const char* name, unsigned char** pic, int* width, int* height, textureUsage_t* usage)
{
	*usage = TD_DEFAULT;
}

// qqImage
qqImage::qqImage(const char* imageName)
{
	m_imageName = imageName;
	m_index = 0;

	m_texNum = TEXTURE_NOT_LOADED;
	m_dataFormat = 0;
	m_dataType = 0;
	m_internalFormat = 0;

	m_filter = TF_DEFAULT;
	m_repeat = TR_REPEAT;
	m_usage = TD_DEFAULT;
}

qqImage::~qqImage()
{

}

const char* qqImage::GetName() const
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

//{@Doom3 BFG Comment:
//===============
//ActuallyLoadImage
//
//Absolutely every image goes through this path
//On exit, the idImage will have a valid OpenGL texture number that can be bound
//===============
//@}
void qqImage::ActuallyLoadImage(bool fromBackEnd)
{
	R_LoadImageProgram(GetName(), NULL, NULL, NULL, &m_usage);
	DeriveOpts();

	////{@
	//int width, height, nrComponents;
	//unsigned char* pic = stbi_load(path, &width, &height, &nrComponents, 0);

	//// 加载图片数据失败，将纹理对象中的数据初始化为零，保证数据的合法性。
	//if (pic == nullptr)
	//{
	//	commonSystem->Warning("Couldn't load image: %s .", GetName());
	//	
	//	m_opts.m_width = 8;
	//	m_opts.m_height = 8;
	//	m_opts.m_numLevels = 1;
	//	DeriveOpts();
	//	AllocImage();

	//		// clear the data so it's not left uninitialized
	//		idTempArray<byte> clear(opts.width * opts.height * 4);
	//		memset(clear.Ptr(), 0, clear.Size());
	//		for (int level = 0; level < opts.numLevels; level++) {
	//			SubImageUpload(level, 0, 0, 0, opts.width >> level, opts.height >> level, clear.Ptr());
	//		}

	//		return;
	//	}

	//	return;
	//}
	//

	//if (nrComponents == 1)
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
	//	
	////@}

}

void qqImage::SetTexParameters()
{
	int target = GL_TEXTURE_2D;
	switch (m_opts.m_textureType)
	{
	case TT_2D:
		target = GL_TEXTURE_2D;
		break;
	case TT_CUBIC:
		target = GL_TEXTURE_CUBE_MAP_EXT;
		break;
	default:
		commonSystem->Error("%s: bad texture type %d", GetName(), m_opts.m_textureType);
		return;
	}

	// ALPHA, LUMINANCE, LUMINANCE_ALPHA, and INTENSITY have been removed
	// in OpenGL 3.2. In order to mimic those modes, we use the swizzle operators
#if defined( USE_CORE_PROFILE )
	if (m_opts.m_colorFormat == CFM_GREEN_ALPHA)
	{
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, GL_ONE);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, GL_ONE);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, GL_ONE);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
	}
	else if (m_opts.m_format == FMT_LUM8)
	{
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, GL_RED);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, GL_ONE);
	}
	else if (m_opts.m_format == FMT_L8A8)
	{
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, GL_RED);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
	}
	else if (m_opts.m_format == FMT_ALPHA)
	{
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, GL_ONE);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, GL_ONE);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, GL_ONE);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, GL_RED);
	}
	else if (m_opts.m_format == FMT_INT8)
	{
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, GL_RED);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, GL_RED);
	}
	else {
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
	}
#else
	if (m_opts.colorFormat == CFM_GREEN_ALPHA) {
		qglTexParameteri(target, GL_TEXTURE_SWIZZLE_R, GL_ONE);
		qglTexParameteri(target, GL_TEXTURE_SWIZZLE_G, GL_ONE);
		qglTexParameteri(target, GL_TEXTURE_SWIZZLE_B, GL_ONE);
		qglTexParameteri(target, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
	}
	else if (m_opts.format == FMT_ALPHA) {
		qglTexParameteri(target, GL_TEXTURE_SWIZZLE_R, GL_ONE);
		qglTexParameteri(target, GL_TEXTURE_SWIZZLE_G, GL_ONE);
		qglTexParameteri(target, GL_TEXTURE_SWIZZLE_B, GL_ONE);
		qglTexParameteri(target, GL_TEXTURE_SWIZZLE_A, GL_RED);
	}
#endif

	switch (m_filter)
	{
	case TF_DEFAULT:
		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case TF_LINEAR:
		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case TF_NEAREST:
		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	default:
		commonSystem->Error("%s: bad texture filter %d", GetName(), m_filter);
	}

	//if (glConfig.anisotropicFilterAvailable) {
	//	// only do aniso filtering on mip mapped images
	//	if (filter == TF_DEFAULT) {
	//		int aniso = r_maxAnisotropicFiltering.GetInteger();
	//		if (aniso > glConfig.maxTextureAnisotropy) {
	//			aniso = glConfig.maxTextureAnisotropy;
	//		}
	//		if (aniso < 0) {
	//			aniso = 0;
	//		}
	//		qglTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
	//	}
	//	else {
	//		qglTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
	//	}
	//}
	//if (glConfig.textureLODBiasAvailable && (usage != TD_FONT)) {
	//	// use a blurring LOD bias in combination with high anisotropy to fix our aliasing grate textures...
	//	qglTexParameterf(target, GL_TEXTURE_LOD_BIAS_EXT, r_lodBias.GetFloat());
	//}

	//// set the wrap/clamp modes
	//switch (repeat) {
	//case TR_REPEAT:
	//	qglTexParameterf(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	qglTexParameterf(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	break;
	//case TR_CLAMP_TO_ZERO: {
	//	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//	qglTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, color);
	//	qglTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//	qglTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//}
	//					 break;
	//case TR_CLAMP_TO_ZERO_ALPHA: {
	//	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//	qglTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, color);
	//	qglTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//	qglTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//}
	//						   break;
	//case TR_CLAMP:
	//	qglTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	qglTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	break;
	//default:
	//	common->FatalError("%s: bad texture repeat %d", GetName(), repeat);
	//}
}

void qqImage::PurgeImage()
{
	if (m_texNum != TEXTURE_NOT_LOADED)
	{
		glDeleteTextures(1, (GLuint*)&m_texNum);	// this should be the ONLY place it is ever called!
		m_texNum = TEXTURE_NOT_LOADED;
	}
}

void qqImage::DeriveOpts()
{
	if (m_opts.m_format == FMT_NONE)
	{
		m_opts.m_colorFormat = CFM_DEFAULT;

		switch (m_usage)
		{
			case TD_COVERAGE:
				m_opts.m_format = FMT_DXT1;
				m_opts.m_colorFormat = CFM_GREEN_ALPHA;
				break;
			case TD_DEPTH:
				m_opts.m_format = FMT_DEPTH;
				break;
			case TD_DIFFUSE:
				// TD_DIFFUSE gets only set to when its a diffuse texture for an interaction
				m_opts.m_gammaMips = true;
				m_opts.m_format = FMT_DXT5;
				m_opts.m_colorFormat = CFM_YCOCG_DXT5;
				break;
			case TD_SPECULAR:
				m_opts.m_gammaMips = true;
				m_opts.m_format = FMT_DXT1;
				m_opts.m_colorFormat = CFM_DEFAULT;
				break;
			case TD_DEFAULT:
				m_opts.m_gammaMips = true;
				m_opts.m_format = FMT_DXT5;
				m_opts.m_colorFormat = CFM_DEFAULT;
				break;
			case TD_BUMP:
				m_opts.m_format = FMT_DXT5;
				m_opts.m_colorFormat = CFM_NORMAL_DXT5;
				break;
			case TD_FONT:
				m_opts.m_format = FMT_DXT1;
				m_opts.m_colorFormat = CFM_GREEN_ALPHA;
				m_opts.m_numLevels = 4; // We only support 4 levels because we align to 16 in the exporter
				m_opts.m_gammaMips = true;
				break;
			case TD_LIGHT:
				m_opts.m_format = FMT_RGB565;
				m_opts.m_gammaMips = true;
				break;
			case TD_LOOKUP_TABLE_MONO:
				m_opts.m_format = FMT_INT8;
				break;
			case TD_LOOKUP_TABLE_ALPHA:
				m_opts.m_format = FMT_ALPHA;
				break;
			case TD_LOOKUP_TABLE_RGB1:
			case TD_LOOKUP_TABLE_RGBA:
				m_opts.m_format = FMT_RGBA8;
				break;
			default:
				assert(false);
				m_opts.m_format = FMT_RGBA8;
		}
	}

	if (m_opts.m_numLevels == 0)
	{
		m_opts.m_numLevels = 1;

		if (m_filter == TF_LINEAR || m_filter == TF_NEAREST)
		{
			// don't create mip maps if we aren't going to be using them
		}
		else
		{
			int	temp_width = m_opts.m_width;
			int	temp_height = m_opts.m_height;
			while (temp_width > 1 || temp_height > 1)
			{
				temp_width >>= 1;
				temp_height >>= 1;
				if ((m_opts.m_format == FMT_DXT1 || m_opts.m_format == FMT_DXT5) &&
					((temp_width & 0x3) != 0 || (temp_height & 0x3) != 0))
				{
					break;
				}
				m_opts.m_numLevels++;
			}
		}
	}
}

void qqImage::AllocImage()
{
	GL_CheckErrors();
	PurgeImage();

	switch (m_opts.m_format)
	{
	case FMT_RGBA8:
		m_internalFormat = GL_RGBA8;
		m_dataFormat = GL_RGBA;
		m_dataType = GL_UNSIGNED_BYTE;
		break;
	case FMT_XRGB8:
		m_internalFormat = GL_RGB;
		m_dataFormat = GL_RGBA;
		m_dataType = GL_UNSIGNED_BYTE;
		break;
	case FMT_RGB565:
		m_internalFormat = GL_RGB;
		m_dataFormat = GL_RGB;
		m_dataType = GL_UNSIGNED_SHORT_5_6_5;
		break;
	case FMT_ALPHA:
#if defined( USE_CORE_PROFILE )
		m_internalFormat = GL_R8;
		m_dataFormat = GL_RED;
#else
		m_internalFormat = GL_ALPHA8;
		m_dataFormat = GL_ALPHA;
#endif
		m_dataType = GL_UNSIGNED_BYTE;
		break;
	case FMT_L8A8:
#if defined( USE_CORE_PROFILE )
		m_internalFormat = GL_RG8;
		m_dataFormat = GL_RG;
#else
		m_internalFormat = GL_LUMINANCE8_ALPHA8;
		m_dataFormat = GL_LUMINANCE_ALPHA;
#endif
		m_dataType = GL_UNSIGNED_BYTE;
		break;
	case FMT_LUM8:
#if defined( USE_CORE_PROFILE )
		m_internalFormat = GL_R8;
		m_dataFormat = GL_RED;
#else
		m_internalFormat = GL_LUMINANCE8;
		m_dataFormat = GL_LUMINANCE;
#endif
		m_dataType = GL_UNSIGNED_BYTE;
		break;
	case FMT_INT8:
#if defined( USE_CORE_PROFILE )
		m_internalFormat = GL_R8;
		m_dataFormat = GL_RED;
#else
		m_internalFormat = GL_INTENSITY8;
		m_dataFormat = GL_LUMINANCE;
#endif
		m_dataType = GL_UNSIGNED_BYTE;
		break;
	case FMT_DXT1:
		m_internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		m_dataFormat = GL_RGBA;
		m_dataType = GL_UNSIGNED_BYTE;
		break;
	case FMT_DXT5:
		m_internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		m_dataFormat = GL_RGBA;
		m_dataType = GL_UNSIGNED_BYTE;
		break;
	case FMT_DEPTH:
		m_internalFormat = GL_DEPTH_COMPONENT;
		m_dataFormat = GL_DEPTH_COMPONENT;
		m_dataType = GL_UNSIGNED_BYTE;
		break;
	case FMT_X16:
		m_internalFormat = GL_INTENSITY16;
		m_dataFormat = GL_LUMINANCE;
		m_dataType = GL_UNSIGNED_SHORT;
		break;
	case FMT_Y16_X16:
		m_internalFormat = GL_LUMINANCE16_ALPHA16;
		m_dataFormat = GL_LUMINANCE_ALPHA;
		m_dataType = GL_UNSIGNED_SHORT;
		break;
	default:
		commonSystem->Error("Unhandled image format %d in %s\n",
			m_opts.m_format, GetName());
	}

	// generate the texture number
	glGenTextures(1, (GLuint*)&m_texNum);
	assert(m_texNum != TEXTURE_NOT_LOADED);

	//----------------------------------------------------
	// allocate all the mip levels with NULL data
	//----------------------------------------------------

	int numSides;
	int target;
	int uploadTarget;
	if (m_opts.m_textureType == TT_2D)
	{
		target = uploadTarget = GL_TEXTURE_2D;
		numSides = 1;
	}
	else if (m_opts.m_textureType == TT_CUBIC)
	{
		target = GL_TEXTURE_CUBE_MAP_EXT;
		uploadTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT;
		numSides = 6;
	}
	else {
		assert(!"opts.textureType");
		target = uploadTarget = GL_TEXTURE_2D;
		numSides = 1;
	}

	glBindTexture(target, m_texNum);

	for (int side = 0; side < numSides; side++)
	{
		int w = m_opts.m_width;
		int h = m_opts.m_height;
		if (m_opts.m_textureType == TT_CUBIC)
		{
			h = w;
		}
		for (int level = 0; level < m_opts.m_numLevels; level++)
		{
			// clear out any previous error
			GL_CheckErrors();
			glTexImage2D(uploadTarget + side, level, m_internalFormat, w, h, 0, m_dataFormat, m_dataType, NULL);
			GL_CheckErrors();
			w = std::max<int>(1, w >> 1);
			h = std::max<int>(1, h >> 1);
		}
	}

	//qglTexParameteri(target, GL_TEXTURE_MAX_LEVEL, opts.numLevels - 1);

	//// see if we messed anything up
	//GL_CheckErrors();

	//SetTexParameters();

	//GL_CheckErrors();
}
