#pragma once
#include"KL2_API.h"
#include"Texture.hpp"
#include"RenderBuffer.hpp"
#include<vector>
#include"Tools/ErrorsSystem.hpp"

namespace KL2::Graphics::Primitives {

    class CFrameBuffer {
    protected:
        unsigned int ID = 0u;
        mutable bool Finished = false;

        static unsigned int _BufferDataFormat_SwitchCase(CRenderBuffer::EBufferDataFormat bufferDataFormat) noexcept;
        static unsigned int _DataFormatOnGPU_SwitchCase(CTexture::SDataSettings::EDataFormatOnGPU dataFormatOnGPU) noexcept;
    public:

        enum class EAttachmentTypes : uint8_t {
            Depth,Stencil,DepthStencil,Color
        };
    protected:
        static unsigned int _AttachmentType_SwitchCase(EAttachmentTypes attachmentType) noexcept;
    public:

        struct SErrorsEnumWrapper :KL2::ErrorsSystem::SErrorBase {
            enum EErrors :uint8_t {
                AlreadyFinished,
                NotFinished,
                NotComplete
            };
            EErrors Error;
            SErrorsEnumWrapper(EErrors error) :Error(error) {};
        }; using ErrorsEnum = SErrorsEnumWrapper; using AnyError = SErrorsEnumWrapper;
        

        KL2_API void ClearColorBuffer() const;
        KL2_API void ClearDepthBuffer() const;
        KL2_API void ClearStencilBuffer() const;
        //color,depth,stencil
        KL2_API void ClearAllBuffers() const;

        KL2_API CFrameBuffer();
        KL2_API CFrameBuffer(CFrameBuffer&& toCopy) noexcept;
        KL2_API virtual CFrameBuffer& operator=(CFrameBuffer&& toCopy);
        KL2_API virtual ~CFrameBuffer() noexcept(false);

        typedef unsigned int FrameBufferID_Type;
        FrameBufferID_Type gID() const noexcept { return ID; }
        operator FrameBufferID_Type() const noexcept { return ID; }

        KL2_API void Finish() const;
        KL2_API void Bind() const;
        //use colorAttachmentInd only if bufferDataFormat have somnething to do with colors, not depth/stencil
        KL2_API virtual void AttachRenderBuffer(CRenderBuffer& renderBuffer, CRenderBuffer::EBufferDataFormat bufferDataFormat, unsigned int colorAttachmentInd = 0);
        KL2_API virtual void UnattachRenderBuffer(CRenderBuffer::EBufferDataFormat bufferDataFormat, unsigned int colorAttachmentInd = 0);
        KL2_API virtual void UnattachRenderBuffer(EAttachmentTypes attachmentType, unsigned int colorAttachmentInd = 0);
        //use colorAttachmentInd only if dataFormatOnGPU have somnething to do with colors, not depth/stencil
        KL2_API virtual void AttachTexture(CTexture& texture, CTexture::SDataSettings::EDataFormatOnGPU dataFormatOnGPU, unsigned int colorAttachmentInd = 0);
        KL2_API virtual void UnattachTexture(CTexture::SDataSettings::EDataFormatOnGPU dataFormatOnGPU, unsigned int colorAttachmentInd = 0);
        KL2_API virtual void UnattachTexture(EAttachmentTypes attachmentType, unsigned int colorAttachmentInd = 0);
        KL2_API static void Unbind();
        //will update current viewport size
        KL2_API static void SetViewportSize(Vector2U viewportSize);

    };
}