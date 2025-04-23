#pragma once
#include"KL2_API.h"
#include"Input/Keyboard.hpp"
#include"Input/Mouse.hpp"
#include"Maths/Vector.hpp"
#include"Tools/ErrorsSystem.hpp"

namespace KL2::Window {

    /*
    TODO: dosent support more than one monitors for now(maybe)(i cant fucking test it bruh)
    TODO: need to make everything about "window context"
    */

    /*
    recommended order is:
    render....;
    SwapScreenBuffers();
    ProcessEvents();
    */
    class CWindow {

    public:
        enum class ECursorModes :uint8_t {
            Free, Hidden, Disabled,
            LockedAndInvisible,
        };
    private:

        void* GLFW_WindowPtr;//GLFWwindow*
        bool Deleted = false;

        Vector2I MouseDelta;
        static bool FirstWindow;//true

        CKeyboard KeyboardHandle;
        CMouse MouseHandle;

    public:

        CWindow(CWindow const&) = delete;
        CWindow& operator=(CWindow const&) = delete;
        CWindow(CWindow&&) = delete;
        CWindow& operator=(CWindow&&) = delete;



        struct SGLFW_ErrorWrapper :KL2::ErrorsSystem::SErrorBase {
            unsigned int ErrCode;
            inline SGLFW_ErrorWrapper(unsigned int errCode) :ErrCode(errCode) {};
        }; using GLFW_AnyError = SGLFW_ErrorWrapper;

        KL2_API const CKeyboard& GetKeyboardHandle() const;
        KL2_API const CMouse& GetMouseHandle() const;

        KL2_API void ClearColorBuffer();
        KL2_API void ClearDepthBuffer();
        KL2_API void ClearStencilBuffer();
        //color,depth,stencil
        KL2_API void ClearAllBuffers();

        //will change width and height to actual window width and height
        KL2_API CWindow(Vector2U& size, const char* title, bool fullscreen, int vsyncValue, CWindow* shareContextWith = nullptr);
        KL2_API bool GetIsWindowWaitingToClose() const;
        KL2_API void SwapScreenBuffers();
        //will proccess events that havent been processed
        KL2_API void ProcessEvents();
        //will stop thread until some event will occur, usefull to not use 100% of cpu in while loop
        KL2_API void WaitTillEvent();
        KL2_API Vector2U GetWindowSize() const;
        KL2_API Vector2I GetCursorPosition() const;
        KL2_API Vector2I GetCursorDelta() const;
        KL2_API void SetCursorMode(ECursorModes mode);
        KL2_API ECursorModes GetCursorMode() const;

        KL2_API bool GetIsWindowResizable() const;
        KL2_API void SetIsWindowResizable(bool resizable) const;

        KL2_API void BindContext() const;
        KL2_API static void UnbindContext();

        KL2_API void Destroy();
        KL2_API ~CWindow();

        struct MonitorDataStruct {
            Vector2U Size;
            Vector3U ColorDepth;
            unsigned int RefreshRate;//in Hz
        };
        static KL2_API MonitorDataStruct GetPrimaryMonitorData();
    };
}