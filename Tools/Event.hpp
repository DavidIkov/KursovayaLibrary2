#pragma once
#include"KL2_API.h"
#include"functional"
#include<vector>
#include"ErrorsSystem.hpp"

class CEventConnectionsHandler;

class CEvent{
    friend class CEventConnectionsHandler;

    mutable bool NeedBufferSwap = false;

    bool EventIsFiring = false;

    struct CConnectionDataInFrontBuffer{
        std::function<void(void*)> Func;
        std::function<bool(void*)> CheckFunc;
        bool CheckFuncPassed;

        bool Deleted = false;

        CConnectionDataInFrontBuffer(std::function<void(void*)>& func, std::function<bool(void*)>& checkFunc);
    };
    mutable std::vector<CConnectionDataInFrontBuffer> ConnectionsFrontBuffer;
    struct CConnectionDataInBackBuffer{
        std::function<void(void*)> Func;
        std::function<bool(void*)> CheckFunc;

        float Priority;

        CEventConnectionsHandler* EventConnectionsHandler;
        unsigned int ConnectionInd;

        unsigned int IndInFrontBuffer = 0;//+1 so if its = 0 then its invalid

        CConnectionDataInBackBuffer(std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc, float priority, const CEventConnectionsHandler* handler, unsigned int conInd);
    };
    mutable std::vector<CConnectionDataInBackBuffer> ConnectionsBackBuffer;
public:
    KL2_API void FireEvent(void* data);

    KL2_API ~CEvent();
};

class CEventConnectionsHandler{
    friend class CEvent;

    mutable unsigned int IDCounter = 0;

public:
    typedef unsigned ConnectionID;
private:
    struct CConnectionData {
        ConnectionID ID;

        const CEvent* Event;
        unsigned int ConnectionInd;

        CConnectionData(ConnectionID id, const CEvent* ev, unsigned int conInd);
    };
    mutable std::vector<CConnectionData> Connections;

private: ConnectionID _ConnectToEvent(const CEvent* ev, float priority, unsigned int priorityInsertInd, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const;
public:

    struct SErrorsEnumWrapper :KL2::ErrorsSystem::SErrorBase {
        enum ErrorsEnum {
            IncorrectID,
        };
        ErrorsEnum Error;
        inline SErrorsEnumWrapper(ErrorsEnum error) :Error(error) {};
    }; using ErrorsEnum = SErrorsEnumWrapper; using AnyError = SErrorsEnumWrapper;

    KL2_API ConnectionID ConnectToEvent(const CEvent* ev, std::function<void(void*)>&& func) const;
    KL2_API ConnectionID ConnectToEvent(const CEvent* ev, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const;
    KL2_API ConnectionID ConnectToEvent(const CEvent* ev, float priority, std::function<void(void*)>&& func) const;
    KL2_API ConnectionID ConnectToEvent(const CEvent* ev, float priority, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const;
    KL2_API void RemoveConnection(const ConnectionID id);
    KL2_API ~CEventConnectionsHandler();
};