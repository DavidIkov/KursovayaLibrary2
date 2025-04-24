#include"VertexArray.hpp"
#include"Tools/GLDebug.hpp"
#include"glad/glad.h"

using namespace KL2;
using namespace Graphics::Primitives;

CVertexArray::CVertexArray() {
    glSC(glGenVertexArrays(1, &ID));
    Bind();
}
CVertexArray::CVertexArray(const CArrayView<SAttributeData>& attribsData) :CVertexArray() {
    SetAttributes(attribsData);
}
CVertexArray::CVertexArray(CVertexArray&& toCopy) noexcept :
    ID(toCopy.ID) {
    toCopy.ID = 0u;
}
CVertexArray& CVertexArray::operator=(CVertexArray&& toCopy) {
    this->~CVertexArray();
    new(this) CVertexArray(std::move(toCopy));
    return *this;
}
CVertexArray::~CVertexArray() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteVertexArrays(1, &ID));
        ID = 0u;
    }
}
void CVertexArray::SetAttribute(size_t attribInd, const SAttributeData& attribData) {
    Bind();
    if (attribData.VB_ID == 0)
        ErrorsSystem::SendError << "Cant use zero vertex buffer for attribute" >> SErrorsEnumWrapper(ErrorsEnum::ZeroVB_CantBeUsed);

    EnableAttribute(attribInd);

    glSC(glBindBuffer(GL_ARRAY_BUFFER, attribData.VB_ID));
    unsigned int glDataTypeOnCPU = 0;
    switch (attribData.DataTypeInMemory) {
    case SAttributeData::EDataTypeInMemory::Byte: glDataTypeOnCPU = GL_BYTE; break;
    case SAttributeData::EDataTypeInMemory::UnsignedByte: glDataTypeOnCPU = GL_UNSIGNED_BYTE; break;
    case SAttributeData::EDataTypeInMemory::Float: glDataTypeOnCPU = GL_FLOAT; break;
    case SAttributeData::EDataTypeInMemory::Int: glDataTypeOnCPU = GL_INT; break;
    case SAttributeData::EDataTypeInMemory::UnsignedInt: glDataTypeOnCPU = GL_UNSIGNED_INT; break;
    case SAttributeData::EDataTypeInMemory::Double: glDataTypeOnCPU = GL_DOUBLE; break;
    }

    switch (attribData.DataTypeForReadingOnGPU) {
    case SAttributeData::EDataTypeForReadingOnGPU::Float: {
        glSC(glVertexAttribPointer(attribInd, attribData.ComponentsAmount, glDataTypeOnCPU, attribData.Normalize, attribData.ByteOffsetToNextAttribute, (const void*)(unsigned long long int)attribData.FirstAttributeByteOffset));
        break;
    } case SAttributeData::EDataTypeForReadingOnGPU::Int: {
        glSC(glVertexAttribIPointer(attribInd, attribData.ComponentsAmount, glDataTypeOnCPU, attribData.ByteOffsetToNextAttribute, (const void*)attribData.FirstAttributeByteOffset));
        break;
    } case SAttributeData::EDataTypeForReadingOnGPU::Double: {
        glSC(glVertexAttribLPointer(attribInd, attribData.ComponentsAmount, glDataTypeOnCPU, attribData.ByteOffsetToNextAttribute, (const void*)attribData.FirstAttributeByteOffset));
        break;
    }
    }

    glSC(glVertexAttribDivisor(attribInd, attribData.Divisor));
}
void CVertexArray::SetAttributes(const CArrayView<SAttributeData>& attribsData) {
    Bind();

    for (unsigned int i = 0; i < attribsData.GetLen(); i++)
        SetAttribute(i, attribsData[i]);

    glSC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
void CVertexArray::EnableAttribute(size_t attribInd) {
    Bind();
    glSC(glEnableVertexAttribArray(attribInd));
}
void CVertexArray::DisableAttribute(size_t attribInd) {
    Bind();
    glSC(glDisableVertexAttribArray(attribInd));
}
void CVertexArray::Bind() const {
    glSC(glBindVertexArray(ID));
}
void CVertexArray::Unbind() {
    glSC(glBindVertexArray(0));
}