#pragma once

#include "vec3.h"
#include <SFML/System/Vector2.hpp>

class Camera {
public:
    Camera() = default;
    Camera(const Vec3& pos_,
            const Vec3& lookAt,
            const Vec3& up,
            const Vec3& boxMin_,
            const Vec3& boxMax_);
    
    Vec3 project(const Vec3& targetPos) const;

    void setOrientation(const Vec3& lookAt, const Vec3& up);

    Vec3 pos;
private:
    Vec3 boxMin;
    Vec3 boxMax;
    Vec3 right;
    Vec3 up;
    Vec3 forward;
};
