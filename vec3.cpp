#include "vec3.h"

#include <cmath>

Vec3::Type Vec3::dist(const Vec3& other) const {
    return (other - (*this)).magn();
}

Vec3::Type Vec3::magn() const {
    return std::sqrt(x * x + y * y + z * z);
}

void Vec3::magn(Type magnitude) {
    Type phi = std::atan2(y, x);
    Type theta = std::acos(z / magn());

    x = magnitude * std::sin(theta) * cos(phi);
    y = magnitude * std::sin(theta) * sin(phi);
    z = magnitude * std::cos(theta);
}

Vec3& Vec3::norm() {
    magn(1);
    return *this;
}

Vec3::Type Vec3::dot(const Vec3& other) {
    return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3& other) const {
    return Vec3{
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

Vec3 Vec3::operator+(const Vec3& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vec3& Vec3::operator+=(const Vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Vec3& Vec3::operator-=(const Vec3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

Vec3& Vec3::operator*=(Type scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}

Vec3 Vec3::operator*(Type scale) const {
    Vec3 ret = *this;
    ret.x *= scale;
    ret.y *= scale;
    ret.z *= scale;
    return ret;
}

Vec3 Vec3::operator/(Type scale) const {
    Vec3 ret = *this;
    ret.x /= scale;
    ret.y /= scale;
    ret.z /= scale;
    return ret;
}
