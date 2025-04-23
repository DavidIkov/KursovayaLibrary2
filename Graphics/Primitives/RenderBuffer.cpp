#include"RenderBuffer.hpp"
#include"glad/glad.h"
#include"Tools/GLDebug.hpp"

using namespace KL2;
using namespace Graphics::Primitives;


unsigned int CRenderBuffer::_BufferDataFormat_SwitchCase(EBufferDataFormat bufferDataFormat) noexcept {
    switch (bufferDataFormat) {
    case EBufferDataFormat::Depth: return GL_DEPTH_COMPONENT24;
    case EBufferDataFormat::DepthStencil: return GL_DEPTH24_STENCIL8;
    case EBufferDataFormat::Stencil: return GL_STENCIL_INDEX8;
    case EBufferDataFormat::Red: return GL_R8;
    case EBufferDataFormat::RG: return GL_RG8;
    case EBufferDataFormat::RGB: return GL_RGB8;
    case EBufferDataFormat::RGBA: return GL_RGBA8;
    default: return 0;
    }
}
CRenderBuffer::CRenderBuffer(Vector2U size, EBufferDataFormat bufferDataFormat) {
    glSC(glGenRenderbuffers(1, &ID));
    Bind();
    glSC(glRenderbufferStorage(GL_RENDERBUFFER, _BufferDataFormat_SwitchCase(bufferDataFormat), size[0], size[1]));
}
CRenderBuffer::CRenderBuffer(CRenderBuffer&& toCopy) noexcept :
    ID(toCopy.ID) {
    toCopy.ID = 0u;
}
CRenderBuffer& CRenderBuffer::operator=(CRenderBuffer&& toCopy) {
    this->~CRenderBuffer();
    new(this) CRenderBuffer(std::move(toCopy));
    return *this;
}
CRenderBuffer::~CRenderBuffer() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteRenderbuffers(1, &ID));
        ID = 0u;
    }
}
void CRenderBuffer::Bind() const {
    glSC(glBindRenderbuffer(GL_RENDERBUFFER, ID));
}
void CRenderBuffer::Unbind() {
    glSC(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}