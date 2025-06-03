#pragma once
#include"KL2_API.h"
#include<string>
namespace KL2::ErrorsSystem {

    inline bool IgnoreWarnings = false;

    inline std::string LastErrorMessage;
    inline std::string LastWarningMessage;

    class _CMessageCreatingHelper {
    protected:
        std::string Msg;
    public:
        KL2_API inline _CMessageCreatingHelper& operator<<(const char* msg) { Msg += msg; return *this; }
        KL2_API _CMessageCreatingHelper& operator<<(const std::string& msg) { operator<<(msg.c_str()); return *this; }
    };

    struct _SEndOfWarning {} inline EndOfWarning;
    class _CSendWarning :public _CMessageCreatingHelper {
    public:
        template<typename MsgT>
        _CSendWarning& operator<<(MsgT&& msg) { _CMessageCreatingHelper::operator<<(msg); return *this; }

        KL2_API void operator>>(_SEndOfWarning);
    } inline SendWarning;


    //base class for all KL2 errors
    struct SErrorBase :public std::exception {
    private:
        friend class _CSendError;
        std::string Msg;
    public:
        virtual char const* what() const noexcept override { return Msg.empty() ? "Empty message" : Msg.c_str(); }
    };
    typedef SErrorBase AnyError;
    class _CSendError :public _CMessageCreatingHelper {
        KL2_API void _ProcessError() noexcept;
    public:
        template<typename MsgT>
        _CSendError& operator<<(MsgT&& msg) { _CMessageCreatingHelper::operator<<(msg); return *this; }
        template<typename ErrWrapperTyp>
        void operator>>(ErrWrapperTyp errWrapper) {
            static_assert(std::is_base_of_v<SErrorBase, ErrWrapperTyp>, "error struct should be publicly derived from ErrorBase");
            errWrapper.Msg = Msg;
            _ProcessError();
            throw errWrapper;
        }
    };
    class _CRethrowError {
    public:
        template<typename ErrWrapperTyp>
        void operator<<(ErrWrapperTyp errWrapper) {
            static_assert(std::is_base_of_v<SErrorBase, ErrWrapperTyp>, "error struct should be publicly derived from ErrorBase");
            throw errWrapper;
        }
    };
    inline _CSendError SendError;
    inline _CRethrowError RethrowError;


}