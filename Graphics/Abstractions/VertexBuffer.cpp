#include"VertexBuffer.h"
#include<string>
#include"Tools/FileTypesReaders/Obj.h"

using namespace KE2;
using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

VertexBufferClass::VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode) :BufferReadWriteMode(bufferReadWriteMode) {}
VertexBufferClass::VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const ArrayView<void>& data) :
	BufferReadWriteMode(bufferReadWriteMode), DataSizeInBytes(data.gLenInBytes()) {
	GP::VertexBufferClass::SetData(data, bufferReadWriteMode);
}
VertexBufferClass::VertexBufferClass(BufferReadWriteModeEnum bufferReadWriteMode, const char* filePath) :
	BufferReadWriteMode(bufferReadWriteMode) {
	std::string filePathStr(filePath);
	std::string fileType = filePathStr.substr(filePathStr.find_last_of(L'.') + 1);
	if (fileType == "obj") {
		std::vector<float> data = ReadObjFileType(filePath);
		DataSizeInBytes = data.size() * sizeof(float);
		GP::VertexBufferClass::SetData(ArrayView<void>(&data[0], DataSizeInBytes), bufferReadWriteMode);
	}
	else {
		ErrorsSystemNamespace::SendError << "provided type of file: [" << fileType << "] is not supported" >> ErrorsEnumWrapperStruct(ErrorsEnum::ProvidedTypeOfFileNotSupported);
	}
}
VertexBufferClass::VertexBufferClass(const VertexBufferClass& toCopy, bool copyBufferData) :
	BufferReadWriteMode(toCopy.BufferReadWriteMode), DataSizeInBytes(toCopy.DataSizeInBytes) {
	GP::VertexBufferClass::SetData(ArrayView<void>(nullptr, DataSizeInBytes), BufferReadWriteMode);
	if (copyBufferData) GP::VertexBufferClass::CopySubData(toCopy, 0, 0, DataSizeInBytes);
}
VertexBufferClass::VertexBufferClass(VertexBufferClass&& toCopy) noexcept :
	GP::VertexBufferClass(std::move(toCopy)),
	BufferReadWriteMode(toCopy.BufferReadWriteMode), DataSizeInBytes(toCopy.DataSizeInBytes) {};
VertexBufferClass& VertexBufferClass::operator=(const VertexBufferClass& toCopy) {
	this->~VertexBufferClass();
	new(this) VertexBufferClass(toCopy, false);
	return *this;
}
VertexBufferClass& VertexBufferClass::operator=(VertexBufferClass&& toCopy) {
	this->~VertexBufferClass();
	new(this) VertexBufferClass(std::move(toCopy));
	return *this;
}


void VertexBufferClass::SetData(const ArrayView<void>& data) {
	if (DataSizeInBytes == data.gLenInBytes())
		GP::VertexBufferClass::SetSubData(0, data);
	else {
		DataSizeInBytes = data.gLenInBytes();
		GP::VertexBufferClass::SetData(data, BufferReadWriteMode);
	}
}
void VertexBufferClass::SetSubData(size_t offsetInBytes, const ArrayView<void>& data) {
	GP::VertexBufferClass::SetSubData(offsetInBytes, data);
}

void VertexBufferClass::CopyData(const VertexBufferClass& srcBuffer) {
	if (DataSizeInBytes == srcBuffer.DataSizeInBytes) GP::VertexBufferClass::CopySubData(srcBuffer, 0, 0, DataSizeInBytes);
	else {
		DataSizeInBytes = srcBuffer.DataSizeInBytes;
		GP::VertexBufferClass::SetData(ArrayView<void>(nullptr, DataSizeInBytes), BufferReadWriteMode);
		GP::VertexBufferClass::CopySubData(srcBuffer, 0, 0, DataSizeInBytes);
	}
}
void VertexBufferClass::CopySubData(const VertexBufferClass& srcBuffer, size_t srcOffsetInBytes, size_t dstOffsetInBytes, size_t amountOfBytesToCopy) {
	GP::VertexBufferClass::CopySubData(srcBuffer, srcOffsetInBytes, dstOffsetInBytes, amountOfBytesToCopy);
}
void VertexBufferClass::GetData(void* data) const {
	GP::VertexBufferClass::GetSubData(0, DataSizeInBytes, data);
}
void VertexBufferClass::GetSubData(size_t offsetInBytes, size_t amountOfBytesToCopy, void* data) const {
	GP::VertexBufferClass::GetSubData(offsetInBytes, amountOfBytesToCopy, data);
}


