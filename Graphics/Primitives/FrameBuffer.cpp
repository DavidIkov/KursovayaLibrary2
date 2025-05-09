#include"FrameBuffer.hpp"
#include"glad/glad.h"
#include"Tools/GLDebug.hpp"

using namespace KL2;
using namespace Graphics::Primitives;

#define Assert_Finished_Macro if (not Finished) ErrorsSystem::SendError<<"FrameBuffer is not finished">>\
SErrorsEnumWrapper(ErrorsEnum::NotFinished);
#define Assert_NotFinished_Macro if (Finished) ErrorsSystem::SendError<<"FrameBuffer is already finished">>\
SErrorsEnumWrapper(ErrorsEnum::AlreadyFinished);

unsigned int CFrameBuffer::_BufferDataFormat_SwitchCase(CRenderBuffer::EBufferDataFormat bufferDataFormat) noexcept {
    switch (bufferDataFormat) {
    case CRenderBuffer::EBufferDataFormat::Depth: return GL_DEPTH_ATTACHMENT;
    case CRenderBuffer::EBufferDataFormat::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
    case CRenderBuffer::EBufferDataFormat::Stencil: return GL_STENCIL_ATTACHMENT;
    case CRenderBuffer::EBufferDataFormat::Red: return GL_COLOR_ATTACHMENT0;
    case CRenderBuffer::EBufferDataFormat::RG: return GL_COLOR_ATTACHMENT0;
    case CRenderBuffer::EBufferDataFormat::RGB: return GL_COLOR_ATTACHMENT0;
    case CRenderBuffer::EBufferDataFormat::RGBA: return GL_COLOR_ATTACHMENT0;
    default: return 0;
    }
}
unsigned int CFrameBuffer::_DataFormatOnGPU_SwitchCase(CTexture::SDataSettings::EDataFormatOnGPU dataFormatOnGPU) noexcept {
    switch (dataFormatOnGPU) {
    case CTexture::SDataSettings::EDataFormatOnGPU::Depth: return GL_DEPTH_ATTACHMENT;
    case CTexture::SDataSettings::EDataFormatOnGPU::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
    case CTexture::SDataSettings::EDataFormatOnGPU::Stencil: return GL_STENCIL_ATTACHMENT;
    default: return GL_COLOR_ATTACHMENT0;
    }
}
unsigned int CFrameBuffer::_AttachmentType_SwitchCase(EAttachmentTypes attachmentType) noexcept {
    switch (attachmentType) {
    case EAttachmentTypes::Depth: return GL_DEPTH_ATTACHMENT;
    case EAttachmentTypes::Stencil: return GL_STENCIL_ATTACHMENT;
    case EAttachmentTypes::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
    case EAttachmentTypes::Color: return GL_COLOR_ATTACHMENT0;
    }
    return 0;
}
void CFrameBuffer::ClearColorBuffer() const {
    Assert_Finished_Macro;
    Bind();
    glSC(glClear, GL_COLOR_BUFFER_BIT);
}
void CFrameBuffer::ClearDepthBuffer() const {
    Assert_Finished_Macro;
    Bind();
    glSC(glClear, GL_DEPTH_BUFFER_BIT);
}
void CFrameBuffer::ClearStencilBuffer() const {
    Assert_Finished_Macro;
    Bind();
    glSC(glClear, GL_STENCIL_BUFFER_BIT);
}
void CFrameBuffer::ClearAllBuffers() const {
    ClearColorBuffer();
    ClearDepthBuffer();
    ClearStencilBuffer();
}

CFrameBuffer::CFrameBuffer() {
    glSC(glGenFramebuffers, 1, &ID);
    Bind();
}
CFrameBuffer::CFrameBuffer(CFrameBuffer&& toCopy) noexcept:
    ID(toCopy.ID) ,Finished(toCopy.Finished)
{
    toCopy.ID = 0u;
}
CFrameBuffer& CFrameBuffer::operator=(CFrameBuffer&& toCopy) {
    this->~CFrameBuffer();
    new(this) CFrameBuffer(std::move(toCopy));
    return *this;
}
CFrameBuffer::~CFrameBuffer() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteFramebuffers, 1, &ID);
        ID = 0u;
    }
}
void CFrameBuffer::AttachRenderBuffer(CRenderBuffer& renderBuffer, CRenderBuffer::EBufferDataFormat bufferDataFormat, unsigned int colorAttachmentInd) {
    Bind();
    Assert_NotFinished_Macro;

    glSC(glFramebufferRenderbuffer, GL_FRAMEBUFFER, _BufferDataFormat_SwitchCase(bufferDataFormat) + colorAttachmentInd, GL_RENDERBUFFER, renderBuffer.gID());
}
void CFrameBuffer::UnattachRenderBuffer(CRenderBuffer::EBufferDataFormat bufferDataFormat, unsigned int colorAttachmentInd) {
    Bind();
    Assert_NotFinished_Macro;

    glSC(glFramebufferRenderbuffer, GL_FRAMEBUFFER, _BufferDataFormat_SwitchCase(bufferDataFormat) + colorAttachmentInd, GL_RENDERBUFFER, 0);
}
void CFrameBuffer::UnattachRenderBuffer(EAttachmentTypes attachmentType, unsigned int colorAttachmentInd) {
    Bind(); Assert_NotFinished_Macro;

    glSC(glFramebufferRenderbuffer, GL_FRAMEBUFFER, _AttachmentType_SwitchCase(attachmentType) + colorAttachmentInd, GL_RENDERBUFFER, 0);
}
void CFrameBuffer::AttachTexture(CTexture& texture, CTexture::SDataSettings::EDataFormatOnGPU dataFormatOnGPU, unsigned int colorAttachmentInd) {
    Bind();
    Assert_NotFinished_Macro;

    glSC(glFramebufferTexture2D, GL_FRAMEBUFFER, _DataFormatOnGPU_SwitchCase(dataFormatOnGPU) + colorAttachmentInd, GL_TEXTURE_2D, texture.gID(), 0);
}
void CFrameBuffer::UnattachTexture(CTexture::SDataSettings::EDataFormatOnGPU dataFormatOnGPU, unsigned int colorAttachmentInd) {
    Bind();
    Assert_NotFinished_Macro;

    glSC(glFramebufferTexture2D, GL_FRAMEBUFFER, _DataFormatOnGPU_SwitchCase(dataFormatOnGPU) + colorAttachmentInd, GL_TEXTURE_2D, 0, 0);
}
void CFrameBuffer::UnattachTexture(EAttachmentTypes attachmentType, unsigned int colorAttachmentInd) {
    Bind(); Assert_NotFinished_Macro;

    glSC(glFramebufferTexture2D, GL_FRAMEBUFFER, _AttachmentType_SwitchCase(attachmentType) + colorAttachmentInd, GL_TEXTURE_2D, 0, 0);
}


void CFrameBuffer::Finish() const {
    Bind();
    Assert_NotFinished_Macro;

    Finished = true;
    unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        ErrorsSystem::SendError << "FrameBuffer is not complete, OpenGL's error: [" << std::to_string(status) << "]" >> SErrorsEnumWrapper(ErrorsEnum::NotComplete);
}
void CFrameBuffer::Bind() const {

    glSC(glBindFramebuffer, GL_FRAMEBUFFER, ID);
}
void CFrameBuffer::Unbind() {
    glSC(glBindFramebuffer, GL_FRAMEBUFFER, 0);
}
void CFrameBuffer::SetViewportSize(Vector2U viewportSize) {
    glSC(glViewport, 0, 0, viewportSize[0], viewportSize[1]);
}
