#include"VertexArray.h"

using namespace KE2;
using namespace Graphics::Abstractions;
namespace GP = Graphics::Primitives;

VertexArrayClass::VertexArrayClass(const ArrayView<AttributeDataStruct>& attribsData) :
	GP::VertexArrayClass(attribsData), AttribsData(attribsData) {};
VertexArrayClass::VertexArrayClass(const VertexArrayClass& toCopy) :
	GP::VertexArrayClass(std::vector<AttributeDataStruct>(toCopy.AttribsData)), AttribsData(toCopy.AttribsData) {};
VertexArrayClass::VertexArrayClass(VertexArrayClass&& toCopy) noexcept :
	GP::VertexArrayClass(std::move(toCopy)), AttribsData(std::move(toCopy.AttribsData)) {};
VertexArrayClass::~VertexArrayClass(){}
VertexArrayClass& VertexArrayClass::operator=(const VertexArrayClass& toCopy) {
	this->~VertexArrayClass();
	new(this) VertexArrayClass(toCopy);
	return *this;
}
VertexArrayClass& VertexArrayClass::operator=(VertexArrayClass&& toCopy) {
	this->~VertexArrayClass();
	new(this) VertexArrayClass(std::move(toCopy));
	return *this;
}

void VertexArrayClass::SetAttributes(const ArrayView<AttributeDataStruct>& attribsData) {
	AttribsData.reserve(attribsData.gLen());
	GP::VertexArrayClass::SetAttributes(attribsData);
}
void VertexArrayClass::SetAttribute(unsigned int attribInd, const AttributeDataStruct& attribData) {
	AttribsData[attribInd] = attribData;
	GP::VertexArrayClass::SetAttribute(attribInd, attribData);
}
void VertexArrayClass::Change_VB_ID_InAttribute(unsigned int attribInd, GP::VertexBufferClass::VertexBufferID_Type vb_id) 
	{ AttribsData[attribInd].VB_ID = vb_id; SetAttribute(attribInd, AttribsData[attribInd]); }
void VertexArrayClass::Change_Normalize_InAttribute(unsigned int attribInd, bool normalize) 
	{ AttribsData[attribInd].Normalize = normalize; SetAttribute(attribInd, AttribsData[attribInd]); }
void VertexArrayClass::Change_Divisor_InAttribute(unsigned int attribInd, unsigned int divisor) 
	{ AttribsData[attribInd].Divisor = divisor; SetAttribute(attribInd, AttribsData[attribInd]); }
void VertexArrayClass::Change_ComponentsAmount_InAttribute(unsigned int attribInd, unsigned int componentsAmount) 
	{ AttribsData[attribInd].ComponentsAmount = componentsAmount; SetAttribute(attribInd, AttribsData[attribInd]); }
void VertexArrayClass::Change_ByteOffsetToNextElement_InAttribute(unsigned int attribInd, unsigned int byteOffsetToNextElement) 
	{ AttribsData[attribInd].ByteOffsetToNextElement = byteOffsetToNextElement; SetAttribute(attribInd, AttribsData[attribInd]); }
void VertexArrayClass::Change_DataTypeInMemory_InAttribute(unsigned int attribInd, AttributeDataStruct::DataTypeInMemory_Enum dataTypeInMemory) 
	{ AttribsData[attribInd].DataTypeInMemory = dataTypeInMemory; SetAttribute(attribInd, AttribsData[attribInd]); }
void VertexArrayClass::Change_DataTypeForReadingOnGPU_InAttribute(unsigned int attribInd, AttributeDataStruct::DataTypeForReadingOnGPU_Enum dataTypeForReadingOnGPU)
{ AttribsData[attribInd].DataTypeForReadingOnGPU = dataTypeForReadingOnGPU; SetAttribute(attribInd, AttribsData[attribInd]); }





