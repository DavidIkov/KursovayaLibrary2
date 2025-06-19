#pragma once
#include"ConnectionSlot.h"
#include<vector>
template<typename VectorT>
class BindingSlotC : public ConnectionSlotC {
protected:
	std::vector<VectorT>* Holder;
public:
	using SlotT = BindingSlotC<VectorT>;
	BindingSlotC(std::vector<VectorT>* holder) noexcept :Holder(holder) {
		static_assert(std::is_base_of_v<BindingSlotC<VectorT>, VectorT>, "type of vector should be derived from BindingSlotC<VectorT>");
	};
	BindingSlotC(SlotT&& toMove) noexcept :ConnectionSlotC(std::move(toMove)), Holder(toMove.Holder) { toMove.Holder = nullptr; }
	virtual ConnectionSlotC& operator=(ConnectionSlotC&& toMove) noexcept override final { return operator=(dynamic_cast<SlotT&&>(toMove)); }
	virtual SlotT& operator=(SlotT&& toMove) noexcept {
		ConnectionSlotC::Disconnect();
		return *new(this) SlotT(std::move(toMove));
	}
	virtual ~BindingSlotC() override {
		Holder = nullptr;
	}
	virtual void Connect(ConnectionSlotC& otherCon) noexcept override {
		ConnectionSlotC::Disconnect();
		ConnectionSlotC::Connect(otherCon);
	}
	virtual void Disconnect() noexcept override {
		if (Holder != nullptr) {
			ConnectionSlotC::Disconnect();
			std::vector<VectorT>* _Holder = Holder;
			Holder = nullptr;
			_Holder->erase(_Holder->begin() + ((VectorT*)this - &*_Holder->begin()));
		}
	}
};
class BindingSlotC_Default :public BindingSlotC<BindingSlotC_Default> { public: using SlotT::BindingSlotC; };
