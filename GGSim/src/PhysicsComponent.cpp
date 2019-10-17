#include <GGSim/PhysicsComponent.h>

PhysicsComponent& PhysicsComponent::applyForce(Vec3_t const& f, Vec3_t const& relativePos)
{
    force += f;
    torque += glm::cross(relativePos, force);
    return *this;
}

void PhysicsComponent::resetForNextTick()
{
    static Vec3_t const zeroV(0, 0, 0);
    force      = zeroV;
    acc        = zeroV;
    torque     = zeroV;
    angularAcc = zeroV;
}

void PhysicsComponent::setMass(float mass)
{
    m    = mass;
    mInv = mass != 0.f ? 1.f / mass : 0.f;
}

void PhysicsComponent::setInertiaTensor(Mat3_t const& tensor)
{
    iTensor    = tensor;
    iTensorInv = glm::inverse(tensor);
}

Vec3_t PhysicsComponent::velocityAt(Vec3_t const& relativePos) const
{
    return vel + glm::cross(angularVel, relativePos);
}