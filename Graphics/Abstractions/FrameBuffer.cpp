#include"FrameBuffer.h"
using namespace KE2;
using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

FrameBufferClass::FrameBufferClass() { }
FrameBufferClass::FrameBufferClass(Vector2U viewportSize) :ViewportSize(viewportSize) {};
FrameBufferClass::FrameBufferClass(const FrameBufferClass& toCopy) noexcept :
	ViewportSize(toCopy.ViewportSize) {
	for (size_t i = 0; i < toCopy.Attachments.size(); i++) {
		if (toCopy.Attachments[i].AttachmentInstanceType == AttachmentSlotC::AttachmentInstanceTypeE::RenderBuffer)
			AttachRenderBuffer(*toCopy.Attachments[i].PtrToAttachmentInstance.RB_Ptr, toCopy.Attachments[i].ColorAttachmentInd);
		else if (toCopy.Attachments[i].AttachmentInstanceType==AttachmentSlotC::AttachmentInstanceTypeE::Texture)
			AttachTexture(*toCopy.Attachments[i].PtrToAttachmentInstance.T_Ptr, toCopy.Attachments[i].ColorAttachmentInd);
	}
	Finish();
}
void FrameBufferClass::Bind(bool updViewport) const {
	GP::FrameBufferClass::Bind();
	if (updViewport) GP::FrameBufferClass::SetViewportSize(ViewportSize);
}
void FrameBufferClass::AttachRenderBuffer(RenderBufferClass& renderBuffer, unsigned int colorAttachmentInd) {

	AttachmentTypesEnum attachmentTyp;
	switch (renderBuffer.BufferDataFormat) {
	case RenderBufferClass::BufferDataFormatEnum::Depth: attachmentTyp = AttachmentTypesEnum::Depth; break;
	case RenderBufferClass::BufferDataFormatEnum::Stencil: attachmentTyp = AttachmentTypesEnum::Stencil; break;
	case RenderBufferClass::BufferDataFormatEnum::DepthStencil: attachmentTyp = AttachmentTypesEnum::DepthStencil; break;
	default: attachmentTyp = AttachmentTypesEnum::Color; break;
	}

	for (size_t i = 0; i < Attachments.size(); i++)
		if (Attachments[i].AttachmentType == attachmentTyp) { 
			Attachments.erase(Attachments.begin() + i); 
			GP::FrameBufferClass::UnattachRenderBuffer(attachmentTyp, colorAttachmentInd);
			break; 
		}

	Attachments.emplace_back(&Attachments, &renderBuffer, AttachmentSlotC::AttachmentInstanceTypeE::RenderBuffer, attachmentTyp, colorAttachmentInd);
	GP::FrameBufferClass::AttachRenderBuffer(renderBuffer, renderBuffer.BufferDataFormat, colorAttachmentInd);
}
void FrameBufferClass::UnattachRenderBuffer(RenderBufferClass& renderBuffer) {
	for (size_t i = 0; i < Attachments.size(); i++)
		if (Attachments[i].PtrToAttachmentInstance.RB_Ptr == &renderBuffer) { 
			Attachments.erase(Attachments.begin() + i);
			GP::FrameBufferClass::UnattachRenderBuffer(Attachments[i].AttachmentType, Attachments[i].ColorAttachmentInd);
			break; 
		}
}
void FrameBufferClass::UnattachRenderBuffer(RenderBufferClass::BufferDataFormatEnum bufferDataFormat, unsigned int colorAttachmentInd) {
	for (size_t i = 0; i < Attachments.size(); i++) {
		AttachmentSlotC& att = Attachments[i];
		if (att.AttachmentInstanceType == AttachmentSlotC::AttachmentInstanceTypeE::RenderBuffer) {
			if (att.AttachmentType == AttachmentTypesEnum::Depth && bufferDataFormat == RenderBufferClass::BufferDataFormatEnum::Depth ||
				att.AttachmentType == AttachmentTypesEnum::Stencil && bufferDataFormat == RenderBufferClass::BufferDataFormatEnum::Stencil ||
				att.AttachmentType == AttachmentTypesEnum::DepthStencil && bufferDataFormat == RenderBufferClass::BufferDataFormatEnum::DepthStencil ||
				att.AttachmentType == AttachmentTypesEnum::Color && colorAttachmentInd == att.ColorAttachmentInd &&
				bufferDataFormat != RenderBufferClass::BufferDataFormatEnum::Depth &&
				bufferDataFormat != RenderBufferClass::BufferDataFormatEnum::Stencil &&
				bufferDataFormat != RenderBufferClass::BufferDataFormatEnum::DepthStencil) {
				Attachments.erase(Attachments.begin() + i);
				GP::FrameBufferClass::UnattachRenderBuffer(Attachments[i].AttachmentType, Attachments[i].ColorAttachmentInd);
				break;
			}
		}
	}
}
void FrameBufferClass::UnattachRenderBuffer(AttachmentTypesEnum attachmentType, unsigned int colorAttachmentInd) {
	for (size_t i = 0; i < Attachments.size(); i++)
		if (Attachments[i].AttachmentType == attachmentType) {
			Attachments.erase(Attachments.begin() + i);
			GP::FrameBufferClass::UnattachRenderBuffer(Attachments[i].AttachmentType, Attachments[i].ColorAttachmentInd);
			break;
		}
}
void FrameBufferClass::AttachTexture(TextureClass& texture, unsigned int colorAttachmentInd) {

	AttachmentTypesEnum attachmentTyp;
	switch (texture.DataSettings.DataFormatOnGPU) {
	case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Depth: attachmentTyp = AttachmentTypesEnum::Depth; break;
	case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Stencil: attachmentTyp = AttachmentTypesEnum::Stencil; break;
	case TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::DepthStencil: attachmentTyp = AttachmentTypesEnum::DepthStencil; break;
	default: attachmentTyp = AttachmentTypesEnum::Color; break;
	}

	for (size_t i = 0; i < Attachments.size(); i++)
		if (Attachments[i].AttachmentType == attachmentTyp) { 
			Attachments.erase(Attachments.begin() + i);
			GP::FrameBufferClass::UnattachTexture(attachmentTyp, colorAttachmentInd);
			break; 
		}

	Attachments.emplace_back(&Attachments, &texture, AttachmentSlotC::AttachmentInstanceTypeE::Texture, attachmentTyp, colorAttachmentInd);
	GP::FrameBufferClass::AttachTexture(texture, texture.DataSettings.DataFormatOnGPU, colorAttachmentInd);
}
void FrameBufferClass::UnattachTexture(TextureClass& texture) {
	for (size_t i = 0; i < Attachments.size(); i++)
		if (Attachments[i].PtrToAttachmentInstance.T_Ptr == &texture) { 
			Attachments.erase(Attachments.begin() + i);
			GP::FrameBufferClass::UnattachTexture(Attachments[i].AttachmentType, Attachments[i].ColorAttachmentInd);
			break; 
		}
}
void FrameBufferClass::UnattachTexture(TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, unsigned int colorAttachmentInd) {
	for (size_t i = 0; i < Attachments.size(); i++) {
		AttachmentSlotC& att = Attachments[i];
		if (att.AttachmentInstanceType == AttachmentSlotC::AttachmentInstanceTypeE::Texture) {
			if (att.AttachmentType == AttachmentTypesEnum::Depth && dataFormatOnGPU == TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Depth ||
				att.AttachmentType == AttachmentTypesEnum::Stencil && dataFormatOnGPU == TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Stencil ||
				att.AttachmentType == AttachmentTypesEnum::DepthStencil && dataFormatOnGPU == TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::DepthStencil ||
				att.AttachmentType == AttachmentTypesEnum::Color && colorAttachmentInd == att.ColorAttachmentInd &&
				dataFormatOnGPU != TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Depth &&
				dataFormatOnGPU != TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::Stencil &&
				dataFormatOnGPU != TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum::DepthStencil) {
				Attachments.erase(Attachments.begin() + i);
				GP::FrameBufferClass::UnattachTexture(att.AttachmentType, att.ColorAttachmentInd);
				break;
			}
		}
	}
}
void FrameBufferClass::UnattachTexture(AttachmentTypesEnum attachmentType, unsigned int colorAttachmentInd) {
	for (size_t i = 0; i < Attachments.size(); i++)
		if (Attachments[i].AttachmentType == attachmentType) {
			Attachments.erase(Attachments.begin() + i);
			GP::FrameBufferClass::UnattachTexture(Attachments[i].AttachmentType, Attachments[i].ColorAttachmentInd);
			break;
		}
}
void FrameBufferClass::UpdateViewportSize() const {
	GP::FrameBufferClass::SetViewportSize(ViewportSize);
}
void FrameBufferClass::SetViewportSize(Vector2U viewportSize, bool updCurViewportSize) {
	ViewportSize = viewportSize;
	if (updCurViewportSize) UpdateViewportSize();
}
void FrameBufferClass::SetViewportSize_Static(Vector2U viewportSize) {
	GP::FrameBufferClass::SetViewportSize(viewportSize);
}

