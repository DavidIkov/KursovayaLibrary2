#include"ShaderProgram.hpp"
#include"glad/glad.h"
#include<vector>
#include<string>
#include"Tools/GLDebug.hpp"

using namespace KL2;
using namespace Graphics::Primitives;

CShaderProgram::CShaderProgram() {
    ID = glSC(glCreateProgram);
}
CShaderProgram::CShaderProgram(const char* vsPath, const char* fsPath) :CShaderProgram() {
    CShader VS(vsPath, CShader::ETypes::Vertex);
    VS.Compile();
    AttachShader(VS.gID());

    CShader FS(fsPath, CShader::ETypes::Fragment);
    FS.Compile();
    AttachShader(FS.gID());

    LinkShaders();
}
CShaderProgram::CShaderProgram(const char* vsPath, const char* gsPath, const char* fsPath) :CShaderProgram() {
    CShader VS(vsPath, CShader::ETypes::Vertex);
    VS.Compile();
    AttachShader(VS.gID());

    CShader GS(gsPath, CShader::ETypes::Geometry);
    GS.Compile();
    AttachShader(GS.gID());

    CShader FS(fsPath, CShader::ETypes::Fragment);
    FS.Compile();
    AttachShader(FS.gID());

    LinkShaders();
}

CShaderProgram::CShaderProgram(CShaderProgram&& toCopy) noexcept :ID(toCopy.ID) {
    toCopy.ID = 0u;
}
CShaderProgram& CShaderProgram::operator=(CShaderProgram&& toCopy) {
    this->~CShaderProgram();
    new(this) CShaderProgram(std::move(toCopy));
    return *this;
}
void CShaderProgram::AttachShader(unsigned int id) const {
    glSC(glAttachShader, ID, id);
}
void CShaderProgram::LinkShaders() const {
    glSC(glLinkProgram, ID);
    {//check for linking
        int success = 0; glSC(glGetProgramiv, ID, GL_LINK_STATUS, &success);
        if (success == false) {
            int infoLen = 0; glSC(glGetProgramiv, ID, GL_INFO_LOG_LENGTH, &infoLen);
            std::string info(infoLen - 1, '\0');
            glSC(glGetProgramInfoLog, ID, infoLen, nullptr, (char*)info.c_str());
            ErrorsSystem::SendError << "Failed to link shaders, OpenGL returned a message: [" << info << "]" >> SErrorsEnumWrapper(ErrorsEnum::FailedToLinkShaders);
        }
    }
}
CShaderProgram::~CShaderProgram() noexcept(false) {
    if (ID != 0u) {
        Unbind();
        glSC(glDeleteProgram, ID);
        ID = 0u;
    }
}

void CShaderProgram::Bind() const {
    glSC(glUseProgram, ID);
}
void CShaderProgram::Unbind() {
    glSC(glUseProgram, 0);
}

unsigned int CShaderProgram::GetUniformIDByName(const char* name) const {
    Bind();
    int uniformID = glSC(glGetUniformLocation, ID, name);
    return uniformID;
}
CShaderProgram::SUniformData CShaderProgram::GetUniformData(unsigned int index) const {
    Bind();
    SUniformData returnData;
    char name[100] = {};
    int actualNameLength = 0;
    int size = 0;
    glSC(glGetActiveUniform, ID, index, sizeof(name) / sizeof(char), &actualNameLength, &size, &returnData.Type, name);
    if (actualNameLength == (sizeof(name) / sizeof(char) - 1))
        ErrorsSystem::SendError << "name of uniform is too long, current limit is: [" << std::to_string(sizeof(name) / sizeof(char)) << "]" >> SErrorsEnumWrapper(ErrorsEnum::UniformNameIsTooLarge);
    returnData.Size = size;
    returnData.Name = std::string(name);
    returnData.ID = GetUniformIDByName(name);
    return returnData;
}
std::vector<CShaderProgram::SUniformData> CShaderProgram::GetUniformsData() const {
    Bind();
    int amountOfUniforms = 0;
    glSC(glGetProgramiv, ID, GL_ACTIVE_UNIFORMS, &amountOfUniforms);

    std::vector<SUniformData> arr;
    arr.reserve(amountOfUniforms);

    for (int i = 0; i < amountOfUniforms; i++)
        arr[i] = GetUniformData(i);

    return arr;
}


#define uniformCOPYPASTE(funcName, ...) Bind(); glSC(funcName, uniformID, __VA_ARGS__);

void CShaderProgram::SetUniform1f(unsigned int uniformID, float v0) const { uniformCOPYPASTE(glUniform1f, v0); }
void CShaderProgram::SetUniform2f(unsigned int uniformID, float v0, float v1) const { uniformCOPYPASTE(glUniform2f, v0, v1); }
void CShaderProgram::SetUniform3f(unsigned int uniformID, float v0, float v1, float v2) const { uniformCOPYPASTE(glUniform3f, v0, v1, v2); }
void CShaderProgram::SetUniform4f(unsigned int uniformID, float v0, float v1, float v2, float v3) const { uniformCOPYPASTE(glUniform4f, v0, v1, v2, v3); }
void CShaderProgram::SetUniform1i(unsigned int uniformID, int v0) const { uniformCOPYPASTE(glUniform1i, v0); }
void CShaderProgram::SetUniform2i(unsigned int uniformID, int v0, int v1) const { uniformCOPYPASTE(glUniform2i, v0, v1); }
void CShaderProgram::SetUniform3i(unsigned int uniformID, int v0, int v1, int v2) const { uniformCOPYPASTE(glUniform3i, v0, v1, v2); }
void CShaderProgram::SetUniform4i(unsigned int uniformID, int v0, int v1, int v2, int v3) const { uniformCOPYPASTE(glUniform4i, v0, v1, v2, v3); }
void CShaderProgram::SetUniform1ui(unsigned int uniformID, unsigned int v0) const { uniformCOPYPASTE(glUniform1ui, v0); }
void CShaderProgram::SetUniform2ui(unsigned int uniformID, unsigned int v0, unsigned int v1) const { uniformCOPYPASTE(glUniform2ui, v0, v1); }
void CShaderProgram::SetUniform3ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2) const { uniformCOPYPASTE(glUniform3ui, v0, v1, v2); }
void CShaderProgram::SetUniform4ui(unsigned int uniformID, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const { uniformCOPYPASTE(glUniform4ui, v0, v1, v2, v3); }
void CShaderProgram::SetUniform1fv(unsigned int uniformID, unsigned int count, const float* value) const { uniformCOPYPASTE(glUniform1fv, count, value); }
void CShaderProgram::SetUniform2fv(unsigned int uniformID, unsigned int count, const float* value) const { uniformCOPYPASTE(glUniform2fv, count, value); }
void CShaderProgram::SetUniform3fv(unsigned int uniformID, unsigned int count, const float* value) const { uniformCOPYPASTE(glUniform3fv, count, value); }
void CShaderProgram::SetUniform4fv(unsigned int uniformID, unsigned int count, const float* value) const { uniformCOPYPASTE(glUniform4fv, count, value); }
void CShaderProgram::SetUniform1iv(unsigned int uniformID, unsigned int count, const int* value) const { uniformCOPYPASTE(glUniform1iv, count, value); }
void CShaderProgram::SetUniform2iv(unsigned int uniformID, unsigned int count, const int* value) const { uniformCOPYPASTE(glUniform2iv, count, value); }
void CShaderProgram::SetUniform3iv(unsigned int uniformID, unsigned int count, const int* value) const { uniformCOPYPASTE(glUniform3iv, count, value); }
void CShaderProgram::SetUniform4iv(unsigned int uniformID, unsigned int count, const int* value) const { uniformCOPYPASTE(glUniform4iv, count, value); }
void CShaderProgram::SetUniform1uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const { uniformCOPYPASTE(glUniform1uiv, count, value); }
void CShaderProgram::SetUniform2uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const { uniformCOPYPASTE(glUniform2uiv, count, value); }
void CShaderProgram::SetUniform3uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const { uniformCOPYPASTE(glUniform3uiv, count, value); }
void CShaderProgram::SetUniform4uiv(unsigned int uniformID, unsigned int count, const unsigned int* value) const { uniformCOPYPASTE(glUniform4uiv, count, value); }
void CShaderProgram::SetUniformMatrix2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix2fv, count, transpose, value); }
void CShaderProgram::SetUniformMatrix3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix3fv, count, transpose, value); }
void CShaderProgram::SetUniformMatrix4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix4fv, count, transpose, value); }
void CShaderProgram::SetUniformMatrix2x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix2x3fv, count, transpose, value); }
void CShaderProgram::SetUniformMatrix3x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix3x2fv, count, transpose, value); }
void CShaderProgram::SetUniformMatrix2x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix2x4fv, count, transpose, value); }
void CShaderProgram::SetUniformMatrix4x2fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix4x2fv, count, transpose, value); }
void CShaderProgram::SetUniformMatrix3x4fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix3x4fv, count, transpose, value); }
void CShaderProgram::SetUniformMatrix4x3fv(unsigned int uniformID, unsigned int count, bool transpose, const float* value) const { uniformCOPYPASTE(glUniformMatrix4x3fv, count, transpose, value); }

