#include"Shader.hpp"
#include"glad/glad.h"
#include<string>
#include"Tools/GLDebug.hpp"
#include<fstream>

using namespace KL2;
using namespace Graphics::Primitives;

CShader::CShader(const char* filePath, ETypes typ):ShaderType(typ){
    ID = glSC(glCreateShader, (typ == ETypes::Fragment) ? GL_FRAGMENT_SHADER : ((typ == ETypes::Vertex) ? GL_VERTEX_SHADER : GL_GEOMETRY_SHADER));
    std::string srcCode; 
    {
        std::fstream srcFile(filePath, std::ios_base::in); std::string	curLine;
        size_t charactersAmount = 0; while (std::getline(srcFile, curLine)) charactersAmount += curLine.length() + 1;
        srcFile.clear(); srcFile.seekg(0); srcCode.reserve(charactersAmount);
        while (std::getline(srcFile, curLine)) srcCode += curLine + '\n';
    }
    const char* code = srcCode.c_str();
    glSC(glShaderSource, ID, 1, &code, nullptr);
}
CShader::CShader(ETypes typ, const char* code):ShaderType(typ) {
    ID = glSC(glCreateShader, (typ == ETypes::Fragment) ? GL_FRAGMENT_SHADER : ((typ == ETypes::Vertex) ? GL_VERTEX_SHADER : GL_GEOMETRY_SHADER));
    glSC(glShaderSource, ID, 1, &code, nullptr);
}
CShader::CShader(CShader&& toCopy) noexcept : ID(toCopy.ID),ShaderType(toCopy.ShaderType){
    toCopy.ID = 0u;
}
CShader& CShader::operator=(CShader&& toCopy) {
    this->~CShader();
    new(this) CShader(std::move(toCopy));
    return *this;
}
CShader::~CShader() noexcept(false) {
    if (ID != 0u) {
        glSC(glDeleteShader, ID);
        ID = 0u;
    }
}
void CShader::Compile() const {
    glSC(glCompileShader, ID);
    {//check for compilation
        int success = 0; glSC(glGetShaderiv, ID, GL_COMPILE_STATUS, &success);
        if (success == false) {
            int infoLen = 0; glSC(glGetShaderiv, ID, GL_INFO_LOG_LENGTH, &infoLen);
            std::string info(infoLen - 1, '\0');
            glSC(glGetShaderInfoLog, ID, infoLen, nullptr, (char*)info.c_str());
            ErrorsSystem::SendError << "Failed to compile " <<
                ((ShaderType == ETypes::Fragment) ? "fragment" : ((ShaderType == ETypes::Vertex) ? "vertex" : "geometry")) <<
                " shader, OpenGL returned a message: [" << info << "]" >> SErrorsEnumWrapper(ErrorsEnum::FailedToCompile);
        }
    }
}