#pragma once

class ConnectionSlotC {
protected:
	ConnectionSlotC* OtherSlot = nullptr;
public:
    ConnectionSlotC() noexcept {}
    ConnectionSlotC(ConnectionSlotC&& toMove) noexcept:
        OtherSlot(toMove.OtherSlot) {
        toMove.OtherSlot = nullptr;
        if (OtherSlot != nullptr) OtherSlot->OtherSlot = this;
    }
	virtual ConnectionSlotC& operator=(ConnectionSlotC&& toMove) noexcept {
		Disconnect();
		return *new(this) ConnectionSlotC(std::move(toMove));
	}
    virtual ~ConnectionSlotC() {
		Disconnect();
    }
    virtual void Connect(ConnectionSlotC& otherCon) noexcept {
        if (OtherSlot != nullptr) Disconnect();
        if (otherCon.OtherSlot != nullptr) Disconnect();
        OtherSlot = &otherCon;
        otherCon.OtherSlot = this;
    }
    virtual void Disconnect() noexcept { 
		if (OtherSlot != nullptr) {
            OtherSlot->OtherSlot = nullptr;
            OtherSlot->Disconnect();
            OtherSlot = nullptr;
        }
	}
	bool gIsConnected() const noexcept { return OtherSlot != nullptr; }
};