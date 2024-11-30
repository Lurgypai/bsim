#pragma once

struct Vec3 {
    using Type = double;

    Type x;
    Type y;
    Type z;

    Type dist(const Vec3& other) const;
    Type magn() const;
    void magn(Type magnitude);

    Vec3& norm();
    Type dot(const Vec3& other);
    Vec3 cross(const Vec3& other) const;

    Vec3 operator+(const Vec3& other) const;
    Vec3& operator+=(const Vec3& other);
    Vec3 operator-(const Vec3& other) const;
    Vec3& operator -=(const Vec3& other);

    Vec3& operator*=(Type scale);
    Vec3 operator*(Type scale) const;

    Vec3 operator/(Type scale) const;
};
