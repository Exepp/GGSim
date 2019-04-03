#include <GGSim/Vec.h>

class Quaternion
{
    Vec4f quat;
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

private:
    Mat33f rot;
};

class Transformable
{
};