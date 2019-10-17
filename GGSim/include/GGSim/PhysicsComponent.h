#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <ECSpp/Component.h>
#include <GGSim/Typedefs.h>

struct PhysicsComponent : public epp::Component
{
    // arguments in world space
    PhysicsComponent& applyForce(Vec3_t const& f, Vec3_t const& relativePos = Vec3_t(0, 0, 0));

    void resetForNextTick();

    void setMass(float mass);
    void setInertiaTensor(Mat3_t const& tensor);

    inline float  mass() const { return m; }
    inline float  massInversed() const { return mInv; }
    inline Mat3_t tensor() const { return iTensor * m; }
    inline Mat3_t tensorInversed() const { return iTensorInv * 1.f / m; }

    // argument in world space (to match the space that the angular velocity is described in)
    Vec3_t velocityAt(Vec3_t const& relativePos) const;


    Vec3_t force = Vec3_t(0, 0, 0);
    Vec3_t acc   = Vec3_t(0, 0, 0);
    Vec3_t vel   = Vec3_t(0, 0, 0);

    Vec3_t torque     = Vec3_t(0, 0, 0);
    Vec3_t angularAcc = Vec3_t(0, 0, 0);
    Vec3_t angularVel = Vec3_t(0, 0, 0);

    float restitution = 0.3f;

    float stFrCo = 0.4f;
    float dyFrCo = 0.3f;

private:
    float  m          = 10.f;
    float  mInv       = 1.f / 10.f;
    Mat3_t iTensor    = Mat3_t(0);
    Mat3_t iTensorInv = Mat3_t(0);
};

#endif // PHYSICSCOMPONENT_H