#include <GGSim/Vec.h>

class Quaternion
{

public:
    Quaternion() = default;

    explicit Quaternion(Vec3f const& axis, Float_t angle)
        : axis((Float_t)sin(angle / 2) * axis.normalized())
        , scalar(cos(angle / 2))
    {}

private:
    explicit Quaternion(Float_t scalar, Vec3f const& axis)
        : axis(axis)
        , scalar(scalar)
    {}


public:
    Vec3f& transform(Vec3f& vec) const
    {
        vec = (*this * Quaternion(0, vec) * !*this).axis;
        return vec;
    }


    Quaternion operator!() const
    {
        return Quaternion(scalar, -axis);
    }

    Quaternion operator*(Quaternion const& b) const
    {
        return Quaternion(scalar * b.scalar - axis.x() * b.axis.x() - axis.y() * b.axis.y() - axis.z() * b.axis.z(),
                          Vec3f({ scalar * b.axis.x() + axis.x() * b.scalar + axis.y() * b.axis.z() - axis.z() * b.axis.y(),
                                  scalar * b.axis.y() + axis.y() * b.scalar + axis.z() * b.axis.x() - axis.x() * b.axis.z(),
                                  scalar * b.axis.z() + axis.z() * b.scalar + axis.x() * b.axis.y() - axis.y() * b.axis.x() }));
    }

private:
    Float_t scalar;
    Vec3f   axis;
};

class Rotation
{
public:
    Mat44f asTransform() const
    {
        return Mat44f(rot);
    }

    Vec3f angles()
    {
        Vec3f result;

        return result;
    }

    template<class T>
    static T& rotateAround(Vec3f const& axis, T& obj)
    {
    }

private:
    Mat33f rot;
};

class Transformable
{
};