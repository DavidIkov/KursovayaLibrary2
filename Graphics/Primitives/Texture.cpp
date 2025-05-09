#include"Texture.hpp"
#ifndef KL2_DONT_USE_STB_IMAGE
#include"stb_image/stb_image.h"
#endif
#include"Tools/GLDebug.hpp"
#include"glad/glad.h"
#include"FrameBuffer.hpp"

using namespace KL2;
using namespace Graphics::Primitives;

unsigned int CTexture::_DataFormatOnGPU_SwitchCase(SDataSettings::EDataFormatOnGPU format) noexcept {
    switch (format) {
    case CTexture::SDataSettings::EDataFormatOnGPU::Depth: return GL_DEPTH_COMPONENT24;
    case CTexture::SDataSettings::EDataFormatOnGPU::DepthStencil: return GL_DEPTH24_STENCIL8;
    case CTexture::SDataSettings::EDataFormatOnGPU::Stencil: return GL_STENCIL_INDEX8;
    case CTexture::SDataSettings::EDataFormatOnGPU::R8UN: return GL_R8;
    case CTexture::SDataSettings::EDataFormatOnGPU::RG8UN: return GL_RG8;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGB8UN: return GL_RGB8;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGBA8UN: return GL_RGBA8;
    case CTexture::SDataSettings::EDataFormatOnGPU::R8SN: return GL_R8_SNORM;
    case CTexture::SDataSettings::EDataFormatOnGPU::RG8SN: return GL_RG8_SNORM;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGB8SN: return GL_RGB8_SNORM;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGBA8SN: return GL_RGBA8_SNORM;
    case CTexture::SDataSettings::EDataFormatOnGPU::R32F: return GL_R32F;
    case CTexture::SDataSettings::EDataFormatOnGPU::RG32F: return GL_RG32F;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGB32F: return GL_RGB32F;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGBA32F: return GL_RGBA32F;
    case CTexture::SDataSettings::EDataFormatOnGPU::R8S: return GL_R8I;
    case CTexture::SDataSettings::EDataFormatOnGPU::RG8S: return GL_RG8I;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGB8S: return GL_RGB8I;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGBA8S: return GL_RGBA8I;
    case CTexture::SDataSettings::EDataFormatOnGPU::R32S: return GL_R32I;
    case CTexture::SDataSettings::EDataFormatOnGPU::RG32S: return GL_RG32I;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGB32S: return GL_RGB32I;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGBA32S: return GL_RGBA32I;
    case CTexture::SDataSettings::EDataFormatOnGPU::R8U: return GL_R8UI;
    case CTexture::SDataSettings::EDataFormatOnGPU::RG8U: return GL_RG8UI;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGB8U: return GL_RGB8UI;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGBA8U: return GL_RGBA8UI;
    case CTexture::SDataSettings::EDataFormatOnGPU::R32U: return GL_R32UI;
    case CTexture::SDataSettings::EDataFormatOnGPU::RG32U: return GL_RG32UI;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGB32U: return GL_RGB32UI;
    case CTexture::SDataSettings::EDataFormatOnGPU::RGBA32U: return GL_RGBA32UI;
    default: return 0;
    }
}
unsigned int CTexture::_DataFormatOnCPU_SwitchCase(SDataSettings::EDataFormatOnCPU format) noexcept {
    switch (format) {
    case CTexture::SDataSettings::EDataFormatOnCPU::R: return GL_RED;
    case CTexture::SDataSettings::EDataFormatOnCPU::RG: return GL_RG;
    case CTexture::SDataSettings::EDataFormatOnCPU::RGB: return GL_RGB;
    case CTexture::SDataSettings::EDataFormatOnCPU::BGR: return GL_BGR;
    case CTexture::SDataSettings::EDataFormatOnCPU::RGBA: return GL_RGBA;
    case CTexture::SDataSettings::EDataFormatOnCPU::BGRA: return GL_BGRA;
    case CTexture::SDataSettings::EDataFormatOnCPU::R_NN: return GL_RED_INTEGER;
    case CTexture::SDataSettings::EDataFormatOnCPU::RG_NN: return GL_RG_INTEGER;
    case CTexture::SDataSettings::EDataFormatOnCPU::RGB_NN: return GL_RGB_INTEGER;
    case CTexture::SDataSettings::EDataFormatOnCPU::BGR_NN: return GL_BGR_INTEGER;
    case CTexture::SDataSettings::EDataFormatOnCPU::RGBA_NN: return GL_RGBA_INTEGER;
    case CTexture::SDataSettings::EDataFormatOnCPU::BGRA_NN: return GL_BGRA_INTEGER;
    case CTexture::SDataSettings::EDataFormatOnCPU::Stencil: return GL_STENCIL_INDEX;
    case CTexture::SDataSettings::EDataFormatOnCPU::Depth: return GL_DEPTH_COMPONENT;
    case CTexture::SDataSettings::EDataFormatOnCPU::DepthStencil: return GL_DEPTH_STENCIL;
    default: return 0;
    }
}
unsigned int CTexture::_DataTypeOnCPU_SwitchCase(SDataSettings::EDataTypeOnCPU type) noexcept {
    switch (type) {
    case CTexture::SDataSettings::EDataTypeOnCPU::UnsignedByte: return GL_UNSIGNED_BYTE;
    case CTexture::SDataSettings::EDataTypeOnCPU::Byte: return GL_BYTE;
    case CTexture::SDataSettings::EDataTypeOnCPU::UnsignedShort: return GL_UNSIGNED_SHORT;
    case CTexture::SDataSettings::EDataTypeOnCPU::Short: return GL_SHORT;
    case CTexture::SDataSettings::EDataTypeOnCPU::UnsignedInt: return GL_UNSIGNED_INT;
    case CTexture::SDataSettings::EDataTypeOnCPU::Int: return GL_INT;
    case CTexture::SDataSettings::EDataTypeOnCPU::Float: return GL_FLOAT;
    case CTexture::SDataSettings::EDataTypeOnCPU::UnsignedInt_24_8: return GL_UNSIGNED_INT_24_8;
    default: return 0;
    }
}
unsigned int CTexture::_DataTypeOnCPU_Sizeof_SwitchCase(SDataSettings::EDataTypeOnCPU type) noexcept {
    switch (type) {
    case CTexture::SDataSettings::EDataTypeOnCPU::UnsignedByte: return sizeof(unsigned char);
    case CTexture::SDataSettings::EDataTypeOnCPU::Byte: return sizeof(unsigned char);
    case CTexture::SDataSettings::EDataTypeOnCPU::UnsignedInt: return sizeof(unsigned int);
    case CTexture::SDataSettings::EDataTypeOnCPU::Int: return sizeof(int);
    case CTexture::SDataSettings::EDataTypeOnCPU::Float: return sizeof(float);
    default: return 0;
    }
}
unsigned int CTexture::_WrapType_SwitchCase(SSettings::EWrapType wrapTyp) noexcept {
    switch (wrapTyp) {
    case CTexture::SSettings::EWrapType::ClampToEdge: return GL_CLAMP_TO_EDGE;
    case CTexture::SSettings::EWrapType::MirroredRepeat: return GL_MIRRORED_REPEAT;
    case CTexture::SSettings::EWrapType::Repeat: return GL_REPEAT;
    case CTexture::SSettings::EWrapType::MirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
    default: return 0;
    }
}
unsigned int CTexture::_DownscalingFilterFunc_SwitchCase(SSettings::EDownscalingFilterFunc filt) noexcept {
    switch (filt) {
    case CTexture::SSettings::EDownscalingFilterFunc::Nearest: return GL_NEAREST;
    case CTexture::SSettings::EDownscalingFilterFunc::Linear: return GL_LINEAR;
    case CTexture::SSettings::EDownscalingFilterFunc::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
    case CTexture::SSettings::EDownscalingFilterFunc::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
    case CTexture::SSettings::EDownscalingFilterFunc::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
    case CTexture::SSettings::EDownscalingFilterFunc::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
    default: return 0;
    }
}
unsigned int CTexture::_UpscalingFilterFunc_SwitchCase(SSettings::EUpscalingFilterFunc filt) noexcept {
    switch (filt) {
    case CTexture::SSettings::EUpscalingFilterFunc::Nearest: return GL_NEAREST;
    case CTexture::SSettings::EUpscalingFilterFunc::Linear: return GL_LINEAR;
    default: return 0;
    }
}
unsigned int CTexture::_DepthStencilReadMode_SwitchCase(SSettings::EDepthStencilReadMode readMode) noexcept {
    switch (readMode) {
    case CTexture::SSettings::EDepthStencilReadMode::Depth: return GL_DEPTH_COMPONENT;
    case CTexture::SSettings::EDepthStencilReadMode::Stencil: return GL_STENCIL_INDEX;
    case CTexture::SSettings::EDepthStencilReadMode::None: return GL_DEPTH_COMPONENT;
    default: return 0;
    }
}
unsigned int CTexture::_SwizzleMask_SwitchCase(SSettings::ESwizzleMask swizzleMask) noexcept {
    switch (swizzleMask) {
    case SSettings::ESwizzleMask::Red: return GL_RED;
    case SSettings::ESwizzleMask::Blue: return GL_BLUE;
    case SSettings::ESwizzleMask::Green: return GL_GREEN;
    case SSettings::ESwizzleMask::Alpha: return GL_ALPHA;
    case SSettings::ESwizzleMask::Zero: return GL_ZERO;
    case SSettings::ESwizzleMask::One: return GL_ONE;
    }
}

unsigned int CTexture::E_GL_Texture(CTexture::EDimensions dim) noexcept {
    switch (dim) {
    case CTexture::EDimensions::One: return GL_TEXTURE_1D; break;
    case CTexture::EDimensions::Two: return GL_TEXTURE_2D; break;
    case CTexture::EDimensions::Three: return GL_TEXTURE_3D; break;
    }
    return 0;
}

#ifndef KL2_DONT_USE_STB_IMAGE
CTexture::CTexture(EDimensions dimensions, const char* filePath, Vector3U* writeSizePtr, CArrayView<uint8_t>* writeArrayView, unsigned int mipmapLevels, const SSettings& sets, const SDataSettings& dataSets) 
    :Dimensions(dimensions), EGL_Tex(E_GL_Texture(dimensions)) {

    int width, height, textureChannelsAmount;
    unsigned char* textureData = stbi_load(filePath, &width, &height, &textureChannelsAmount, 0);
    if (textureData == nullptr)
        ErrorsSystem::SendError << "stb_image error" >> SErrorsEnumWrapper(ErrorsEnum::STB_IMAGE_Failed);

    if (writeSizePtr != nullptr) *writeSizePtr = Vector3U(width, height, 0);
    if (writeArrayView != nullptr) *writeArrayView = CArrayView<uint8_t>(textureData, width * height);

    glSC(glGenTextures, 1, &ID);
    Bind();

    _AllocatePixels(Vector3U(width, height, 0u), mipmapLevels, dataSets.DataFormatOnGPU);
    SetSubData(Vector3U(0u), Vector3U(width, height, 0u), textureData, dataSets.DataFormatOnCPU, dataSets.DataTypeOnCPU);

    glSC(glGenerateMipmap, EGL_Tex);

    SetSettings(sets);
    
    if (writeArrayView == nullptr) stbi_image_free(textureData);
}
#endif
CTexture::CTexture(EDimensions dimensions, Vector3U pixelsAmount, const void* data, unsigned int mipmapLevels, const SSettings& sets, const SDataSettings& dataSets) 
    :Dimensions(dimensions), EGL_Tex(E_GL_Texture(dimensions)) {

    if (pixelsAmount == Vector3U(0u)) return;

    glSC(glGenTextures, 1, &ID);
    Bind();

    _AllocatePixels(pixelsAmount, mipmapLevels, dataSets.DataFormatOnGPU);
    SetSubData(Vector3U(0u), pixelsAmount, data, dataSets.DataFormatOnCPU, dataSets.DataTypeOnCPU);

    glSC(glGenerateMipmap, EGL_Tex);

    SetSettings(sets);

}
CTexture::CTexture(EDimensions dimensions, Vector3U pixelsAmount, unsigned int mipmapLevels, SDataSettings::EDataFormatOnGPU dataFormatOnGPU, const SSettings& sets) :
    Dimensions(dimensions), EGL_Tex(E_GL_Texture(dimensions)) {

    if (pixelsAmount == Vector3U(0u)) return;

    glSC(glGenTextures, 1, &ID);
    Bind();

    _AllocatePixels(pixelsAmount, mipmapLevels, dataFormatOnGPU);

    glSC(glGenerateMipmap, EGL_Tex);

    SetSettings(sets);
}
CTexture::CTexture(CTexture&& toCopy) noexcept :
    ID(toCopy.ID), Dimensions(toCopy.Dimensions), EGL_Tex(toCopy.EGL_Tex) {
    toCopy.ID = 0u;
}
CTexture& CTexture::operator=(CTexture&& toCopy){
    this->~CTexture();
    new(this) CTexture(std::move(toCopy));
    return *this;
}
CTexture::~CTexture() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteTextures, 1, &ID);
        ID = 0u;
    }
}

void CTexture::_AllocatePixels(Vector3U pixelsAmount, unsigned int mipmapLevels, SDataSettings::EDataFormatOnGPU dataFormatOnGPU) {
    Bind();
    unsigned int gl_dataFormatOnGPU = _DataFormatOnGPU_SwitchCase(dataFormatOnGPU);
    switch (Dimensions) {
    case EDimensions::One: glSC(glTexStorage1D, EGL_Tex, mipmapLevels + 1, gl_dataFormatOnGPU, pixelsAmount[0]); return;
    case EDimensions::Two: glSC(glTexStorage2D, EGL_Tex, mipmapLevels + 1, gl_dataFormatOnGPU, pixelsAmount[0], pixelsAmount[1]); return;
    case EDimensions::Three: glSC(glTexStorage3D, EGL_Tex, mipmapLevels + 1, gl_dataFormatOnGPU, pixelsAmount[0], pixelsAmount[1], pixelsAmount[2]); return;
    }
}

void CTexture::CopySubData(const CTexture& srcTex, Vector3U offsetInSource, Vector3U offsetInDestination, Vector3U pixelsAmount) {

    glSC(glCopyImageSubData, srcTex.ID, srcTex.EGL_Tex, 0, offsetInSource[0], offsetInSource[1], offsetInSource[2],
        ID, EGL_Tex, 0, offsetInDestination[0], offsetInDestination[1], offsetInDestination[2], pixelsAmount[0], pixelsAmount[1], pixelsAmount[2]);
    //TODO make it work with 3.3
    //opengl 3.3 variant
    /*
    CFrameBuffer fb;
    fb.AttachTexture(textureID, dataFormatOnGPU);
    fb.Finish();
    CopyFromFrameBuffer(fb.gID(), offsetInSource, offsetInDestination, pixelsAmount);
    // glCopyTexSubImage2D(GL_TEXTURE_2D, 0, offsetInDestination[0], offsetInDestination[1], offsetInSource[0], offsetInSource[1], pixelsAmount[0], pixelsAmount[1]);
    */
}

void CTexture::SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data,
    SDataSettings::EDataFormatOnCPU dataFormat, SDataSettings::EDataTypeOnCPU dataType) {
    Bind();

    unsigned int gl_dataFormatOnCPU = _DataFormatOnCPU_SwitchCase(dataFormat);
    unsigned int gl_dataTypeOnCPU = _DataTypeOnCPU_SwitchCase(dataType);
    switch (Dimensions) {
    case EDimensions::One: glSC(glTexSubImage1D, EGL_Tex, 0, pixelsOffset[0], pixelsAmount[0], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data); return;
    case EDimensions::Two: glSC(glTexSubImage2D, EGL_Tex, 0, pixelsOffset[0], pixelsOffset[1], pixelsAmount[0], pixelsAmount[1], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data); return;
    case EDimensions::Three: glSC(glTexSubImage3D, EGL_Tex, 0, pixelsOffset[0], pixelsOffset[1], pixelsOffset[2], pixelsAmount[0], pixelsAmount[1], pixelsAmount[2], gl_dataFormatOnCPU, gl_dataTypeOnCPU, data); return;
    }
}


void CTexture::GetData(void* buffer, SDataSettings::EDataFormatOnCPU dataFormat, SDataSettings::EDataTypeOnCPU dataType) const {
    Bind();

    glSC(glGetTexImage, EGL_Tex, 0, _DataFormatOnCPU_SwitchCase(dataFormat), _DataTypeOnCPU_SwitchCase(dataType), buffer);
}
void CTexture::GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount, SDataSettings::EDataFormatOnCPU dataFormat, 
    SDataSettings::EDataTypeOnCPU dataType) const {
    
    //TODO make this work with 3.3, now in order for this to work i have to use opengl 4.5
    glSC(glGetTextureSubImage, ID, 0, offset[0], offset[1], offset[2], pixelsAmount[0], pixelsAmount[1], pixelsAmount[2],
        _DataFormatOnCPU_SwitchCase(dataFormat), _DataTypeOnCPU_SwitchCase(dataType),
        _DataTypeOnCPU_Sizeof_SwitchCase(dataType) * pixelsAmount[0] * pixelsAmount[1] * pixelsAmount[2], buffer);
}

void CTexture::SetSettings_WrapTypeByX(SSettings::EWrapType wrapType) {
    Bind();
    glSC(glTexParameteri, EGL_Tex, GL_TEXTURE_WRAP_S, _WrapType_SwitchCase(wrapType));
}
void CTexture::SetSettings_WrapTypeByY(SSettings::EWrapType wrapType) {
    Bind();
    glSC(glTexParameteri, EGL_Tex, GL_TEXTURE_WRAP_T, _WrapType_SwitchCase(wrapType));
}
void CTexture::SetSettings_WrapTypeByZ(SSettings::EWrapType wrapType) {
    Bind();
    glSC(glTexParameteri, EGL_Tex, GL_TEXTURE_WRAP_R, _WrapType_SwitchCase(wrapType));
}
void CTexture::SetSettings_DownscalingFilt(SSettings::EDownscalingFilterFunc downscalingFilt) {
    Bind();
    glSC(glTexParameteri, EGL_Tex, GL_TEXTURE_MIN_FILTER, _DownscalingFilterFunc_SwitchCase(downscalingFilt));
}
void CTexture::SetSettings_UpscalingFilt(SSettings::EUpscalingFilterFunc upscalingFilt) {
    Bind(); 
    glSC(glTexParameteri, EGL_Tex, GL_TEXTURE_MAG_FILTER, _UpscalingFilterFunc_SwitchCase(upscalingFilt));
}
void CTexture::SetSettings_DepthStencilReadMode(SSettings::EDepthStencilReadMode depthStencilReadMode) {
    Bind();
    glSC(glTexParameteri, EGL_Tex, GL_DEPTH_STENCIL_TEXTURE_MODE, _DepthStencilReadMode_SwitchCase(depthStencilReadMode));
}
void CTexture::SetSettings_SwizzleMaskByR(SSettings::ESwizzleMask swizzleMask) {
    Bind();
    glSC(glTexParameteri, EGL_Tex, GL_TEXTURE_SWIZZLE_R, _SwizzleMask_SwitchCase(swizzleMask));
}
void CTexture::SetSettings_SwizzleMaskByG(SSettings::ESwizzleMask swizzleMask) {
    Bind();
    glSC(glTexParameteri, EGL_Tex, GL_TEXTURE_SWIZZLE_G, _SwizzleMask_SwitchCase(swizzleMask));
}
void CTexture::SetSettings_SwizzleMaskByB(SSettings::ESwizzleMask swizzleMask) {
    Bind();
    glSC(glTexParameteri, EGL_Tex, GL_TEXTURE_SWIZZLE_B, _SwizzleMask_SwitchCase(swizzleMask));
}
void CTexture::SetSettings_SwizzleMaskByA(SSettings::ESwizzleMask swizzleMask) {
    Bind();
    glSC(glTexParameteri, EGL_Tex, GL_TEXTURE_SWIZZLE_A, _SwizzleMask_SwitchCase(swizzleMask));
}

void CTexture::Bind(unsigned int textureInd) const {
    glSC(glActiveTexture, GL_TEXTURE0 + textureInd);
    glSC(glBindTexture, EGL_Tex, ID);
}
void CTexture::Bind() const {
    glSC(glBindTexture, EGL_Tex, ID);
}

void CTexture::Unbind() const {
    glSC(glBindTexture, EGL_Tex, 0);
}

