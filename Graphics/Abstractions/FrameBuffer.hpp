#pragma once
#include"Graphics/Primitives/FrameBuffer.h"
#include"RenderBuffer.h"
#include"Texture.h"

namespace KE2::Graphics::Abstractions {
	class FrameBufferClass :protected Primitives::FrameBufferClass {
	protected:
		//dont really describe framebuffer itself, more like a helper variable
		mutable Vector2U ViewportSize = Vector2U(0u);
		
	public:
		using AttachmentTypesEnum = Primitives::FrameBufferClass::AttachmentTypesEnum;
		
	protected:
		class AttachmentSlotC :public BindingSlotC<AttachmentSlotC> {
			friend FrameBufferClass;
		protected:
			union { void* Ptr; RenderBufferClass* RB_Ptr; TextureClass* T_Ptr; } PtrToAttachmentInstance;
			enum class AttachmentInstanceTypeE: unsigned char { RenderBuffer, Texture } AttachmentInstanceType;

			AttachmentTypesEnum AttachmentType;
			unsigned int ColorAttachmentInd = 0u;

		public:
			AttachmentSlotC(std::vector<AttachmentSlotC>* vec, void* attachmentInstancePtr, AttachmentInstanceTypeE attachmentInstanceTyp, 
				AttachmentTypesEnum attachmentTyp, unsigned int colorAttachmentInd = 0u) noexcept : SlotT(vec), PtrToAttachmentInstance{ attachmentInstancePtr },
				AttachmentInstanceType(attachmentInstanceTyp), AttachmentType(attachmentTyp), ColorAttachmentInd(colorAttachmentInd) { 
				if (attachmentInstanceTyp == AttachmentInstanceTypeE::Texture)
					Connect(PtrToAttachmentInstance.T_Ptr->Bindings.emplace_back(&PtrToAttachmentInstance.T_Ptr->Bindings));
				else if (attachmentInstanceTyp == AttachmentInstanceTypeE::RenderBuffer)
					Connect(PtrToAttachmentInstance.RB_Ptr->Bindings.emplace_back(&PtrToAttachmentInstance.RB_Ptr->Bindings));
			};
			AttachmentSlotC(AttachmentSlotC&& toMove) noexcept: SlotT(std::move(toMove)), 
				PtrToAttachmentInstance(toMove.PtrToAttachmentInstance), AttachmentInstanceType(toMove.AttachmentInstanceType),
				AttachmentType(toMove.AttachmentType), ColorAttachmentInd(toMove.ColorAttachmentInd) { }
			AttachmentSlotC& operator=(AttachmentSlotC&& toMove) noexcept = default;
		};
		std::vector<AttachmentSlotC> Attachments;
	public:
		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
#ifdef KE2_Debug
				TryingToAttachRenderBufferWithDifferentBufferDataFormat,
				TryingToAttachTextureWithDifferentDataFormatOnGPU,
#endif
			}; ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct::ErrorsEnum; using AnyError = ErrorsEnumWrapperStruct;


		inline Primitives::FrameBufferClass& gPrimitiveFrameBufferClass() { return *this; }
		inline const Primitives::FrameBufferClass& gPrimitiveFrameBufferClass() const { return *this; }

		using Primitives::FrameBufferClass::ClearColorBuffer;
		using Primitives::FrameBufferClass::ClearDepthBuffer;
		using Primitives::FrameBufferClass::ClearStencilBuffer;
		using Primitives::FrameBufferClass::ClearAllBuffers;

		DLLTREATMENT FrameBufferClass();
		DLLTREATMENT FrameBufferClass(Vector2U viewportSize);
		DLLTREATMENT FrameBufferClass(const FrameBufferClass& toCopy) noexcept;
		FrameBufferClass(FrameBufferClass&& toCopy) noexcept = default;
		FrameBufferClass& operator=(const FrameBufferClass& toCopy) = default;
	private:
		virtual Primitives::FrameBufferClass& operator=(Primitives::FrameBufferClass&& toCopy) override final { return operator=(dynamic_cast<FrameBufferClass&&>(toCopy)); }
	public:
		FrameBufferClass& operator=(FrameBufferClass&& toCopy) = default;
		virtual ~FrameBufferClass() noexcept(false) override = default;

		using FrameBufferID_Type = Primitives::FrameBufferClass::FrameBufferID_Type;
		using Primitives::FrameBufferClass::gID;
		using Primitives::FrameBufferClass::operator FrameBufferID_Type;

		using Primitives::FrameBufferClass::Finish;
		DLLTREATMENT void Bind(bool updViewport = true) const;
	private:
		inline virtual void AttachRenderBuffer(Primitives::RenderBufferClass& renderBuffer, Primitives::RenderBufferClass::BufferDataFormatEnum bufferDataFormat, unsigned int colorAttachmentInd) override final {
			RenderBufferClass& rb = dynamic_cast<RenderBufferClass&>(renderBuffer);
#ifdef KE2_Debug
			if (rb.gBufferDataFormat() != bufferDataFormat) 
				ErrorsSystemNamespace::SendError << "Trying to attach render buffer with different bufferDataFormat" >> 
				ErrorsEnumWrapperStruct(ErrorsEnum::TryingToAttachRenderBufferWithDifferentBufferDataFormat);
#endif
			AttachRenderBuffer(rb, colorAttachmentInd);
		}
	public:
		//use colorAttachmentInd only if renderBuffer have somnething to do with colors, not depth/stencil
		DLLTREATMENT void AttachRenderBuffer(RenderBufferClass& renderBuffer, unsigned int colorAttachmentInd = 0);
		DLLTREATMENT virtual void UnattachRenderBuffer(RenderBufferClass& renderBuffer);
		DLLTREATMENT virtual void UnattachRenderBuffer(RenderBufferClass::BufferDataFormatEnum bufferDataFormat, unsigned int colorAttachmentInd = 0) override final;
		DLLTREATMENT virtual void UnattachRenderBuffer(AttachmentTypesEnum attachmentType, unsigned int colorAttachmentInd = 0) override final;
	private:
		inline virtual void AttachTexture(Primitives::TextureClass& texture, Primitives::TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, unsigned int colorAttachmentInd) override final {
			TextureClass& tr = dynamic_cast<TextureClass&>(texture);
#ifdef KE2_Debug
			if (tr.gDataSettings_DataFormatOnGPU() != dataFormatOnGPU) 
				ErrorsSystemNamespace::SendError << "Trying to attach texture with different dataFormatOnGPU" >> 
				ErrorsEnumWrapperStruct(ErrorsEnum::TryingToAttachTextureWithDifferentDataFormatOnGPU);
#endif
			AttachTexture(tr, colorAttachmentInd);
		}
	public:
		//use colorAttachmentInd only if dataFormatOnGPU have something to do with colors, not depth/stencil
		DLLTREATMENT void AttachTexture(TextureClass& texture, unsigned int colorAttachmentInd = 0);
		DLLTREATMENT virtual void UnattachTexture(TextureClass& texture);
		DLLTREATMENT virtual void UnattachTexture(TextureClass::DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, unsigned int colorAttachmentInd = 0) override final;
		DLLTREATMENT virtual void UnattachTexture(AttachmentTypesEnum attachmentType, unsigned int colorAttachmentInd = 0) override final;
		using Primitives::FrameBufferClass::Unbind;
		//will update opengl's viewport to viewport of current frame buffer
		DLLTREATMENT void UpdateViewportSize() const;
		//will update frame buffer's viewport size, optionaly can update opengl's viewport too
		DLLTREATMENT void SetViewportSize(Vector2U viewportSize, bool updCurViewportSize = false);
		inline Vector2U gViewportSize() const noexcept { return ViewportSize; }
		//will update current viewport size, not viewport of instance
		DLLTREATMENT static void SetViewportSize_Static(Vector2U viewportSize);
	};
}
