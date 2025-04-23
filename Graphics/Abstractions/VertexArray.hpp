#pragma once
#include"Graphics/Primitives/VertexArray.h"

namespace KE2::Graphics::Abstractions {
	class VertexArrayClass :protected Primitives::VertexArrayClass {
	protected:
		std::vector<AttributeDataStruct> AttribsData;
	public:
		using AttributeDataStruct = Primitives::VertexArrayClass::AttributeDataStruct;

		struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
			enum ErrorsEnum {
				TryingToChangeAttributes,
			};
			ErrorsEnum Error;
			inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
		}; using ErrorsEnum = ErrorsEnumWrapperStruct::ErrorsEnum; using AnyError = ErrorsEnumWrapperStruct;


		inline Primitives::VertexArrayClass& gPrimitiveVertexArrayClass() noexcept { return *this; }
		inline const Primitives::VertexArrayClass& gPrimitiveVertexArrayClass() const noexcept { return *this; }


		DLLTREATMENT VertexArrayClass(const ArrayView<AttributeDataStruct>& attribsData);
		DLLTREATMENT VertexArrayClass(const VertexArrayClass& toCopy);
		DLLTREATMENT VertexArrayClass(VertexArrayClass&& toCopy) noexcept;
		DLLTREATMENT virtual ~VertexArrayClass() noexcept(false) override;
		DLLTREATMENT virtual VertexArrayClass& operator=(const VertexArrayClass& toCopy);
	private:
		inline virtual Primitives::VertexArrayClass& operator=(Primitives::VertexArrayClass&& toCopy) override final { return operator=(dynamic_cast<VertexArrayClass&&>(toCopy)); }
	public:
		DLLTREATMENT virtual VertexArrayClass& operator=(VertexArrayClass&& toCopy);

		DLLTREATMENT void SetAttributes(const ArrayView<AttributeDataStruct>& attribsData) override final;
		DLLTREATMENT virtual void SetAttribute(unsigned int attribInd, const AttributeDataStruct& attribData) override final;

		using Primitives::VertexArrayClass::EnableAttribute;
		using Primitives::VertexArrayClass::DisableAttribute;

		DLLTREATMENT void Change_VB_ID_InAttribute(unsigned int attribInd, Primitives::VertexBufferClass::VertexBufferID_Type vb_id);
		DLLTREATMENT void Change_Normalize_InAttribute(unsigned int attribInd, bool normalize);
		DLLTREATMENT void Change_Divisor_InAttribute(unsigned int attribInd, unsigned int divisor);
		DLLTREATMENT void Change_ComponentsAmount_InAttribute(unsigned int attribInd, unsigned int componentsAmount);
		DLLTREATMENT void Change_ByteOffsetToNextElement_InAttribute(unsigned int attribInd, unsigned int byteOffsetToNextElement);
		DLLTREATMENT void Change_DataTypeInMemory_InAttribute(unsigned int attribInd, AttributeDataStruct::DataTypeInMemory_Enum dataTypeInMemory);
		DLLTREATMENT void Change_DataTypeForReadingOnGPU_InAttribute(unsigned int attribInd, AttributeDataStruct::DataTypeForReadingOnGPU_Enum dataTypeForReadingOnGPU);

		inline const AttributeDataStruct& gAttributeData(unsigned int ind) const noexcept { return AttribsData[ind]; }
		inline const std::vector<AttributeDataStruct>& gAttributesData() const noexcept { return AttribsData; }

		using Primitives::VertexArrayClass::Bind;
		using Primitives::VertexArrayClass::Unbind;

	};
}
