#include "camera.h"
#include <SFML/System/Vector2.hpp>


Camera::Camera(const Vec3& pos_,
            const Vec3& lookAt,
            const Vec3& up,
            const Vec3& boxMin_,
            const Vec3& boxMax_) :
    pos{pos_},
    boxMin{boxMin_},
    boxMax{boxMax_}
{ setOrientation(lookAt, up); }

Vec3 Camera::project(const Vec3& targetPos) const {
    Vec3 translated = targetPos - pos;
    Vec3 aligned = {
        translated.dot(right),
        -translated.dot(up),
        translated.dot(forward)
    };
    return aligned;
}

void Camera::setOrientation(const Vec3& lookAt, const Vec3& up_) {
    forward = (lookAt-pos).norm();
    right = up_.cross(forward).norm();
    up = forward.cross(right);
}
