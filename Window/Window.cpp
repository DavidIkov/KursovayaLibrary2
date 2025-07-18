#include"glad/glad.h"
#include"glfw/include/GLFW/glfw3.h"
#include"Window.hpp"
#include"Tools/GLDebug.hpp"
#include<iostream>

using namespace KL2::Window;

bool CWindow::FirstWindow = true;

void CWindow::ClearColorBuffer() {
    glSC(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    glSC(glClear, GL_COLOR_BUFFER_BIT);
}
void CWindow::ClearDepthBuffer() {
    glSC(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    glSC(glClear, GL_DEPTH_BUFFER_BIT);
}
void CWindow::ClearStencilBuffer() {
    glSC(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    glSC(glClear, GL_STENCIL_BUFFER_BIT);
}
void CWindow::ClearAllBuffers() {
    glSC(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    glSC(glClear, GL_COLOR_BUFFER_BIT);
    glSC(glClear, GL_DEPTH_BUFFER_BIT);
    glSC(glClear, GL_STENCIL_BUFFER_BIT);
}

CKeyboard& CWindow::GetKeyboardHandle() {
    return KeyboardHandle;
}
CMouse& CWindow::GetMouseHandle() {
    return MouseHandle;
}

CWindow::CWindow(Vector2U& size, const char* title, bool fullscreen, int vsyncValue, CWindow* shareContextWith) {
    
    glfwSetErrorCallback(+[](int code, const char* msg) {
        ErrorsSystem::SendError << "GLFW error with code [" << std::to_string(code) << "] and message: [" << msg << "]" >> SGLFW_ErrorWrapper(code);
        });

    GLFW_WindowPtr = glfwCreateWindow(size[0], size[1], title, fullscreen ? glfwGetPrimaryMonitor() : NULL, (GLFWwindow*)(shareContextWith ? shareContextWith->GLFW_WindowPtr : nullptr));

    size = GetWindowSize();

    glfwSetWindowUserPointer((GLFWwindow*)GLFW_WindowPtr, (void*)this);

    BindContext();

    glfwSwapInterval(vsyncValue);

    glfwSetMouseButtonCallback((GLFWwindow*)GLFW_WindowPtr, [](GLFWwindow* window, int button, int action, int mods) {
        ((CWindow*)glfwGetWindowUserPointer(window))->MouseHandle._GLFW_KEYCALLBACK(button, action, mods);
        });

    glfwSetKeyCallback((GLFWwindow*)GLFW_WindowPtr, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        ((CWindow*)glfwGetWindowUserPointer(window))->KeyboardHandle._GLFW_KEYCALLBACK(key, scancode, action, mods);
        });

    if (FirstWindow) {
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        std::cout << "OPENGL VERSION: " << glGetString(GL_VERSION) << std::endl;
    }

    FirstWindow = false;
}
void CWindow::SetCursorMode(ECursorModes mode) {
    glfwSetInputMode((GLFWwindow*)GLFW_WindowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
}
CWindow::ECursorModes CWindow::GetCursorMode() const {
    return (ECursorModes)(glfwGetInputMode((GLFWwindow*)GLFW_WindowPtr, GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
}
bool CWindow::GetIsWindowWaitingToClose() const {
    return glfwWindowShouldClose((GLFWwindow*)GLFW_WindowPtr);
}
Vector2U CWindow::GetWindowSize() const {
    int w, h;
    glfwGetWindowSize((GLFWwindow*)GLFW_WindowPtr, &w, &h);
    return Vector2U{ (unsigned)w, (unsigned)h };
}
Vector2I CWindow::GetCursorPosition() const {
    double x, y;
    glfwGetCursorPos((GLFWwindow*)GLFW_WindowPtr, &x, &y);
    return Vector2I{ (int)x, (int)y };
}
void CWindow::SwapScreenBuffers() {
    glfwSwapBuffers((GLFWwindow*)GLFW_WindowPtr);
}
void CWindow::ProcessEvents() {
    glfwPollEvents();
}
void CWindow::WaitTillEvent() {
    glfwWaitEvents();
}
bool CWindow::GetIsWindowResizable() const {
    return glfwGetWindowAttrib((GLFWwindow*)GLFW_WindowPtr, GLFW_RESIZABLE);
}
void CWindow::SetIsWindowResizable(bool resizable) const {
    glfwSetWindowAttrib((GLFWwindow*)GLFW_WindowPtr, GLFW_RESIZABLE, resizable);
}
void CWindow::BindContext() const {
    glfwMakeContextCurrent((GLFWwindow*)GLFW_WindowPtr);
}
void CWindow::UnbindContext() {
    glfwMakeContextCurrent(nullptr);
}
void CWindow::Destroy() {
    if (GLFW_WindowPtr) {
        if (glfwGetCurrentContext() == GLFW_WindowPtr) UnbindContext();
        glfwDestroyWindow((GLFWwindow*)GLFW_WindowPtr);
        GLFW_WindowPtr = nullptr;
    }
}
CWindow::~CWindow() {
    Destroy();
}

CWindow::MonitorDataStruct CWindow::GetPrimaryMonitorData() {
    const GLFWvidmode* data = glfwGetVideoMode(glfwGetPrimaryMonitor());
    return MonitorDataStruct{ Vector2U{(unsigned)data->width,(unsigned)data->height},
        Vector3U{(unsigned)data->redBits,(unsigned)data->greenBits,(unsigned)data->blueBits},(unsigned)data->refreshRate };
}
