#include"KL2_Manager.hpp"
#include"stb_image/stb_image.h"
#include"GLFW/glfw3.h"
#include<iostream>

size_t KL2::Manager::CKL2::_GetVoidPtrSizeInKL2() noexcept { return sizeof(void*); }

void KL2::Manager::CKL2::_InitializeKL2(unsigned openglVersionMajor, unsigned openglVersionMinor) {

    if (!glfwInit()) {
        ErrorsSystem::SendError << "Failed to initialize GLFW" >> SErrorsEnumWrapper(ErrorsEnum::FailedToInitializeGLFW);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
}


void KL2::Manager::CKL2::_UninitializeKL2() {
    glfwTerminate();
}