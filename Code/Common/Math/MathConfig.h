/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MATH_CONFIG_H
#define MATH_CONFIG_H

// GLM message system
// #define GLM_FORCE_MESSAGES
#define GLM_FORCE_INLINE 


// GLM math
//#include <Common/Math/MathConfig.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI ((float)  3.141592654f)

namespace env
{
    typedef glm::vec2 Vec2;
    typedef glm::vec3 Vec3;
    typedef glm::vec4 Vec4;
    typedef glm::mat4 Matrix4x4;
    typedef glm::mat3 Matrix3x3;
    typedef glm::quat Quaternion;

    namespace math
    {

        inline Matrix4x4 translation(const Vec3& p)
        {
            Matrix4x4 m(1.0f);
            m[0][3] = p.x;
            m[1][3] = p.y;
            m[2][3] = p.z;
            return m;
        }

        inline Matrix4x4 scaling(const Vec3& s)
        {
            Matrix4x4 m(1.0f);
            m[0][0] = s.x;
            m[1][1] = s.y;
            m[2][2] = s.z;
            return m;
        }

    }

#ifdef ENV_PLATFORM_WINDOWS
    typedef __int64   envS64;
#else
    typedef __int64_t envS64;
#endif // ENV_PLATFORM_WINDOWS
}

#endif // MATH_CONFIG_H