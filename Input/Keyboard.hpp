#pragma once
#include"Tools/Event.hpp"
#include<cstdint>
#include<bitset>

class CKeyboard {

public:
    enum class EPressableKeys :uint8_t {
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        LeftShift, LeftControl, LeftAlt, LeftSuper, RightShift, RightControl, RightAlt, RightSuper,

        ENUMEND
    };
private:

    std::bitset<(size_t)EPressableKeys::ENUMEND> PressableKeysStates;

    //parameters: bool if it was pressed down or up:true=down,false=up
    CEvent PressableKeysEvents[(unsigned short int)EPressableKeys::ENUMEND];

public:

    KL2_API const CEvent& gPressableKeyEvent(EPressableKeys key) const;

    //not for user
    void _GLFW_KEYCALLBACK(int key, int scancode, int action, int mods);


    KL2_API bool gPressableKeyState(EPressableKeys key) const;

};
