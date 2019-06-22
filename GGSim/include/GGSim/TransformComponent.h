#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "Typedefs.h"

#include <ECSpp/Component.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


struct Transform
{
    Mat4_t asMatrix() const;

    Transform inversed() const;

    Transform& rotate(float_t angle, Vec3_t const& axis);


    Mat3_t rot = Mat3_t(1.f);
    Vec3_t pos = Vec3_t(0.f);
    Vec3_t sca = Vec3_t(1.f, 1.f, 1.f);
};

Transform& operator*=(Transform& lhs, Transform const& rhs);


class Transformable
{

public:
    Transform const& getTransform() const { return transf; }

    void move(Vec3_t const& vec);

    void translate(Vec3_t const& vec);

    void setPosition(Vec3_t const& pos);

    void rotate(float_t angle, Vec3_t const& axis);

    void scale(Vec3_t const& scale);

    void setScale(Vec3_t const& scale);

private:
    Transform transf;
};


bool operator==(Transform const& lhs, Transform const& rhs);


struct TransformComponent : public Transformable, public epp::Component
{
};

#endif // TRANSFORMABLE_H