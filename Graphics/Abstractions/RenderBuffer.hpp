#pragma once
#include"Graphics/Primitives/RenderBuffer.hpp"
#include"Tools/Connections/BindingSlot.h"
#include<vector>

namespace KE2::Graphics::Abstractions {
    class RenderBufferClass :protected Primitives::RenderBufferClass {
    public:
        using BufferDataFormatEnum = Primitives::RenderBufferClass::BufferDataFormatEnum;
    protected:
        const BufferDataFormatEnum BufferDataFormat;
        const Vector2U Size;

        friend class FrameBufferClass;
        std::vector<BindingSlotC_Default> Bindings;
    public:


        inline Primitives::RenderBufferClass& gPrimitiveRenderBufferClass() noexcept { return *this; }
        inline const Primitives::RenderBufferClass& gPrimitiveRenderBufferClass() const noexcept { return *this; }

        DLLTREATMENT RenderBufferClass(Vector2U size, BufferDataFormatEnum bufferDataFormat);
        DLLTREATMENT RenderBufferClass(RenderBufferClass&& toCopy) noexcept;
        DLLTREATMENT RenderBufferClass(const RenderBufferClass& toCopy) noexcept;
    private:
        inline virtual Primitives::RenderBufferClass& operator=(Primitives::RenderBufferClass&& toCopy) override final { return operator=(dynamic_cast<RenderBufferClass&&>(toCopy)); }
    public:
        DLLTREATMENT virtual RenderBufferClass& operator=(RenderBufferClass&& toCopy);
        DLLTREATMENT virtual RenderBufferClass& operator=(const RenderBufferClass& toCopy);
        DLLTREATMENT virtual ~RenderBufferClass() noexcept(false) override = default;

        inline BufferDataFormatEnum gBufferDataFormat() const noexcept { return BufferDataFormat; }
        inline Vector2U gSize() const noexcept { return Size; }

        using Primitives::RenderBufferClass::Bind;
        using Primitives::RenderBufferClass::Unbind;

        using RenderBufferID_Type = Primitives::RenderBufferClass::RenderBufferID_Type;
        using Primitives::RenderBufferClass::gID;
        using Primitives::RenderBufferClass::operator RenderBufferID_Type;

    };
}
