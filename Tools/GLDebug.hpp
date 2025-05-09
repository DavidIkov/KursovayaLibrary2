#pragma once
#include<iostream>
#include"glad/glad.h"
#include"Tools/ErrorsSystem.hpp"

namespace KL2::Manager {
    extern unsigned OpenGL_VersionMajor, OpenGL_VersionMinor;
}

//if enum is zero then it means that called function is not defined in current version of OpenGL
struct SOpenGL_AnyErrorWrapper : KL2::ErrorsSystem::SErrorBase {
    unsigned int ErrorGL_Enum = 0u;
    inline SOpenGL_AnyErrorWrapper(unsigned int errorGL_Enum) noexcept :ErrorGL_Enum(errorGL_Enum) {};
}; using OpenGL_AnyError = SOpenGL_AnyErrorWrapper;

template<typename T, typename...T2>
auto _GLDebug_FuncCallWrapper(const char* funcName, T func, T2&&...vals) {
    while (glGetError() != GL_NO_ERROR);
    if (func == nullptr) {
        KL2::ErrorsSystem::SendError << "OpenGL function " << funcName << " is not defined in " <<
            std::to_string(KL2::Manager::OpenGL_VersionMajor) << "." << std::to_string(KL2::Manager::OpenGL_VersionMinor) >>
            SOpenGL_AnyErrorWrapper(0);
    }
    if constexpr (std::is_same_v<decltype(func(std::forward<T2>(vals)...)), void>) {
        func(std::forward<T2>(vals)...);
        {
            unsigned error = glGetError();
            if (error != GL_NO_ERROR) KL2::ErrorsSystem::SendError << "OpenGL error, code: [" << std::to_string(error) << "]" >> SOpenGL_AnyErrorWrapper(error);
        }
    }
    else {
        auto ret = func(std::forward<T2>(vals)...);
        {
            unsigned error = glGetError();
            if (error != GL_NO_ERROR) KL2::ErrorsSystem::SendError << "OpenGL error, code: [" << std::to_string(error) << "]" >> SOpenGL_AnyErrorWrapper(error);
        }
        return ret;
    }
}

#define glSC(func, ...) _GLDebug_FuncCallWrapper(#func, func __VA_OPT__(,) __VA_ARGS__)