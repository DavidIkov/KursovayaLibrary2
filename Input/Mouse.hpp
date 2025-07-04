#pragma once
#include"KL2_API.h"
#include"Tools/Event.hpp"
#include<cstdint>
#include<bitset>

class CMouse {

public:
    enum class EPressableKeys :uint8_t {
        Left, Right, Middle,

        ENUMEND
    };
private:

    std::bitset<(size_t)EPressableKeys::ENUMEND> PressableKeysStates;

    //parameters: bool if it was pressed down or up:true=down,false=up
    CEvent<bool> PressableKeysEvents[(unsigned short int)EPressableKeys::ENUMEND];

public:

    KL2_API CEvent<bool>& gMousePressableKeyEvent(EPressableKeys key);

    //not for user
    void _GLFW_KEYCALLBACK(int button, int action, int mods);


    KL2_API bool gMousePressableKeyState(EPressableKeys key) const;
};
