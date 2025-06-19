#include"VertexBuffer.hpp"
#include"Tools/GLDebug.hpp"
#include"glad/glad.h"

using namespace KL2;
using namespace Graphics::Primitives;

CVertexBuffer::CVertexBuffer() {
    glSC(glGenBuffers, 1, &ID);
    Bind();
}
CVertexBuffer::CVertexBuffer(CVertexBuffer&& toCopy) noexcept :
    ID(toCopy.ID) {
    toCopy.ID = 0u;
}
CVertexBuffer& CVertexBuffer::operator=(CVertexBuffer&& toCopy) {
    this->~CVertexBuffer();
    new(this) CVertexBuffer(std::move(toCopy));
    return *this;
}
CVertexBuffer::~CVertexBuffer() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteBuffers, 1, &ID);
        ID = 0u;
    }
};

static unsigned int _GetVBUsageForGL(CVertexBuffer::EBufferReadWriteMode usage) {
    switch (usage) {
    case CVertexBuffer::EBufferReadWriteMode::StreamDraw: return GL_STREAM_DRAW;
    case CVertexBuffer::EBufferReadWriteMode::StreamRead: return GL_STREAM_READ;
    case CVertexBuffer::EBufferReadWriteMode::StreamCopy: return GL_STREAM_COPY;
    case CVertexBuffer::EBufferReadWriteMode::StaticDraw: return GL_STATIC_DRAW;
    case CVertexBuffer::EBufferReadWriteMode::StaticRead: return GL_STATIC_READ;
    case CVertexBuffer::EBufferReadWriteMode::StaticCopy: return GL_STATIC_COPY;
    case CVertexBuffer::EBufferReadWriteMode::DynamicDraw: return GL_DYNAMIC_DRAW;
    case CVertexBuffer::EBufferReadWriteMode::DynamicRead: return GL_DYNAMIC_READ;
    case CVertexBuffer::EBufferReadWriteMode::DynamicCopy: return GL_DYNAMIC_COPY;
    }
    return 0;
}
void CVertexBuffer::ReserveData(size_t len, const EBufferReadWriteMode bufferReadWriteMode) {
    Bind();
    glSC(glBufferData, GL_ARRAY_BUFFER, len, nullptr, _GetVBUsageForGL(bufferReadWriteMode));
}
void CVertexBuffer::SetData(const CArrayView<uint8_t>& data, const EBufferReadWriteMode bufferReadWriteMode) {
    Bind();
    glSC(glBufferData, GL_ARRAY_BUFFER, data.GetLen(), data.GetData(), _GetVBUsageForGL(bufferReadWriteMode));
}
void CVertexBuffer::SetSubData(size_t offsetInBytes, const CArrayView<uint8_t>& data) {
    Bind();
    glSC(glBufferSubData, GL_ARRAY_BUFFER, offsetInBytes, data.GetLen(), data.GetData());
}

void CVertexBuffer::CopySubData(const CVertexBuffer& srcBuffer, size_t srcOffsetInBytes, size_t dstOffsetInBytes, size_t amountOfBytesToCopy) {
    glSC(glBindBuffer, GL_COPY_READ_BUFFER, srcBuffer.ID);
    glSC(glBindBuffer, GL_COPY_WRITE_BUFFER, ID);

    glSC(glCopyBufferSubData, GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, srcOffsetInBytes, dstOffsetInBytes, amountOfBytesToCopy);

    glSC(glBindBuffer, GL_COPY_READ_BUFFER, 0);
    glSC(glBindBuffer, GL_COPY_WRITE_BUFFER, 0);
}

void CVertexBuffer::GetSubData(size_t offsetInBytes, size_t amountOfBytesToCopy, void* data) const {
    Bind();
    glSC(glGetBufferSubData, GL_ARRAY_BUFFER, offsetInBytes, amountOfBytesToCopy, data);
}

void CVertexBuffer::Bind() const {
    glSC(glBindBuffer, GL_ARRAY_BUFFER, ID);
}
void CVertexBuffer::Unbind() {
    glSC(glBindBuffer, GL_ARRAY_BUFFER, 0);
}