#pragma once
#include"KL2_API.h"
#include"Tools/ErrorsSystem.hpp"
#include<ostream>

namespace KL2::Manager {
    struct SErrorsEnumWrapper : public ErrorsSystem::SErrorBase {
        enum EErrors {
            FailedToInitializeGLFW,
            VoidPtrSizesInKL2_AndProjectDontMatch,
        } Error;
        inline SErrorsEnumWrapper(EErrors error) :Error(error) {};
    }; using ErrorsEnum = SErrorsEnumWrapper; using AnyError = SErrorsEnumWrapper;

    class CKL2 {

        KL2_API static size_t _GetVoidPtrSizeInKL2() noexcept;

        KL2_API static void _InitializeKL2(unsigned openglVersionMajor, unsigned openglVersionMinor);
        KL2_API static void _UninitializeKL2();
        
    public:
        CKL2(CKL2 const&) = delete;
        CKL2& operator=(CKL2 const&) = delete;
        CKL2(CKL2 &&) = delete;
        CKL2& operator=(CKL2 &&) = delete;
        CKL2(unsigned openglVersionMajor = 3, unsigned openglVersionMinor = 3) {

            if (_GetVoidPtrSizeInKL2() != sizeof(void*))
                ErrorsSystem::SendError << "in KL2 sizeof(void*) is " << std::to_string(_GetVoidPtrSizeInKL2())
                << " but in your project it is " << std::to_string(sizeof(void*))
                >> SErrorsEnumWrapper(ErrorsEnum::VoidPtrSizesInKL2_AndProjectDontMatch);

            _InitializeKL2(openglVersionMajor, openglVersionMinor);

        }
        ~CKL2() {
            _UninitializeKL2();
        }
    };
}
