#include"IndexBuffer.hpp"
#include"Tools/GLDebug.hpp"
#include"glad/glad.h"

using namespace KL2;
using namespace Graphics::Primitives;

CIndexBuffer::CIndexBuffer() {
    glSC(glGenBuffers(1, &ID));
    Bind();
}
CIndexBuffer::CIndexBuffer(CIndexBuffer&& toCopy) noexcept :
    ID(toCopy.ID) {
    toCopy.ID = 0u;
}
CIndexBuffer& CIndexBuffer::operator=(CIndexBuffer&& toCopy) {
    this->~CIndexBuffer();
    new(this) CIndexBuffer(std::move(toCopy));
    return *this;
}
CIndexBuffer::~CIndexBuffer() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteBuffers(1, &ID));
        ID = 0u;
    }
};

static unsigned int _GetIBUsageForGL(CIndexBuffer::EBufferReadWriteMode usage) {
    switch (usage) {
    case CIndexBuffer::EBufferReadWriteMode::StreamDraw: return GL_STREAM_DRAW;
    case CIndexBuffer::EBufferReadWriteMode::StreamRead: return GL_STREAM_READ;
    case CIndexBuffer::EBufferReadWriteMode::StreamCopy: return GL_STREAM_COPY;
    case CIndexBuffer::EBufferReadWriteMode::StaticDraw: return GL_STATIC_DRAW;
    case CIndexBuffer::EBufferReadWriteMode::StaticRead: return GL_STATIC_READ;
    case CIndexBuffer::EBufferReadWriteMode::StaticCopy: return GL_STATIC_COPY;
    case CIndexBuffer::EBufferReadWriteMode::DynamicDraw: return GL_DYNAMIC_DRAW;
    case CIndexBuffer::EBufferReadWriteMode::DynamicRead: return GL_DYNAMIC_READ;
    case CIndexBuffer::EBufferReadWriteMode::DynamicCopy: return GL_DYNAMIC_COPY;
    }
    return 0;
}
void CIndexBuffer::SetData(const CArrayView<uint8_t>& data, const EBufferReadWriteMode bufferReadWriteMode) {
    Bind();
    glSC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.GetLen(), data.GetData(), _GetIBUsageForGL(bufferReadWriteMode)));
}
void CIndexBuffer::SetSubData(size_t offsetInBytes, const CArrayView<uint8_t>& data) {
    Bind();
    glSC(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offsetInBytes, data.GetLen(), data.GetData()));
}

void CIndexBuffer::CopySubData(const CIndexBuffer& srcBuffer, size_t srcOffsetInBytes, size_t dstOffsetInBytes, size_t amountOfBytesToCopy) {
    glSC(glBindBuffer(GL_COPY_READ_BUFFER, srcBuffer.ID));
    glSC(glBindBuffer(GL_COPY_WRITE_BUFFER, ID));

    glSC(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, srcOffsetInBytes, dstOffsetInBytes, amountOfBytesToCopy));

    glSC(glBindBuffer(GL_COPY_READ_BUFFER, 0));
    glSC(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
}

void CIndexBuffer::GetSubData(size_t offsetInBytes, size_t amountOfBytesToCopy, void* data) const {
    Bind();

    glSC(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offsetInBytes, amountOfBytesToCopy, data));
}

void CIndexBuffer::Bind() const {
    glSC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
}
void CIndexBuffer::Unbind() {
    glSC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}