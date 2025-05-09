#pragma once
#include"Tools/Event.hpp"
#include<bitset>

class CKeyboard {

public:
    enum class PressableKeysEnum :unsigned short int {
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        LeftShift, LeftControl, LeftAlt, LeftSuper, RightShift, RightControl, RightAlt, RightSuper,

        ENUMEND
    };
private:

    std::bitset<(size_t)PressableKeysEnum::ENUMEND> PressableKeysStates;

    //parameters: bool if it was pressed down or up:true=down,false=up
    CEvent PressableKeysEvents[(unsigned short int)PressableKeysEnum::ENUMEND];

public:

    KL2_API const CEvent& gPressableKeyEvent(PressableKeysEnum key) const;

    //not for user
    void _GLFW_KEYCALLBACK(int key, int scancode, int action, int mods);


    KL2_API bool gPressableKeyState(PressableKeysEnum key) const;

};
