#pragma once
#include"KL2_API.h"
#include"Maths/Vector.hpp"
#include"Tools/ErrorsSystem.hpp"
#include"Tools/ArrayView.hpp"

namespace KL2::Graphics::Primitives {
    class CTexture {
    public:
        struct SSettings {
            enum class EWrapType :uint8_t {
                ClampToEdge, MirroredRepeat, Repeat, MirrorClampToEdge
            };
            enum class EDownscalingFilterFunc :uint8_t {
                Nearest, Linear, NearestMipmapNearest, NearestMipmapLinear, LinearMipmapLinear, LinearMipmapNearest
            };
            enum class EUpscalingFilterFunc :uint8_t {
                Nearest, Linear
            };
            enum class EDepthStencilReadMode :uint8_t {
                Depth, Stencil, 
                /*you cant technically select mode as None but in here its used to just dont mention depth mode texture when they have nothing to do with depth*/
                None
            };
            enum class ESwizzleMask :uint8_t {
                Red, Green, Blue, Alpha, Zero, One
            };

            Vector<3, EWrapType> WrapType = Vector<3,EWrapType>(EWrapType::Repeat);
            EDownscalingFilterFunc DownscalingFilt = EDownscalingFilterFunc::LinearMipmapLinear;
            EUpscalingFilterFunc UpscalingFilt = EUpscalingFilterFunc::Linear;
            EDepthStencilReadMode DepthStencilReadMode = EDepthStencilReadMode::None;
            Vector<4, ESwizzleMask> SwizzleMask = Vector<4, ESwizzleMask>{ESwizzleMask::Red, ESwizzleMask::Green, ESwizzleMask::Blue, ESwizzleMask::Alpha};

        };
        struct SDataSettings {
            //those are not all possible formats but i dont really see good use case for others, for full list https://www.khronos.org/opengl/wiki/GLAPI/glTexImage2D
            //N-normalized, means that when data will be accesed it will be normalized(aka divided by maximum value)
            //S-signed, U-unsigned, and number means amount of bits, if no N is in name then value is not normalized
            //note that opengl dont have to actually store in format that you provided becouse of hardware limitations, it will choose closest one,
            //but most likely you will never notice this
            enum class EDataFormatOnGPU :uint8_t {
                Depth, DepthStencil, Stencil,
                R8UN, RG8UN, RGB8UN, RGBA8UN,
                R8SN, RG8SN, RGB8SN, RGBA8SN,
                R32F, RG32F, RGB32F, RGBA32F,
                R8S, RG8S, RGB8S, RGBA8S,
                R32S, RG32S, RGB32S, RGBA32S,
                R8U, RG8U, RGB8U, RGBA8U,
                R32U, RG32U, RGB32U, RGBA32U,
            };
            //NN means not normalized, so you can pass a whole integer
            enum class EDataFormatOnCPU :uint8_t {
                R, RG, RGB, BGR, RGBA, BGRA, R_NN, RG_NN, RGB_NN, BGR_NN, RGBA_NN, BGRA_NN, Stencil,
                Depth, DepthStencil
            };
            enum class EDataTypeOnCPU :uint8_t {
                UnsignedByte, Byte, UnsignedShort, Short, UnsignedInt, Int, Float, UnsignedInt_24_8
            };

            EDataFormatOnGPU DataFormatOnGPU = EDataFormatOnGPU::RGB8UN;
            EDataFormatOnCPU DataFormatOnCPU = EDataFormatOnCPU::RGB;
            EDataTypeOnCPU DataTypeOnCPU = EDataTypeOnCPU::UnsignedByte;

        };

        enum class EDimensions :uint8_t {
            One, Two, Three
        };
    protected:

        static unsigned int _DataFormatOnGPU_SwitchCase(SDataSettings::EDataFormatOnGPU format) noexcept;
        static unsigned int _DataFormatOnCPU_SwitchCase(SDataSettings::EDataFormatOnCPU format) noexcept;
        static unsigned int _DataTypeOnCPU_SwitchCase(SDataSettings::EDataTypeOnCPU type) noexcept;
        static unsigned int _DataTypeOnCPU_Sizeof_SwitchCase(SDataSettings::EDataTypeOnCPU type) noexcept;
        static unsigned int _WrapType_SwitchCase(SSettings::EWrapType wrapTyp) noexcept;
        static unsigned int _DownscalingFilterFunc_SwitchCase(SSettings::EDownscalingFilterFunc filt) noexcept;
        static unsigned int _UpscalingFilterFunc_SwitchCase(SSettings::EUpscalingFilterFunc filt) noexcept;
        static unsigned int _DepthStencilReadMode_SwitchCase(SSettings::EDepthStencilReadMode readMode) noexcept;
        static unsigned int _SwizzleMask_SwitchCase(SSettings::ESwizzleMask swizzleMask) noexcept;

        static unsigned int E_GL_Texture(CTexture::EDimensions dim) noexcept;

        unsigned int ID = 0u;

        const EDimensions Dimensions;
        const unsigned int EGL_Tex;

        void _AllocatePixels(Vector3U pixelsAmount, unsigned int mipmapLevels, SDataSettings::EDataFormatOnGPU dataFormatOnGPU);
    public:

        struct SErrorsEnumWrapper :KL2::ErrorsSystem::SErrorBase {
            enum EErrors :uint8_t {
                STB_IMAGE_Failed,
            };
            EErrors Error;
            SErrorsEnumWrapper(EErrors error) :Error(error) {};
        }; using ErrorsEnum = SErrorsEnumWrapper; using AnyError = SErrorsEnumWrapper;

        KL2_API CTexture(EDimensions dimensions, const char* filePath, Vector3U* writeSizePtr, CArrayView<uint8_t>* writeArrayView, unsigned int mipmapLevels, const SSettings& sets, const SDataSettings& dataSets);
        KL2_API CTexture(EDimensions dimensions, Vector3U pixelsAmount, const void* data, unsigned int mipmapLevels, const SSettings& sets, const SDataSettings& dataSets);
        KL2_API CTexture(EDimensions dimensions, Vector3U pixelsAmount, unsigned int mipmapLevels, SDataSettings::EDataFormatOnGPU dataFormatOnGPU, const SSettings& sets);
        KL2_API CTexture(CTexture&& toCopy) noexcept;
        KL2_API virtual CTexture& operator=(CTexture&& toCopy);
        KL2_API virtual ~CTexture() noexcept(false);

        KL2_API virtual void SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data,
            SDataSettings::EDataFormatOnCPU dataFormat, SDataSettings::EDataTypeOnCPU dataType);

        //if you dont use some axes in pixelsAmounts then dont leave them 0, use 1
        //make sure that your texture have enough pixels for copying
        KL2_API void CopySubData(const CTexture& srcTex, Vector3U offsetInSource, Vector3U offsetInDestination, Vector3U pixelsAmount);

        //this function is slow since it will get data from gpu to cpu
        //buffer should not be nullptr, it should point to already allocated memory
        KL2_API virtual void GetData(void* buffer, SDataSettings::EDataFormatOnCPU dataFormat, SDataSettings::EDataTypeOnCPU dataType) const;
        //this function is slow since it will get data from gpu to cpu
        //buffer should not be nullptr, it should point to already allocated memory
        KL2_API virtual void GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount, 
            SDataSettings::EDataFormatOnCPU dataFormat, SDataSettings::EDataTypeOnCPU dataType) const;

        KL2_API virtual void SetSettings_WrapTypeByX(SSettings::EWrapType wrapType);
        KL2_API virtual void SetSettings_WrapTypeByY(SSettings::EWrapType wrapType);
        KL2_API virtual void SetSettings_WrapTypeByZ(SSettings::EWrapType wrapType);
        KL2_API virtual void SetSettings_DownscalingFilt(SSettings::EDownscalingFilterFunc downscalingFilt);
        KL2_API virtual void SetSettings_UpscalingFilt(SSettings::EUpscalingFilterFunc upscalingFilt);
        KL2_API virtual void SetSettings_DepthStencilReadMode(SSettings::EDepthStencilReadMode depthStencilReadMode);
        KL2_API virtual void SetSettings_SwizzleMaskByR(SSettings::ESwizzleMask swizzleMask);
        KL2_API virtual void SetSettings_SwizzleMaskByG(SSettings::ESwizzleMask swizzleMask);
        KL2_API virtual void SetSettings_SwizzleMaskByB(SSettings::ESwizzleMask swizzleMask);
        KL2_API virtual void SetSettings_SwizzleMaskByA(SSettings::ESwizzleMask swizzleMask);

        virtual void SetSettings(SSettings sets) {
            SetSettings_WrapTypeByX(sets.WrapType[0]); SetSettings_WrapTypeByY(sets.WrapType[1]); SetSettings_WrapTypeByZ(sets.WrapType[2]);
            SetSettings_DownscalingFilt(sets.DownscalingFilt); SetSettings_UpscalingFilt(sets.UpscalingFilt);
            SetSettings_DepthStencilReadMode(sets.DepthStencilReadMode);
            SetSettings_SwizzleMaskByR(sets.SwizzleMask[0]); SetSettings_SwizzleMaskByG(sets.SwizzleMask[1]);
            SetSettings_SwizzleMaskByB(sets.SwizzleMask[2]); SetSettings_SwizzleMaskByA(sets.SwizzleMask[3]);
        }

        typedef unsigned int TextureID_Type;
        TextureID_Type gID() const noexcept { return ID; }
        operator TextureID_Type() const noexcept { return ID; }

        KL2_API void Bind(unsigned int bindingInd) const;
        //will not set active texture, so it will replace texture in last binded slot(by opengl rules)
        KL2_API void Bind() const;
        KL2_API void Unbind() const;

        
    };
}
