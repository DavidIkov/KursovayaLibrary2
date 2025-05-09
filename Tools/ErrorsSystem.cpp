#include"ErrorsSystem.hpp"
#include<iostream>

void KL2::ErrorsSystem::_CSendWarning::operator>>(_SEndOfWarning) {
    if (not IgnoreWarnings) {
        std::cerr << "KL2 Warning: {" << (Msg.empty() ? "Empty warning" : Msg) << '}' << std::endl;
        Msg.clear();
    }
}
void KL2::ErrorsSystem::_CSendError::_ProcessError() noexcept {
    std::cerr << "KL2 Error: {" << (Msg.empty() ? "Empty error" : Msg) << "}" << std::endl;
    Msg.clear();
}
