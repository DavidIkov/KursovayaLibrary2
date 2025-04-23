#include"Event.hpp"
#include"iostream"

void CEvent::FireEvent(void* data) {
    if (NeedBufferSwap) {
        ConnectionsFrontBuffer.reserve(0);
        ConnectionsFrontBuffer.reserve(ConnectionsBackBuffer.size());
        for (unsigned int i = 0; i < ConnectionsBackBuffer.size(); i++) {
            ConnectionsFrontBuffer.emplace_back(ConnectionsBackBuffer[i].Func, ConnectionsBackBuffer[i].CheckFunc);
            ConnectionsBackBuffer[i].IndInFrontBuffer = i + 1;
        }
        NeedBufferSwap = false;
    }
    EventIsFiring = true;
    for (unsigned int i = 0; i < ConnectionsFrontBuffer.size(); i++)
        ConnectionsFrontBuffer[i].CheckFuncPassed = (not ConnectionsFrontBuffer[i].Deleted) and ConnectionsFrontBuffer[i].CheckFunc(data);
    for (unsigned int i = 0; i < ConnectionsFrontBuffer.size(); i++)
        if (ConnectionsFrontBuffer[i].CheckFuncPassed) ConnectionsFrontBuffer[i].Func(data);

    
    EventIsFiring = false;
}

CEvent::~CEvent() {
    for (unsigned int i = 0; i < ConnectionsBackBuffer.size(); i++) {
        auto& cons = ConnectionsBackBuffer[i].EventConnectionsHandler->Connections;
        for (unsigned int ci = ConnectionsBackBuffer[i].ConnectionInd + 1; ci < cons.size(); ci++) {
            cons[ci].Event->ConnectionsBackBuffer[cons[ci].ConnectionInd].ConnectionInd--;
        }
        cons.erase(cons.begin() + ConnectionsBackBuffer[i].ConnectionInd);
    }
}

CEvent::CConnectionDataInFrontBuffer::CConnectionDataInFrontBuffer(std::function<void(void*)>& func, std::function<bool(void*)>& checkFunc) :Func(func), CheckFunc(checkFunc) { }

CEvent::CConnectionDataInBackBuffer::CConnectionDataInBackBuffer(std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc, float priority, const CEventConnectionsHandler* handler, unsigned int conInd) :
    Func(std::move(func)), CheckFunc(std::move(checkFunc)), Priority(priority), EventConnectionsHandler((CEventConnectionsHandler*)handler), ConnectionInd(conInd) { }





CEventConnectionsHandler::CConnectionData::CConnectionData(ConnectionID id, const CEvent* ev, unsigned int conInd) :
    ID(id), Event(ev), ConnectionInd(conInd) {}

CEventConnectionsHandler::ConnectionID CEventConnectionsHandler::_ConnectToEvent
(const CEvent* ev, float priority, unsigned int priorityInsertInd, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const {
    
    Connections.emplace_back((ConnectionID)IDCounter, ev, priorityInsertInd);

    ev->ConnectionsBackBuffer.emplace(ev->ConnectionsBackBuffer.begin() + priorityInsertInd, std::move(func), std::move(checkFunc),
        priority, this, (unsigned int)Connections.size() - 1);

    ev->NeedBufferSwap = true;
    return IDCounter++;
}

CEventConnectionsHandler::ConnectionID CEventConnectionsHandler::ConnectToEvent(const CEvent* ev, std::function<void(void*)>&& func) const {
    return ConnectToEvent(ev, std::move(func), [](void*)->bool {return true; });
}
CEventConnectionsHandler::ConnectionID CEventConnectionsHandler::ConnectToEvent(const CEvent* ev, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const {
    return _ConnectToEvent(ev, (ev->ConnectionsBackBuffer.size() == 0) ? 0 : (ev->ConnectionsBackBuffer[ev->ConnectionsBackBuffer.size() - 1].Priority + 1),
        (unsigned int)ev->ConnectionsBackBuffer.size(), std::move(func), std::move(checkFunc));
}
CEventConnectionsHandler::ConnectionID CEventConnectionsHandler::ConnectToEvent(const CEvent* ev, float priority, std::function<void(void*)>&& func) const {
    return ConnectToEvent(ev, priority, std::move(func), [](void*)->bool {return true; });
}
CEventConnectionsHandler::ConnectionID CEventConnectionsHandler::ConnectToEvent(const CEvent* ev, float priority, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const {

    unsigned int ii = (unsigned int)ev->ConnectionsBackBuffer.size();
    for (unsigned int i = 0; i < ii; i++) if (ev->ConnectionsBackBuffer[i].Priority > priority) { ii = i; break; }

    return _ConnectToEvent(ev, priority, ii, std::move(func), std::move(checkFunc));
}
void CEventConnectionsHandler::RemoveConnection(const ConnectionID id) {
    for (unsigned int i = 0; i < Connections.size(); i++) {
        if (Connections[i].ID == id) {
            auto& econs = Connections[i].Event->ConnectionsBackBuffer;
            for (unsigned int ci = Connections[i].ConnectionInd + 1; ci < econs.size(); ci++)
                econs[ci].EventConnectionsHandler->Connections[econs[ci].ConnectionInd].ConnectionInd--;
            for (unsigned int ci = i + 1; ci < Connections.size(); ci++)
                Connections[ci].Event->ConnectionsBackBuffer[Connections[ci].ConnectionInd].ConnectionInd--;
            
            if (econs[Connections[i].ConnectionInd].IndInFrontBuffer != 0)
                Connections[i].Event->ConnectionsFrontBuffer[econs[Connections[i].ConnectionInd].IndInFrontBuffer - 1].Deleted = true;

            econs.erase(econs.begin() + Connections[i].ConnectionInd);
            Connections.erase(Connections.begin() + i);

            Connections[i].Event->ConnectionsBackBuffer = Connections[i].Event->ConnectionsBackBuffer;
            Connections[i].Event->NeedBufferSwap = true;

            return;
        }
    }

    KL2::ErrorsSystem::SendError << "attemping to remove connection with invalid id(it can be already deleted or just invalid)" >> SErrorsEnumWrapper(ErrorsEnum::IncorrectID);
}

CEventConnectionsHandler::~CEventConnectionsHandler() {
    for (unsigned int i = 0; i < Connections.size(); i++) {

        auto& econs = Connections[i].Event->ConnectionsBackBuffer;
        
        for (unsigned int ci = Connections[i].ConnectionInd + 1; ci < econs.size(); ci++)
            econs[ci].EventConnectionsHandler->Connections[econs[ci].ConnectionInd].ConnectionInd--;

        if (econs[Connections[i].ConnectionInd].IndInFrontBuffer != 0)
            Connections[i].Event->ConnectionsFrontBuffer[econs[Connections[i].ConnectionInd].IndInFrontBuffer - 1].Deleted = true;

        Connections[i].Event->NeedBufferSwap = true;
        econs.erase(econs.begin() + Connections[i].ConnectionInd); 

    }
}