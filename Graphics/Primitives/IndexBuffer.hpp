#pragma once
#include"KL2_API.h"
#include"Tools/ErrorsSystem.hpp"
#include"Tools/ArrayView.hpp"

namespace KL2::Graphics::Primitives {

    class CIndexBuffer{
    protected:
        unsigned int ID = 0u;
    public:
        enum class EBufferReadWriteMode :uint8_t {
            StreamDraw, StreamRead, StreamCopy,
            StaticDraw, StaticRead, StaticCopy,
            DynamicDraw, DynamicRead, DynamicCopy,
        };
        
        KL2_API CIndexBuffer();
        KL2_API CIndexBuffer(CIndexBuffer&& toCopy) noexcept;
        KL2_API virtual CIndexBuffer& operator=(CIndexBuffer&& toCopy);
        KL2_API virtual ~CIndexBuffer() noexcept(false);

        typedef unsigned int ID_t;
        ID_t gID() const noexcept { return ID; }
        operator ID_t() const noexcept { return ID; }

        KL2_API virtual void SetData(const CArrayView<uint8_t>& data, const EBufferReadWriteMode bufferReadWriteMode);
        KL2_API void SetSubData(size_t offsetInBytes, const CArrayView<uint8_t>& data);

        KL2_API void CopySubData(const CIndexBuffer& srcBuffer, size_t srcOffsetInBytes, size_t dstOffsetInBytes, size_t amountOfBytesToCopy);

        //data should point to already allocated memory
        KL2_API void GetSubData(size_t offsetInBytes, size_t amountOfBytesToCopy, void* data) const;

        KL2_API void Bind() const;
        KL2_API static void Unbind();

    };
}
