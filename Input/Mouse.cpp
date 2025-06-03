#include"Mouse.hpp"
#include"glfw/include/GLFW/glfw3.h"


const CEvent& CMouse::gMousePressableKeyEvent(EPressableKeys key) const {
    return PressableKeysEvents[(unsigned int)key];
}

void CMouse::_GLFW_KEYCALLBACK(int button, int action, int mods) {
    if (button >= GLFW_MOUSE_BUTTON_LEFT and button <= GLFW_MOUSE_BUTTON_MIDDLE) {
        unsigned int ind = button - GLFW_MOUSE_BUTTON_LEFT;
        PressableKeysStates.set(ind, action != GLFW_RELEASE);
        if (action == GLFW_PRESS) { bool v = true; PressableKeysEvents[ind].FireEvent(&v); }
        else if (action == GLFW_RELEASE) { bool v = false; PressableKeysEvents[ind].FireEvent(&v); }
    }
}


bool CMouse::gMousePressableKeyState(EPressableKeys key) const {
    return PressableKeysStates[(unsigned int)key];
}
