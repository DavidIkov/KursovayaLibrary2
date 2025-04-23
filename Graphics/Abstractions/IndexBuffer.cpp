#include"IndexBuffer.h"
#include<string>
#include"Tools/FileTypesReaders/Obj.h"

using namespace KE2;
using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

IndexBufferClass::IndexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode) :BufferReadWriteMode(bufferReadWriteMode) {}
IndexBufferClass::IndexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const ArrayView<void>& data) :
	BufferReadWriteMode(bufferReadWriteMode), DataSizeInBytes(data.gLenInBytes()) {
	GP::IndexBufferClass::SetData(data, bufferReadWriteMode);
}
IndexBufferClass::IndexBufferClass(const IndexBufferClass& toCopy, bool copyBufferData) :
	BufferReadWriteMode(toCopy.BufferReadWriteMode), DataSizeInBytes(toCopy.DataSizeInBytes) {
	GP::IndexBufferClass::SetData(ArrayView<void>(nullptr, DataSizeInBytes), BufferReadWriteMode);
	if (copyBufferData) GP::IndexBufferClass::CopySubData(toCopy, 0, 0, DataSizeInBytes);
}
IndexBufferClass::IndexBufferClass(IndexBufferClass&& toCopy) noexcept :
	GP::IndexBufferClass(std::move(toCopy)),
	BufferReadWriteMode(toCopy.BufferReadWriteMode), DataSizeInBytes(toCopy.DataSizeInBytes) {};
IndexBufferClass& IndexBufferClass::operator=(const IndexBufferClass& toCopy) {
	this->~IndexBufferClass();
	new(this) IndexBufferClass(toCopy, false);
	return *this;
}
IndexBufferClass& IndexBufferClass::operator=(IndexBufferClass&& toCopy) {
	this->~IndexBufferClass();
	new(this) IndexBufferClass(std::move(toCopy));
	return *this;
}


void IndexBufferClass::SetData(const ArrayView<void>& data) {
	if (DataSizeInBytes == data.gLenInBytes())
		GP::IndexBufferClass::SetSubData(0, data);
	else {
		DataSizeInBytes = data.gLenInBytes();
		GP::IndexBufferClass::SetData(data, BufferReadWriteMode);
	}
}
void IndexBufferClass::SetSubData(size_t offsetInBytes, const ArrayView<void>& data) {
	GP::IndexBufferClass::SetSubData(offsetInBytes, data);
}

void IndexBufferClass::CopyData(const IndexBufferClass& srcBuffer) {
	if (DataSizeInBytes == srcBuffer.DataSizeInBytes) GP::IndexBufferClass::CopySubData(srcBuffer, 0, 0, DataSizeInBytes);
	else {
		DataSizeInBytes = srcBuffer.DataSizeInBytes;
		GP::IndexBufferClass::SetData(ArrayView<void>(nullptr, DataSizeInBytes), BufferReadWriteMode);
		GP::IndexBufferClass::CopySubData(srcBuffer, 0, 0, DataSizeInBytes);
	}
}
void IndexBufferClass::CopySubData(const IndexBufferClass& srcBuffer, size_t srcOffsetInBytes, size_t dstOffsetInBytes, size_t amountOfBytesToCopy) {
	GP::IndexBufferClass::CopySubData(srcBuffer, srcOffsetInBytes, dstOffsetInBytes, amountOfBytesToCopy);
}

void IndexBufferClass::GetSubData(size_t offsetInBytes, size_t amountOfBytesToCopy, void* data) const {
	GP::IndexBufferClass::GetSubData(offsetInBytes, amountOfBytesToCopy, data);
}
void IndexBufferClass::GetData(void* data) const {
	GP::IndexBufferClass::GetSubData(0, DataSizeInBytes, data);
}


