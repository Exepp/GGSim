#include <ECSpp/System.h>
#include <GGSim/Transformable.h>


struct PhysicsComponent : public epp::Component
{
    float mass = 0;

    Mat3_t inertiaTensor = Mat3_t(0);
};

class PhysicsSystem : public epp::System
{
public:
    virtual void init(epp::EntityManager& entityManager) override;

    virtual void update(epp::EntityManager& entityManager, float dt) override;
};