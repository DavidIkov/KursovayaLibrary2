#include"Texture.h"

using namespace KE2;
using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

TextureClass::TextureClass(DimensionsEnum dimensions, const char* filePath, unsigned int mipmapLevels, SettingsStruct sets, DataSettingsStruct dataSets) :
	GP::TextureClass(dimensions, filePath, &Size, nullptr, mipmapLevels, sets, dataSets), 
	Settings(sets), DataSettings(dataSets), MipmapLevels(mipmapLevels) {}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, const void* data, unsigned int mipmapLevels, SettingsStruct sets, DataSettingsStruct dataSets) :
	GP::TextureClass(dimensions, pixelsAmount, data, mipmapLevels, sets, dataSets), 
	Size(pixelsAmount), Settings(sets), DataSettings(dataSets), MipmapLevels(mipmapLevels) {}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, unsigned int mipmapLevels, DataSettingsStruct::DataFormatOnGPU_Enum dataFormatOnGPU, SettingsStruct sets) :
	GP::TextureClass(dimensions, pixelsAmount, mipmapLevels, dataFormatOnGPU, sets), Size(pixelsAmount),
	Settings(sets), DataSettings{ dataFormatOnGPU }, MipmapLevels(mipmapLevels) {};
TextureClass::TextureClass(const TextureClass& toCopy, bool copyTextureData) :
	GP::TextureClass(toCopy.Dimensions, toCopy.Size, toCopy.MipmapLevels, toCopy.DataSettings.DataFormatOnGPU, Settings),
	Size(toCopy.Size), Settings(toCopy.Settings), DataSettings(toCopy.DataSettings), MipmapLevels(toCopy.MipmapLevels) {
	if (copyTextureData) GP::TextureClass::CopySubData(toCopy, Vector3U(0u), Vector3U(0u), Size);
};
TextureClass::TextureClass(TextureClass&& toCopy) noexcept:
	GP::TextureClass(std::move(toCopy)), Size(toCopy.Size),
	Settings(toCopy.Settings), DataSettings(toCopy.DataSettings), MipmapLevels(toCopy.MipmapLevels) { }
TextureClass& TextureClass::operator=(const TextureClass& toCopy) {
	this->~TextureClass();
	new(this) TextureClass(toCopy, false);
	return *this;
}
TextureClass& TextureClass::operator=(TextureClass&& toCopy) {
	this->~TextureClass();
	new(this) TextureClass(std::move(toCopy));
	return *this;
}
void TextureClass::SetSubData(Vector3U pixelsOffset, Vector3U pixelsAmount, const void* data) {
	GP::TextureClass::SetSubData(pixelsOffset, pixelsAmount, data, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
}
void TextureClass::CopySubData(const TextureClass& srcTex, Vector3U offsetInSrc, Vector3U offsetInDst, Vector3U pixelsAmount) {
	GP::TextureClass::CopySubData(srcTex, offsetInSrc, offsetInDst, pixelsAmount);
}
void TextureClass::GetData(void* buffer) const {
	GP::TextureClass::GetData(buffer, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
}
void TextureClass::GetSubData(Vector3U offset, void* buffer, Vector3U pixelsAmount) const {
	GP::TextureClass::GetSubData(offset, buffer, pixelsAmount, DataSettings.DataFormatOnCPU, DataSettings.DataTypeOnCPU);
}

