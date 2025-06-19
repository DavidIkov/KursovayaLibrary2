#pragma once
#include"KL2_API.h"
#include"Tools/ErrorsSystem.hpp"
#include"Tools/ArrayView.hpp"
#include"VertexBuffer.hpp"

namespace KL2::Graphics::Primitives {

    class CVertexArray {
    protected:
        unsigned int ID = 0u;
    public:

        struct SErrorsEnumWrapper :KL2::ErrorsSystem::SErrorBase {
            enum EErrors :uint8_t {
                ZeroVB_CantBeUsed,
            };
            EErrors Error;
            SErrorsEnumWrapper(EErrors error) :Error(error) {};
        }; using ErrorsEnum = SErrorsEnumWrapper; using AnyError = SErrorsEnumWrapper;

        struct SAttributeData {
            //vertex buffer ID
            CVertexBuffer::ID_t VB_ID;
            /* used only if DataOnOnGPU is Float,
            if true then values will be converted to floats with value/maxValueOfThisType,
            otherwise they are converted to floats without any divisions */
            bool Normalize = false;
            /* amount of insances that have to be passed to use next
            vertex buffer data, for example if divisor is 1 then
            every instance data will update, if 2 then every 2 instances and etc,
            used for instanced rendering
            if Divisor is 0 then data will update every vertex(most often usecase) 
            donst ask why name is like that, thats how this is named in OpenGL*/
            unsigned Divisor;
            /* amount of components for a attribute, for example if in result
            you want to have vec2 then this should be 2, vec3 then 3 and etc,
            if just 1 value then 1 by same logic */
            size_t ComponentsAmount;
            size_t FirstAttributeByteOffset;
            /*byte offset representing where is next value will be located */
            size_t ByteOffsetToNextAttribute;
            enum class EDataTypeInMemory :uint8_t {
                Byte, UnsignedByte, Float, Int, UnsignedInt, Double
            } DataTypeInMemory;
            enum class EDataTypeForReadingOnGPU :uint8_t {
                Float, Int, Double
            } DataTypeForReadingOnGPU;
        };

        KL2_API CVertexArray();
        KL2_API CVertexArray(const CArrayView<SAttributeData>& attribsData);
        KL2_API CVertexArray(CVertexArray&& toCopy) noexcept;
        KL2_API virtual CVertexArray& operator=(CVertexArray&& toCopy);
        KL2_API virtual ~CVertexArray() noexcept(false);

        KL2_API virtual void SetAttributes(const CArrayView<SAttributeData>& attribsData);
        KL2_API virtual void SetAttribute(size_t attribInd, const SAttributeData& attribData);
        KL2_API void EnableAttribute(size_t attribInd);
        KL2_API void DisableAttribute(size_t attribInd);

        typedef unsigned int ID_t;
        ID_t gID() const noexcept { return ID; }
        operator ID_t() const noexcept { return ID; }

        KL2_API void Bind() const;
        KL2_API static void Unbind();

    };
}