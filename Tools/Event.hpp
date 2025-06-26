#pragma once
#include <functional>
#include <vector>

template <typename... Ts>
class CEvent {
  private:
    class CSubscription {
        friend CEvent;

      private:
        std::reference_wrapper<CEvent> Event;
        size_t UID = 0;

      public:
        CSubscription(CEvent& event, size_t UID);
        CSubscription(CSubscription const&) = delete;
        CSubscription(CSubscription&&) = default;
        CSubscription& operator=(CSubscription const&) = delete;
        CSubscription& operator=(CSubscription&&) = default;
        ~CSubscription();
    };
    struct SConnection {
        std::function<void(Ts...)> Func;
        size_t Priority = 0;
        size_t UID = 0;
    };
    std::vector<SConnection> Connections;

    size_t UID_Counter = 0;
    void _UpdateUID_Counter();

  public:
    void FireEvent(Ts... vals) const;
    [[nodiscard]] CSubscription AddConnection(size_t priority,
                                              std::function<void(Ts...)> func);
    void RemoveConnection(CSubscription& subscription);

    CEvent() = default;
    CEvent(CEvent const&) = delete;
    CEvent(CEvent&&) = default;
    CEvent& operator=(CEvent const&) = delete;
    CEvent& operator=(CEvent&&) = default;
    ~CEvent() = default;
};

#include <algorithm>
#include <limits>
#include <stdexcept>

template <typename... Ts>
CEvent<Ts...>::CSubscription::CSubscription(CEvent& event, size_t _UID)
    : Event(event), UID(_UID) {}

template <typename... Ts>
CEvent<Ts...>::CSubscription::~CSubscription() {
    if (UID) Event.get().RemoveConnection(*this);
}

template <typename... Ts>
void CEvent<Ts...>::FireEvent(Ts... vals) const {
    auto consCopy = Connections;
    for (auto const& con : consCopy) con.Func(std::forward<Ts>(vals)...);
}

template <typename... Ts>
void CEvent<Ts...>::_UpdateUID_Counter() {
    if (UID_Counter == std::numeric_limits<size_t>::max())
        UID_Counter = 1;
    else
        UID_Counter++;
}

template <typename... Ts>
auto CEvent<Ts...>::AddConnection(size_t priority,
                                  std::function<void(Ts...)> func)
    -> CSubscription {
    auto whereToInsert =
        std::upper_bound(Connections.begin(), Connections.end(), priority,
                         [](size_t priority, const SConnection& con) {
                             return con.Priority > priority;
                         });
    _UpdateUID_Counter();  // UID of zero is invalid, thats why this call is
                           // here
    Connections.insert(whereToInsert, {func, priority, UID_Counter});
    return {*this, UID_Counter};
}

#define STR(x) _STR(x)
#define _STR(x) #x

template <typename... Ts>
void CEvent<Ts...>::RemoveConnection(CSubscription& subscription) {
    auto con = std::find_if(
        Connections.begin(), Connections.end(),
        [&](SConnection const& con) { return con.UID == subscription.UID; });
    if (con == Connections.end())
        throw std::invalid_argument(__FILE__
                                    ":" STR(__LINE__) " invalid subscription");
    else {
        subscription.UID = 0;
        Connections.erase(con);
    }
}

#undef STR
#undef _STR
