#include"Keyboard.hpp"
#include"glfw/include/GLFW/glfw3.h"
#include<iostream>

bool CKeyboard::gPressableKeyState(PressableKeysEnum key) const {
    return PressableKeysStates[(unsigned int)key];
}

const CEvent& CKeyboard::gPressableKeyEvent(PressableKeysEnum key) const {
    return PressableKeysEvents[(unsigned int)key];
}

void CKeyboard::_GLFW_KEYCALLBACK(int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS and action != GLFW_RELEASE) return;

    unsigned short int ind = (unsigned short int)PressableKeysEnum::ENUMEND;

    if (key >= GLFW_KEY_A and key <= GLFW_KEY_Z) ind = key - GLFW_KEY_A;
    else if (key >= GLFW_KEY_LEFT_SHIFT and key <= GLFW_KEY_RIGHT_SUPER) ind = (unsigned short int)(key - GLFW_KEY_LEFT_SHIFT) + (unsigned short int)PressableKeysEnum::LeftShift;

    if (ind == (unsigned short int)PressableKeysEnum::ENUMEND) return;

    bool pressedDown = action == GLFW_PRESS;
    PressableKeysStates.set(ind, pressedDown);
    PressableKeysEvents[ind].FireEvent(&pressedDown);

}

