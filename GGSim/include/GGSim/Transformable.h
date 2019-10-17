#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include <GGSim/Typedefs.h>


struct Transform
{
    Mat4_t matrix() const;

    Mat4_t inversedMatrix() const;

    Vec3_t forwardVec() const;

    Vec3_t upVec() const;

    Vec3_t rightVec() const;


    Quat_t orient = Quat_t(1.f, 0.f, 0.f, 0.f);

    Vec3_t pos = Vec3_t(0.f);

    Vec3_t sca = Vec3_t(1.f, 1.f, 1.f);
};


inline CTransform_t toCTransform(Transform const& t)
{
    return CTransform_t(toCQuat(t.orient), toCVec3(t.pos));
}

inline CTransform_t& toCTransform(Transform const& t, CTransform_t& ct)
{
    return ct = toCTransform(t);
}


struct Transformable : public Transform
{
    virtual ~Transformable() = default;

    void moveLocal(Vec3_t const& vec);

    void rotateLocal(float angle, Vec3_t const& axis);
    void rotateGlobal(float angle, Vec3_t const& axis);

    void scale(Vec3_t const& scale);
};


bool operator==(Transform const& lhs, Transform const& rhs);

#endif // TRANSFORMABLE_H