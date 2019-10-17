#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <bullet3/btBulletCollisionCommon.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using Vec2_t = glm::vec2;
using Vec3_t = glm::vec3;
using Vec4_t = glm::vec4;

using Mat3_t = glm::mat3;
using Mat4_t = glm::mat4;

using Quat_t = glm::quat;

using int_t  = GLint;
using uint_t = GLuint;


// for collisions:

using CVec3_t = btVector3;

using CMat3_t = btMatrix3x3;

using CQuat_t = btQuaternion;

using CTransform_t = btTransform;


inline Vec3_t toVec3(CVec3_t const& v)
{
    return Vec3_t(v.x(), v.y(), v.z());
}

inline CVec3_t toCVec3(Vec3_t const& v)
{
    return CVec3_t(v.x, v.y, v.z);
}


inline Quat_t toQuat(CQuat_t const& q)
{
    return Quat_t(q.w(), q.x(), q.y(), q.z());
}

inline CQuat_t toCQuat(Quat_t const& q)
{
    return CQuat_t(q.x, q.y, q.z, q.w);
}


#define ALLOW_CONSTR_COPY_MOVE(className)  \
    className(className const&) = default; \
    className(className&&)      = default;

#define ALLOW_COPY(className)              \
    className(className const&) = default; \
    className& operator=(className const&) = default;

#define BAN_COPY(className)               \
    className(className const&) = delete; \
    className& operator=(className const&) = delete;

#define ALLOW_MOVE(className)         \
    className(className&&) = default; \
    className& operator=(className&&) = default;

#define BAN_MOVE(className)          \
    className(className&&) = delete; \
    className& operator=(className&&) = delete;

#define BAN_ASSIGN(className)                        \
    className& operator=(className const&) = delete; \
    className& operator=(className&&) = delete;


#define RULEOF4_ALLOW_ALL(className) \
    ALLOW_COPY(className)            \
    ALLOW_MOVE(className)

#define RULEOF4_BAN_ALL(className) \
    BAN_COPY(className)            \
    BAN_MOVE(className)

#define RULEOF4_BAN_COPY(className) \
    BAN_COPY(className)             \
    ALLOW_MOVE(className)

#define RULEOF4_BAN_ASSIGN(className) \
    ALLOW_CONSTR_COPY_MOVE(className) \
    BAN_ASSIGN(className)

#define RULEOF5_ALLOW_ALL(className) \
    className() = default;           \
    RULEOF4_ALLOW_ALL(className)

#define RULEOF5_BAN_ALL(className) \
    className() = delete;          \
    RULEOF4_BAN_ALL(className)

#define RULEOF5_BAN_COPY(className) \
    className() = default;          \
    RULEOF4_BAN_COPY(className)

#define RULEOF5_BAN_ASSIGN(className) \
    className() = default;            \
    RULEOF4_BAN_ASSIGN(className)

#endif // TYPEDEFS_H