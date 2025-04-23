#pragma once
#include"KL2_API.h"
#include"Tools/ErrorsSystem.hpp"

namespace KL2::Graphics::Primitives {

    class CShader {
        unsigned int ID = 0u;
    public:
        enum class ETypes :uint8_t {
            Fragment, Vertex, Geometry
        };
    protected:
        const ETypes ShaderType;
    public:

        struct SErrorsEnumWrapper :KL2::ErrorsSystem::SErrorBase {
            enum EErrors :uint8_t {
                FailedToCompile,
            };
            EErrors Error;
            SErrorsEnumWrapper(EErrors error) :Error(error) {};
        }; using ErrorsEnum = SErrorsEnumWrapper; using AnyError = SErrorsEnumWrapper;

        KL2_API CShader(const char* filePath, ETypes typ);
        KL2_API CShader(ETypes typ, const char* code);
        KL2_API CShader(CShader&& toCopy) noexcept;
        KL2_API CShader& operator=(CShader&& toCopy);
        KL2_API void Compile() const;

        typedef unsigned int ID_t;
        ID_t gID() const noexcept { return ID; }
        operator ID_t() const noexcept { return ID; }

        KL2_API virtual ~CShader() noexcept(false);
    };
}