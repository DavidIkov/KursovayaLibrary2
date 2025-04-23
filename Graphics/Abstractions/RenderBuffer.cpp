#include"RenderBuffer.h"

using namespace KE2;
using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

RenderBufferClass::RenderBufferClass(Vector2U size, BufferDataFormatEnum bufferDataFormat) :
	GP::RenderBufferClass(size,bufferDataFormat), BufferDataFormat(bufferDataFormat), Size(size) { }
RenderBufferClass::RenderBufferClass(RenderBufferClass&& toCopy) noexcept:
	GP::RenderBufferClass(std::move(toCopy)), BufferDataFormat(toCopy.BufferDataFormat), Size(toCopy.Size) {}
RenderBufferClass::RenderBufferClass(const RenderBufferClass& toCopy) noexcept:
	GP::RenderBufferClass(toCopy.Size, toCopy.BufferDataFormat), BufferDataFormat(toCopy.BufferDataFormat), Size(toCopy.Size) {}
RenderBufferClass& RenderBufferClass::operator=(RenderBufferClass&& toCopy) {
	this->~RenderBufferClass();
	new(this) RenderBufferClass(std::move(toCopy));
	return *this;
}
RenderBufferClass& RenderBufferClass::operator=(const RenderBufferClass& toCopy) {
	this->~RenderBufferClass();
	new(this) RenderBufferClass(toCopy);
	return *this;
}
