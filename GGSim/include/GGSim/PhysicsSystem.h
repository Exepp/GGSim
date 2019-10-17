#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <ECSpp/System.h>
#include <GGSim/PhysicsComponent.h>
#include <GGSim/ShapeComponent.h>
#include <GGSim/TransformComponent.h>
#include <bullet3/btBulletCollisionCommon.h>


class PhysicsSystem : public epp::System
{
public:
    using EntityGroup_t = epp::CGroup<ShapeComponent, TransformComponent, PhysicsComponent>;

    using CConfig_t     = btDefaultCollisionConfiguration;
    using CDispatcher_t = btCollisionDispatcher;
    using CBroadPhase_t = bt32BitAxisSweep3;
    using CWorld_t      = btCollisionWorld;

public:
    PhysicsSystem();
    RULEOF4_BAN_COPY(PhysicsSystem);

private:
    virtual void init(epp::EntityManager& entityManager) override;

    virtual void preUpdate(epp::EntityManager& entityManager, float dt, bool catchUpTick) override;
    virtual void update(epp::EntityManager& entityManager, float dt, bool catchUpTick) override;
    virtual void postUpdate(epp::EntityManager& entityManager, float dt, bool catchUpTick) override;

    void entityCreationCallback(epp::EntityEvent& event);
    void entityDestructionCallback(epp::EntityEvent& event);

    void applyGravity();
    void resolveCollisions();
    void apply(float dt);
    void drawContactPoints();

public:
    static float const GravitationalAcc;

private:
    EntityGroup_t entities;

    CConfig_t     cfg;
    CDispatcher_t disp;
    CBroadPhase_t broadPh;
    CWorld_t      cWorld;
};

#endif // PHYSICSSYSTEM_H