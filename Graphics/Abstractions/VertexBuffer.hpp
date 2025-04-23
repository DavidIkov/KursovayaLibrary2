#pragma once
#include"Graphics/Primitives/VertexBuffer.h"

namespace KE2::Graphics::Abstractions {
	class VertexBufferClass :protected Primitives::VertexBufferClass {
	public:

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				ProvidedTypeOfFileNotSupported,
#ifdef KE2_Debug
				TryingToUseDifferentBufferReadWriteMode,
#endif
			}; ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct::ErrorsEnum; using AnyError = ErrorsEnumWrapperStruct;

		using VertexBufferID_Type = Primitives::VertexBufferClass::VertexBufferID_Type;
		using BufferReadWriteModeEnum = Primitives::VertexBufferClass::BufferReadWriteModeEnum;
	protected:
		const BufferReadWriteModeEnum BufferReadWriteMode;
		size_t DataSizeInBytes = 0;

	public:
		inline Primitives::VertexBufferClass& gPrimitiveVertexBufferClass() noexcept { return *this; }
		inline const Primitives::VertexBufferClass& gPrimitiveVertexBufferClass() const noexcept { return *this; }

		DLLTREATMENT VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode);
		DLLTREATMENT VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const ArrayView<void>& data);
		//filePath to some 3d object, right now only .obj is accepted
		DLLTREATMENT VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const char* filePath);
		DLLTREATMENT VertexBufferClass(const VertexBufferClass& toCopy, bool copyBufferData = false);
		DLLTREATMENT VertexBufferClass(VertexBufferClass&& toCopy) noexcept;
		DLLTREATMENT virtual ~VertexBufferClass() noexcept(false) override = default;
		//wont copy data on GPU, only allocate a space on GPU
		DLLTREATMENT virtual VertexBufferClass& operator=(const VertexBufferClass& toCopy);
	private:
		inline virtual Primitives::VertexBufferClass& operator=(Primitives::VertexBufferClass&& toCopy) override final { return operator=(dynamic_cast<VertexBufferClass&&>(toCopy)); }
	public:
		DLLTREATMENT virtual VertexBufferClass& operator=(VertexBufferClass&& toCopy);

		using Primitives::VertexBufferClass::gID;
		using Primitives::VertexBufferClass::operator VertexBufferID_Type;

	private:
		inline virtual void SetData(const ArrayView<void>& data, const BufferReadWriteModeEnum bufferReadWriteMode) override final { 
#ifdef KE2_Debug
			if (bufferReadWriteMode != BufferReadWriteMode) 
				ErrorsSystemNamespace::SendError << "Trying to use different buffer read write mode" >> ErrorsEnumWrapperStruct(ErrorsEnum::TryingToUseDifferentBufferReadWriteMode);
#endif
			SetData(data); 
		}
	public:
		DLLTREATMENT void SetData(const ArrayView<void>& data);
		DLLTREATMENT void SetSubData(size_t offsetInBytes, const ArrayView<void>& data);

		DLLTREATMENT void CopyData(const VertexBufferClass& srcBuffer);

		DLLTREATMENT void CopySubData(const VertexBufferClass& srcBuffer, size_t srcOffsetInBytes, size_t dstOffsetInBytes, size_t amountOfBytesToCopy);

		//data should point to already allocated memory
		DLLTREATMENT void GetData(void* data) const;
		//data should point to already allocated memory
		DLLTREATMENT void GetSubData(size_t offsetInBytes, size_t amountOfBytesToCopy, void* data) const;

		inline BufferReadWriteModeEnum gBufferReadWriteModeEnum() const noexcept { return BufferReadWriteMode; }
		inline size_t gDataSizeInBytes() const noexcept { return DataSizeInBytes; }

		using Primitives::VertexBufferClass::Bind;
		using Primitives::VertexBufferClass::Unbind;

	};
}
