#pragma once
#include<iostream>
#include"glad/glad.h"
#include"Tools/ErrorsSystem.hpp"

struct SOpenGL_AnyErrorWrapper : KL2::ErrorsSystem::SErrorBase {
    unsigned int ErrorGL_Enum = 0u;
    inline SOpenGL_AnyErrorWrapper(unsigned int errorGL_Enum) noexcept :ErrorGL_Enum(errorGL_Enum) {};
}; using OpenGL_AnyError = SOpenGL_AnyErrorWrapper;

#define glSC(func)  while (glGetError() != GL_NO_ERROR);\
func;\
{unsigned int error = glGetError();\
if (error != GL_NO_ERROR) KL2::ErrorsSystem::SendError<<"OpenGL error, code: ["<<std::to_string(error)<<"]">>SOpenGL_AnyErrorWrapper(error);\
}