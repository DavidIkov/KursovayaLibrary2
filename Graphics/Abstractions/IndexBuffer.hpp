#pragma once
#include"Graphics/Primitives/IndexBuffer.h"

namespace KE2::Graphics::Abstractions {
	class IndexBufferClass :protected Primitives::IndexBufferClass {
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

		using IndexBufferID_Type = Primitives::IndexBufferClass::IndexBufferID_Type;
		using BufferReadWriteModeEnum = Primitives::IndexBufferClass::BufferReadWriteModeEnum;
	protected:
		const BufferReadWriteModeEnum BufferReadWriteMode;
		size_t DataSizeInBytes = 0u;
	public:

		inline Primitives::IndexBufferClass& gPrimitiveIndexBufferClass() noexcept { return *this; }
		inline const Primitives::IndexBufferClass& gPrimitiveIndexBufferClass() const noexcept { return *this; }

		DLLTREATMENT IndexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode);
		DLLTREATMENT IndexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const ArrayView<void>& data);
		//filePath to some 3d object, right now only .obj is accepted
		DLLTREATMENT IndexBufferClass(const IndexBufferClass& toCopy, bool copyBufferData = false);
		DLLTREATMENT IndexBufferClass(IndexBufferClass&& toCopy) noexcept;
		DLLTREATMENT virtual ~IndexBufferClass() noexcept(false) override = default;
		//wont copy data on GPU, only allocate a space on GPU
		DLLTREATMENT virtual IndexBufferClass& operator=(const IndexBufferClass& toCopy);
	private:
		inline virtual Primitives::IndexBufferClass& operator=(Primitives::IndexBufferClass&& toCopy) override final { return operator=(dynamic_cast<IndexBufferClass&&>(toCopy)); }
	public:
		DLLTREATMENT virtual IndexBufferClass& operator=(IndexBufferClass&& toCopy);

		using Primitives::IndexBufferClass::gID;
		using Primitives::IndexBufferClass::operator IndexBufferID_Type;

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

		DLLTREATMENT void CopyData(const IndexBufferClass& srcBuffer);
		DLLTREATMENT void CopySubData(const IndexBufferClass& srcBuffer, size_t srcOffsetInBytes, size_t dstOffsetInBytes, size_t amountOfBytesToCopy);

		//data should point to already allocated memory
		DLLTREATMENT void GetSubData(size_t offsetInBytes, size_t amountOfBytesToCopy, void* data) const;
		//data should point to already allocated memory
		DLLTREATMENT void GetData(void* data) const;

		inline BufferReadWriteModeEnum gBufferReadWriteModeEnum() const noexcept { return BufferReadWriteMode; }
		inline size_t gDataSizeInBytes() const noexcept { return DataSizeInBytes; }

		using Primitives::IndexBufferClass::Bind;
		using Primitives::IndexBufferClass::Unbind;

	};
}