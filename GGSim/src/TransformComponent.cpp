#include <GGSim/TransformComponent.h>


Transform& operator*=(Transform& lhs, Transform const& rhs)
{
    lhs.pos = lhs.pos + lhs.rot * rhs.pos;
    lhs.rot = lhs.rot * rhs.rot;
    lhs.sca *= rhs.sca;
    return lhs;
}


Mat4_t Transform::asMatrix() const
{
    Mat4_t result = Mat4_t(rot) * glm::scale(Mat4_t(1.f), sca);
    result[3]     = Vec4_t(pos, 1);
    return result;
}

Transform Transform::inversed() const
{
    Mat4_t invRot = glm::transpose(rot);
    return { invRot, invRot * Vec4_t(-pos, 1), 1.f / sca };
}

Transform& Transform::rotate(float_t angle, Vec3_t const& axis)
{
    Quat_t quat = glm::angleAxis(angle, axis);
    for (int i = 0; i < 3; ++i)
        rot[i] = rot[i] * quat;
    return *this;
}


void Transformable::move(Vec3_t const& vec)
{
    translate(transf.rot * vec);
}

void Transformable::translate(Vec3_t const& vec)
{
    setPosition(transf.pos + vec);
}

void Transformable::setPosition(Vec3_t const& pos)
{
    transf.pos = pos;
}

void Transformable::rotate(float_t angle, Vec3_t const& axis)
{
    transf.rotate(angle, axis);
}

void Transformable::scale(Vec3_t const& scale)
{
    transf.sca.x *= scale.x;
    transf.sca.y *= scale.y;
    transf.sca.z *= scale.z;
}

void Transformable::setScale(Vec3_t const& scale)
{
    transf.sca = scale;
}


bool operator==(Transform const& lhs, Transform const& rhs)
{
    return lhs.pos == rhs.pos && lhs.sca == rhs.sca && lhs.rot == rhs.rot;
}