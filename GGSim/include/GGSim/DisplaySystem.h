#ifndef DISPLAYSYSTEM_H
#define DISPLAYSYSTEM_H

#include <ECSpp/System.h>
#include <GGSim/ShapeComponent.h>
#include <GGSim/TransformComponent.h>

class DisplaySystem : public epp::System
{
public:
    virtual void init(epp::EntityManager& entityManager) override;

    virtual void update(epp::EntityManager& entityManager, float dt, bool catchUpTick) override;

private:
    epp::CGroup<ShapeComponent, TransformComponent> entities;
};

#endif // DISPLAYSYSTEM_H