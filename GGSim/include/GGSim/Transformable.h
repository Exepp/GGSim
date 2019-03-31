#include <GGSim/Vec.h>

class Quaternion
{
    Vec4f quat;
}


class Rotation
{
public:
    Mat44f asTransform() const
    {
        return Mat44f(rot);
    }


private:
    Mat33f rot;
};

class Transformable
{
};