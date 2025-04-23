#pragma once
#include"KL2_API.h"
#include"Tools/ErrorsSystem.hpp"
#include"Maths/Vector.hpp"

namespace KL2::Graphics::Primitives {
    class CRenderBuffer {
    protected:
        unsigned int ID = 0u;
    public:
        enum class EBufferDataFormat :uint8_t {
            Depth, DepthStencil, Stencil, Red, RG, RGB, RGBA, None
        };
    protected:
        static unsigned int _BufferDataFormat_SwitchCase(EBufferDataFormat bufferDataFormat) noexcept;
    public:

        KL2_API CRenderBuffer(Vector2U size, EBufferDataFormat bufferDataFormat);
        KL2_API CRenderBuffer(CRenderBuffer&& toCopy) noexcept;
        KL2_API virtual CRenderBuffer& operator=(CRenderBuffer&& toCopy);
        KL2_API virtual ~CRenderBuffer() noexcept(false);
        KL2_API void Bind() const;
        KL2_API static void Unbind();

        typedef unsigned int RenderBufferID_Type;
        RenderBufferID_Type gID() const noexcept { return ID; }
        operator RenderBufferID_Type() const noexcept { return ID; }

    };
}