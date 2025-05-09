#pragma once
#include<vector>
#include<array>
#include<cstdint>

//you can just use span at this point, if you are at c++20+.
//this class have a weak const features, like its 
//very easy to loose constness of a pointer.

template<typename T>
class CArrayView {
private:
    T* Data; size_t Len;
public:
    constexpr CArrayView(T* data, size_t len) noexcept:Data(data), Len(len) {};

    CArrayView(const CArrayView&) = default;
    CArrayView& operator=(const CArrayView&) = default;
    constexpr CArrayView(CArrayView&&) noexcept = delete;
    constexpr CArrayView& operator=(CArrayView&&) noexcept = delete;

    CArrayView(std::vector<T> const& vec) noexcept
        :Data((T*)&*vec.begin()), Len(vec.size()) {}
    
    template<size_t L>
    constexpr CArrayView(std::array<T, L> const& arr) noexcept
        :Data((T*)&*arr.begin()), Len(L) {}

    constexpr CArrayView(std::initializer_list<T> const& list) noexcept :
        Data((T*)list.begin()), Len(list.size()) {}

    T& operator[](size_t i) noexcept { return Data[i]; }
    T const& operator[](size_t i) const noexcept { return Data[i]; }

    size_t GetLen() const noexcept { return Len; }
    const T* GetData() const noexcept { return Data; }
    T* GetData() noexcept { return Data; }

    constexpr operator CArrayView<uint8_t>() const noexcept { return CArrayView<const uint8_t>((uint8_t const*)Data, sizeof(T) * Len);}
    constexpr operator CArrayView<uint8_t>() noexcept { return CArrayView<uint8_t>((uint8_t*)Data, sizeof(T) * Len);}

};