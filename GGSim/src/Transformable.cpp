#include <GGSim/Transformable.h>


Mat4_t Transform::matrix() const
{
    Mat4_t result = glm::scale(glm::toMat4(orient), sca);
    result[3]     = Vec4_t(pos, 1);
    return result;
}

Mat4_t Transform::inversedMatrix() const
{
    Mat4_t result = glm::translate(glm::toMat4(glm::inverse(orient)), -pos);
    return result;
}

Vec3_t Transform::forwardVec() const
{
    return orient * Vec3_t(0, 0, 1);
}
Vec3_t Transform::upVec() const
{
    return orient * Vec3_t(0, 1, 0);
}
Vec3_t Transform::rightVec() const
{
    return orient * Vec3_t(1, 0, 0);
}


void Transformable::moveLocal(Vec3_t const& vec)
{
    pos += orient * vec;
}

void Transformable::rotateLocal(float angle, Vec3_t const& axis)
{
    if (angle)
        orient = orient * glm::angleAxis(angle, glm::normalize(axis));
}

void Transformable::rotateGlobal(float angle, Vec3_t const& axis)
{
    if (angle)
        orient = glm::angleAxis(angle, glm::normalize(axis)) * orient;
}

void Transformable::scale(Vec3_t const& scale)
{
    sca.x *= scale.x;
    sca.y *= scale.y;
    sca.z *= scale.z;
}

//
//////////////////////////////// Operators ////////////////////////////////
//


bool operator==(Transform const& lhs, Transform const& rhs)
{
    return lhs.pos == rhs.pos && lhs.sca == rhs.sca && lhs.orient == rhs.orient;
}