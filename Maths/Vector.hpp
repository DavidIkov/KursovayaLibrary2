#pragma once
#include"math.h"
#include<utility>

template<size_t AxesAmount, typename Type>
class Vector {

    Type Axes[AxesAmount];

    template<typename...otherNumsTyp>
    constexpr void ConstructFromTemplate(const Type& num, otherNumsTyp&&...otherNums) noexcept {
        Axes[AxesAmount - sizeof...(otherNums) - 1] = num;
        if constexpr (sizeof...(otherNums) != 0) ConstructFromTemplate(std::forward<otherNumsTyp>(otherNums)...);
    }
    template<typename...otherNumsTyp>
    constexpr void ConstructFromTemplate(Type&& num, otherNumsTyp&&...otherNums) noexcept {
        Axes[AxesAmount - sizeof...(otherNums) - 1] = std::move(num);
        if constexpr (sizeof...(otherNums) != 0) ConstructFromTemplate(std::forward<otherNumsTyp>(otherNums)...);
    }
public:

    template<size_t, typename>
    friend class Vector;


    constexpr Vector(const Type& num) noexcept {
        for (size_t i = 0; i < AxesAmount; i++) Axes[i] = num;
    }
    //be aware that this constructor will not call move contructors for Type, it will use copy constructors
    constexpr Vector(Type&& num) noexcept :Vector(num) {};
    constexpr Vector(Type const* numsArrPtr) noexcept {
        for (size_t i = 0; i < AxesAmount; i++)
            Axes[i] = numsArrPtr[i];
    }
    template<typename...NumsTyp>
    constexpr Vector(const Type& num, NumsTyp&&...nums) noexcept {
        static_assert(sizeof...(nums) == (AxesAmount - 1), "Too many or not enough numbers");
        ConstructFromTemplate(num, std::forward<NumsTyp>(nums)...);
    }
    template<typename...NumsTyp>
    constexpr Vector(Type&& num, NumsTyp&&...nums) noexcept {
        static_assert(sizeof...(nums) == (AxesAmount - 1), "Too many or not enough numbers");
        ConstructFromTemplate(std::move(num), std::forward<NumsTyp>(nums)...);
    }
    constexpr Vector() noexcept {}

    ~Vector() = default;

    constexpr Vector(const Vector<AxesAmount, Type>& vecToCopy) noexcept {
        for (size_t i = 0; i < AxesAmount; i++) Axes[i] = vecToCopy[i];
    }
    constexpr Vector(Vector<AxesAmount, Type>&& vecToCopy) noexcept {
        for (size_t i = 0; i < AxesAmount; i++) Axes[i] = std::move(vecToCopy[i]);
    }
    constexpr Vector<AxesAmount, Type>& operator=(const Vector<AxesAmount, Type>& vecToCopy) noexcept {
        this->~Vector();
        new(this) Vector(vecToCopy);
        return *this;
    }
    constexpr Vector<AxesAmount, Type>& operator=(Vector<AxesAmount, Type>&& vecToCopy) noexcept {
        this->~Vector();
        new(this) Vector(std::move(vecToCopy));
        return *this;
    }


    constexpr Type& operator[](const size_t ind) noexcept {
        return Axes[ind];
    }
    constexpr const Type& operator[](const size_t ind) const noexcept {
        return Axes[ind];
    }

    //length but squared, square root didnt happen
    constexpr Type LengthSQ() const noexcept {
        Type sum = Type(); for (size_t i = 0; i < AxesAmount; i++) sum += Axes[i] * Axes[i];
        return sum;
    }
    Type Length() const noexcept { return sqrtf(LengthSQ()); }

    Vector<AxesAmount, Type> Normalize() const noexcept {
        return (*this) / Length();
    }
    //dot product with full length, means that it wasnt divided by length of both vectors
    constexpr Type DotFL(const Vector<AxesAmount, Type>& vec) const noexcept {
        Type sum = Type();
        for (size_t i = 0; i < AxesAmount; i++) sum += vec.Axes[i] * Axes[i];
        return sum;
    }
    Type Dot(const Vector<AxesAmount, Type>& vec) const noexcept {
        return DotFL(vec) / vec.Length() / Length();
    }

    //first value is dist from p1, and second one is from p2
    static Vector<2, Type> GetDistsToStortestDistBetweenVecs(
        const Vector<AxesAmount, Type>& p1, const Vector<AxesAmount, Type>& v1,
        const Vector<AxesAmount, Type>& p2, const Vector<AxesAmount, Type>& v2) noexcept
    {
        Vector<AxesAmount, Type> diff = p1 - p2;
        Type v1l = v1.Length(); Type v2l = v2.Length();

        //variables for system like
        // {ax+by+c=0
        // {dx+ey+f=0
        Type a = v1.DotFL(v2); Type b = -v2l*v2l; Type c = diff.DotFL(v2);
        Type d = v1l*v1l; Type e = -v2.DotFL(v1); Type f = diff.DotFL(v1);

        return Vector<2, Type>((b * f - e * c) / (a * e - d * b), (d * c - f * a) / (a * e - d * b));
    };
    static Type GetShortestDistBetweenVecs(
        const Vector<AxesAmount, Type>& p1, const Vector<AxesAmount, Type>& v1,
        const Vector<AxesAmount, Type>& p2, const Vector<AxesAmount, Type>& v2) noexcept
    {
        Vector<2, Type> dists = GetDistsToStortestDistBetweenVecs(p1, v1, p2, v2);

        Vector<AxesAmount, Type> mp1 = p1 + dists[0]* v1; Vector<AxesAmount, Type> mp2 = p2 + dists[1]* v2; 

        return (mp1 - mp2).Length();
    };

    //cross product of full length, means that it wasnt deivided by length of both vectors
    constexpr Vector<AxesAmount, Type> CrossFL(const Vector<AxesAmount, Type>& vec) const noexcept {
        static_assert(AxesAmount == 3, "Cross is defined only for 3-axes vectors!");
        return Vector<AxesAmount, Type>(Axes[1] * vec.Axes[2] - Axes[2] * vec.Axes[1], Axes[2] * vec.Axes[0] - Axes[0] * vec.Axes[2], Axes[0] * vec.Axes[1] - Axes[1] * vec.Axes[0]);
    }
    Vector<AxesAmount, Type> Cross(const Vector<AxesAmount, Type>& vec) const noexcept {
        return CrossFL(vec) / Length() / vec.Length();
    }


    constexpr Vector<AxesAmount, Type> operator+(const Type& num) const noexcept { Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] + num; return retVec; }
    constexpr Vector<AxesAmount, Type> operator-(const Type& num) const noexcept { Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] - num; return retVec; }
    constexpr Vector<AxesAmount, Type> operator*(const Type& num) const noexcept { Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] * num; return retVec; }
    constexpr Vector<AxesAmount, Type> operator/(const Type& num) const noexcept { Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] / num; return retVec; }

    constexpr Vector<AxesAmount, Type> operator+(const Vector<AxesAmount, Type>& vec) const noexcept { Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] + vec.Axes[i]; return retVec; }
    constexpr Vector<AxesAmount, Type> operator-(const Vector<AxesAmount, Type>& vec) const noexcept { Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] - vec.Axes[i]; return retVec; }
    constexpr Vector<AxesAmount, Type> operator*(const Vector<AxesAmount, Type>& vec) const noexcept { Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] * vec.Axes[i]; return retVec; }
    constexpr Vector<AxesAmount, Type> operator/(const Vector<AxesAmount, Type>& vec) const noexcept { Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec.Axes[i] = Axes[i] / vec.Axes[i]; return retVec; }

    constexpr Vector<AxesAmount, Type> operator-() const noexcept { Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec.Axes[i] = -Axes[i]; return retVec; }

    constexpr Vector<AxesAmount, Type>& operator+=(const Type& num) noexcept { for (size_t i = 0; i < AxesAmount; i++) Axes[i] += num; return *this; }
    constexpr Vector<AxesAmount, Type>& operator-=(const Type& num) noexcept { for (size_t i = 0; i < AxesAmount; i++) Axes[i] -= num; return *this; }
    constexpr Vector<AxesAmount, Type>& operator*=(const Type& num) noexcept { for (size_t i = 0; i < AxesAmount; i++) Axes[i] *= num; return *this; }
    constexpr Vector<AxesAmount, Type>& operator/=(const Type& num) noexcept { for (size_t i = 0; i < AxesAmount; i++) Axes[i] /= num; return *this; }

    constexpr Vector<AxesAmount, Type>& operator+=(const Vector<AxesAmount, Type>& vec) noexcept { for (size_t i = 0; i < AxesAmount; i++) Axes[i] += vec.Axes[i]; return *this; }
    constexpr Vector<AxesAmount, Type>& operator-=(const Vector<AxesAmount, Type>& vec) noexcept { for (size_t i = 0; i < AxesAmount; i++) Axes[i] -= vec.Axes[i]; return *this; }
    constexpr Vector<AxesAmount, Type>& operator*=(const Vector<AxesAmount, Type>& vec) noexcept { for (size_t i = 0; i < AxesAmount; i++) Axes[i] *= vec.Axes[i]; return *this; }
    constexpr Vector<AxesAmount, Type>& operator/=(const Vector<AxesAmount, Type>& vec) noexcept { for (size_t i = 0; i < AxesAmount; i++) Axes[i] /= vec.Axes[i]; return *this; }

    template<typename Type2>
    constexpr operator Vector<AxesAmount, Type2>() const noexcept { Vector<AxesAmount, Type2> vec; for (size_t i = 0; i < AxesAmount; i++) vec.Axes[i] = (Type2)Axes[i]; return vec; }
    
    constexpr bool operator==(const Vector<AxesAmount, Type>& vec) noexcept { for (size_t i = 0; i < AxesAmount; i++) if (Axes[i] != vec.Axes[i]) return false; return true; }
    constexpr bool operator!=(const Vector<AxesAmount, Type>& vec) noexcept { for (size_t i = 0; i < AxesAmount; i++) if (Axes[i] != vec.Axes[i]) return true; return false; }
};


template<size_t AxesAmount, typename Type>
constexpr Vector<AxesAmount, Type> operator+(const Type& num, const Vector<AxesAmount, Type>& vec) noexcept {
    Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec[i] = num + vec[i]; return retVec;
}
template<size_t AxesAmount, typename Type>
constexpr Vector<AxesAmount, Type> operator-(const Type& num, const Vector<AxesAmount, Type>& vec) noexcept {
    Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec[i] = num - vec[i]; return retVec;
}
template<size_t AxesAmount, typename Type>
constexpr Vector<AxesAmount, Type> operator*(const Type& num, const Vector<AxesAmount, Type>& vec) noexcept {
    Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec[i] = num * vec[i]; return retVec;
}
template<size_t AxesAmount, typename Type>
constexpr Vector<AxesAmount, Type> operator/(const Type& num, const Vector<AxesAmount, Type>& vec) noexcept {
    Vector<AxesAmount, Type> retVec; for (size_t i = 0; i < AxesAmount; i++) retVec[i] = num / vec[i]; return retVec;
}


typedef Vector<1, int> Vector1I;
typedef Vector<2, int> Vector2I;
typedef Vector<3, int> Vector3I;
typedef Vector<4, int> Vector4I;

typedef Vector<1, unsigned> Vector1U;
typedef Vector<2, unsigned> Vector2U;
typedef Vector<3, unsigned> Vector3U;
typedef Vector<4, unsigned> Vector4U;

typedef Vector<1, float> Vector1F;
typedef Vector<2, float> Vector2F;
typedef Vector<3, float> Vector3F;
typedef Vector<4, float> Vector4F;

typedef Vector<1, double> Vector1D;
typedef Vector<2, double> Vector2D;
typedef Vector<3, double> Vector3D;
typedef Vector<4, double> Vector4D;

typedef Vector<1, bool> Vector1B;
typedef Vector<2, bool> Vector2B;
typedef Vector<3, bool> Vector3B;
typedef Vector<4, bool> Vector4B;

constexpr inline Vector2F Vec2D_X_F = Vector2F(1.f, 0.f);
constexpr inline Vector2F Vec2D_Y_F = Vector2F(0.f, 1.f);
constexpr inline Vector2D Vec2D_X_D = Vector2D(1., 0.);
constexpr inline Vector2D Vec2D_Y_D = Vector2D(0., 1.);

constexpr inline Vector3F Vec3D_X_F = Vector3F(1.f, 0.f, 0.f);
constexpr inline Vector3F Vec3D_Y_F = Vector3F(0.f, 1.f, 0.f);
constexpr inline Vector3F Vec3D_Z_F = Vector3F(0.f, 0.f, 1.f);

constexpr inline Vector3D Vec3D_X_D = Vector3D(1., 0., 0.);
constexpr inline Vector3D Vec3D_Y_D = Vector3D(0., 1., 0.);
constexpr inline Vector3D Vec3D_Z_D = Vector3D(0., 0., 1.);


