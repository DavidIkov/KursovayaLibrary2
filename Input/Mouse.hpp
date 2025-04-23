#pragma once
#include"Tools/Event.hpp"
#include<bitset>

class CMouse {

public:
    enum class PressableKeysEnum :unsigned short int {
        Left, Right, Middle,

        ENUMEND
    };
private:

    std::bitset<(size_t)PressableKeysEnum::ENUMEND> PressableKeysStates;

    //parameters: bool if it was pressed down or up:true=down,false=up
    CEvent PressableKeysEvents[(unsigned short int)PressableKeysEnum::ENUMEND];

public:

    KL2_API const CEvent& gMousePressableKeyEvent(PressableKeysEnum key) const;

    //not for user
    void _GLFW_KEYCALLBACK(int button, int action, int mods);


    KL2_API bool gMousePressableKeyState(PressableKeysEnum key) const;
};