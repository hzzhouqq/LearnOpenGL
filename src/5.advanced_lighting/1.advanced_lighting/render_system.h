#pragma once

#include <glad/glad.h>

#define USE_CORE_PROFILE

enum stereo3DMode_t
{
	STEREO3D_OFF,

	// half-resolution, non-square pixel views
	STEREO3D_SIDE_BY_SIDE_COMPRESSED,
	STEREO3D_TOP_AND_BOTTOM_COMPRESSED,

	// two full resolution views side by side, as for a dual cable display
	STEREO3D_SIDE_BY_SIDE,

	STEREO3D_INTERLACED,

	// OpenGL quad buffer
	STEREO3D_QUAD_BUFFER,

	// two full resolution views stacked with a 30 pixel guard band
	// On the PC this can be configured as a custom video timing, but
	// it definitely isn't a consumer level task.  The quad_buffer
	// support can handle 720P-3D with apropriate driver support.
	STEREO3D_HDMI_720
};

enum graphicsVendor_t
{
	VENDOR_NVIDIA,
	VENDOR_AMD,
	VENDOR_INTEL
};

//{@Doom3BFG Comment:
// Contains variables specific to the OpenGL configuration being run right now.
// These are constant once the OpenGL subsystem is initialized.
//@}
struct glconfig_t
{
	const char* renderer_string;
	const char* vendor_string;
	const char* version_string;
	const char* extensions_string;
	const char* wgl_extensions_string;
	const char* shading_language_string;

	float glVersion;				// atof( version_string )
	graphicsVendor_t vendor;

	int maxTextureSize;			// queried from GL
	int maxTextureCoords;
	int maxTextureImageUnits;
	int uniformBufferOffsetAlignment;
	float maxTextureAnisotropy;

	int colorBits;
	int depthBits;
	int stencilBits;

	bool multitextureAvailable;
	bool directStateAccess;
	bool textureCompressionAvailable;
	bool anisotropicFilterAvailable;
	bool textureLODBiasAvailable;
	bool seamlessCubeMapAvailable;
	bool sRGBFramebufferAvailable;
	bool vertexBufferObjectAvailable;
	bool mapBufferRangeAvailable;
	bool vertexArrayObjectAvailable;
	bool drawElementsBaseVertexAvailable;
	bool fragmentProgramAvailable;
	bool glslAvailable;
	bool uniformBufferAvailable;
	bool twoSidedStencilAvailable;
	bool depthBoundsTestAvailable;
	bool syncAvailable;
	bool timerQueryAvailable;
	bool occlusionQueryAvailable;
	bool debugOutputAvailable;
	bool swapControlTearAvailable;

	stereo3DMode_t stereo3Dmode;
	int nativeScreenWidth; // this is the native screen width resolution of the renderer
	int nativeScreenHeight; // this is the native screen height resolution of the renderer

	int displayFrequency;

	int isFullscreen;					// monitor number
	bool isStereoPixelFormat;
	bool stereoPixelFormatAvailable;
	int multisamples;

	// Screen separation for stereoscopic rendering is set based on this.
	// PC vid code sets this, converting from diagonals / inches / whatever as needed.
	// If the value can't be determined, set something reasonable, like 50cm.
	float physicalScreenWidthInCentimeters;
	float pixelAspect;
	GLuint global_vao;
};

// qqRenderSystem
class qqRenderSystem
{
public:
	virtual ~qqRenderSystem() {}

	virtual void InitOpenGL() = 0;
	virtual void ShutdownOpenGL() = 0;
};

void R_InitOpenGL();
bool R_IsInitialized();

void GL_CheckErrors();

extern glconfig_t	glConfig;
extern qqRenderSystem* renderSystem;
