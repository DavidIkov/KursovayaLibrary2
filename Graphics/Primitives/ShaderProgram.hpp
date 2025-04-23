#pragma once
#include"Shader.hpp"
#include<string>
#include<vector>

namespace KL2::Graphics::Primitives {

    class CShaderProgram {
    protected:
        unsigned int ID = 0u;
    public:

        struct SErrorsEnumWrapper :KL2::ErrorsSystem::SErrorBase {
            enum EErrors :uint8_t {
                FailedToLinkShaders,
                UniformNameIsTooLarge,
            };
            EErrors Error;
            SErrorsEnumWrapper(EErrors error) :Error(error) {};
        }; using ErrorsEnum = SErrorsEnumWrapper; using AnyError = SErrorsEnumWrapper;

        KL2_API CShaderProgram();
        KL2_API CShaderProgram(const char* vsPath, const char* fsPath);
        KL2_API CShaderProgram(const char* vsPath, const char* gsPath, const char* fsPath);
        KL2_API CShaderProgram(CShaderProgram&& toCopy) noexcept;
        KL2_API CShaderProgram& operator=(CShaderProgram&& toCopy);
        KL2_API void AttachShader(unsigned int id) const;
        KL2_API void LinkShaders() const;
        KL2_API virtual ~CShaderProgram() noexcept(false);

        typedef unsigned int ID_t;
        ID_t gID() const noexcept { return ID; }
        operator ID_t() const noexcept { return ID; }

        KL2_API void Bind() const;
        KL2_API static void Unbind();

        struct SUniformData {
            unsigned int Type;
            unsigned int Size;
            std::string Name;
            unsigned int ID;
        };


        KL2_API unsigned int GetUniformIDByName(const char* name) const;
        KL2_API SUniformData GetUniformData(unsigned int index) const;
        KL2_API std::vector<SUniformData> GetUniformsData() const;

        KL2_API void SetUniform1f(unsigned int uniformID, float v0) const;
        KL2_API void SetUniform2f(unsigned int uniformID, float v0, float v1) const;
        KL2_API void SetUniform3f(unsigned int uniformID, float v0, float v1, float v2) const;
        KL2_API void SetUniform4f(unsigned int uniformID, float v0, float v1, float v2, float v3) const;
        KL2_API void SetUniform1i(unsigned int uniformID, int v0) const;
        KL2_API void SetUniform2i(unsigned int uniformID, int v0, int v1) const;
        KL2_API void SetUniform3i(unsigned int uniformID, int v0, int v1, int v2) const;
        KL2_API void SetUniform4i(unsigned int uniformID, int v0, int v1, int v2, int v3) const;
        KL2_API void SetUniform1ui(unsigned int uniformID, unsigned int v0) const;
        KL2_API void SetUniform2ui(unsigned int uniformID, unsigned int v0, unsigned int v1) const;
        KL2_API void SetUniform3ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2) const;
        KL2_API void SetUniform4ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const;
        KL2_API void SetUniform1fv(unsigned int uniformID, unsigned int count, const float* value) const;
        KL2_API void SetUniform2fv(unsigned int uniformID, unsigned int count, const float* value) const;
        KL2_API void SetUniform3fv(unsigned int uniformID, unsigned int count, const float* value) const;
        KL2_API void SetUniform4fv(unsigned int uniformID, unsigned int count, const float* value) const;
        KL2_API void SetUniform1iv(unsigned int uniformID, unsigned int count, const int* value) const;
        KL2_API void SetUniform2iv(unsigned int uniformID, unsigned int count, const int* value) const;
        KL2_API void SetUniform3iv(unsigned int uniformID, unsigned int count, const int* value) const;
        KL2_API void SetUniform4iv(unsigned int uniformID, unsigned int count, const int* value) const;
        KL2_API void SetUniform1uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const;
        KL2_API void SetUniform2uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const;
        KL2_API void SetUniform3uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const;
        KL2_API void SetUniform4uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const;
        KL2_API void SetUniformMatrix2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
        KL2_API void SetUniformMatrix3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
        KL2_API void SetUniformMatrix4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
        KL2_API void SetUniformMatrix2x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
        KL2_API void SetUniformMatrix3x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
        KL2_API void SetUniformMatrix2x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
        KL2_API void SetUniformMatrix4x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
        KL2_API void SetUniformMatrix3x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;
        KL2_API void SetUniformMatrix4x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const;

        void SetUniform1f(const char* uniformName, float v0) const { SetUniform1f(GetUniformIDByName(uniformName), v0); }
        void SetUniform2f(const char* uniformName, float v0, float v1) const { SetUniform2f(GetUniformIDByName(uniformName), v0, v1); }
        void SetUniform3f(const char* uniformName, float v0, float v1, float v2) const { SetUniform3f(GetUniformIDByName(uniformName), v0, v1, v2); }
        void SetUniform4f(const char* uniformName, float v0, float v1, float v2, float v3) const { SetUniform4f(GetUniformIDByName(uniformName), v0, v1, v2, v3); }
        void SetUniform1i(const char* uniformName, int v0) const { SetUniform1i(GetUniformIDByName(uniformName), v0); }
        void SetUniform2i(const char* uniformName, int v0, int v1) const { SetUniform2i(GetUniformIDByName(uniformName), v0, v1); }
        void SetUniform3i(const char* uniformName, int v0, int v1, int v2) const { SetUniform3i(GetUniformIDByName(uniformName), v0, v1, v2); }
        void SetUniform4i(const char* uniformName, int v0, int v1, int v2, int v3) const { SetUniform4i(GetUniformIDByName(uniformName), v0, v1, v2, v3); }
        void SetUniform1ui(const char* uniformName, unsigned int v0) const { SetUniform1ui(GetUniformIDByName(uniformName), v0); }
        void SetUniform2ui(const char* uniformName, unsigned int v0, unsigned int v1) const { SetUniform2ui(GetUniformIDByName(uniformName), v0, v1); }
        void SetUniform3ui(const char* uniformName, unsigned int v0, unsigned int v1, unsigned int v2) const { SetUniform3ui(GetUniformIDByName(uniformName), v0, v1, v2); }
        void SetUniform4ui(const char* uniformName, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const { SetUniform4ui(GetUniformIDByName(uniformName), v0, v1, v2, v3); }
        void SetUniform1fv(const char* uniformName, unsigned int count, const float* value) const { SetUniform1fv(GetUniformIDByName(uniformName), count, value); }
        void SetUniform2fv(const char* uniformName, unsigned int count, const float* value) const { SetUniform2fv(GetUniformIDByName(uniformName), count, value); }
        void SetUniform3fv(const char* uniformName, unsigned int count, const float* value) const { SetUniform3fv(GetUniformIDByName(uniformName), count, value); }
        void SetUniform4fv(const char* uniformName, unsigned int count, const float* value) const { SetUniform4fv(GetUniformIDByName(uniformName), count, value); }
        void SetUniform1iv(const char* uniformName, unsigned int count, const int* value) const { SetUniform1iv(GetUniformIDByName(uniformName), count, value); }
        void SetUniform2iv(const char* uniformName, unsigned int count, const int* value) const { SetUniform2iv(GetUniformIDByName(uniformName), count, value); }
        void SetUniform3iv(const char* uniformName, unsigned int count, const int* value) const { SetUniform3iv(GetUniformIDByName(uniformName), count, value); }
        void SetUniform4iv(const char* uniformName, unsigned int count, const int* value) const { SetUniform4iv(GetUniformIDByName(uniformName), count, value); }
        void SetUniform1uiv(const char* uniformName, unsigned int count, const unsigned int* value) const { SetUniform1uiv(GetUniformIDByName(uniformName), count, value); }
        void SetUniform2uiv(const char* uniformName, unsigned int count, const unsigned int* value) const { SetUniform2uiv(GetUniformIDByName(uniformName), count, value); }
        void SetUniform3uiv(const char* uniformName, unsigned int count, const unsigned int* value) const { SetUniform3uiv(GetUniformIDByName(uniformName), count, value); }
        void SetUniform4uiv(const char* uniformName, unsigned int count, const unsigned int* value) const { SetUniform4uiv(GetUniformIDByName(uniformName), count, value); }
        void SetUniformMatrix2fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix2fv(GetUniformIDByName(uniformName), count, transpose, value); }
        void SetUniformMatrix3fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix3fv(GetUniformIDByName(uniformName), count, transpose, value); }
        void SetUniformMatrix4fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix4fv(GetUniformIDByName(uniformName), count, transpose, value); }
        void SetUniformMatrix2x3fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix2x3fv(GetUniformIDByName(uniformName), count, transpose, value); }
        void SetUniformMatrix3x2fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix3x2fv(GetUniformIDByName(uniformName), count, transpose, value); }
        void SetUniformMatrix2x4fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix2x4fv(GetUniformIDByName(uniformName), count, transpose, value); }
        void SetUniformMatrix4x2fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix4x2fv(GetUniformIDByName(uniformName), count, transpose, value); }
        void SetUniformMatrix3x4fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix3x4fv(GetUniformIDByName(uniformName), count, transpose, value); }
        void SetUniformMatrix4x3fv(const char* uniformName, unsigned int count, bool transpose, const float* value) const { SetUniformMatrix4x3fv(GetUniformIDByName(uniformName), count, transpose, value); }
    };
}