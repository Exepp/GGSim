#include <GGSim/Application.h>
#include <GGSim/DisplaySystem.h>
#include <GGSim/Window.h>

void DisplaySystem::init(epp::EntityManager& entityManager)
{
    entityManager.requestCGroup(entities, { epp::getCTypeId<NoDrawTag>() });
}

void DisplaySystem::update(epp::EntityManager& entityManager, float dt, bool catchUpTick)
{
    // if (catchUpTick)
    //     return;

    WindowModule& winM = App::getModules().windowM;

    for (auto entity : entities)
        entity.get<ShapeComponent&>().getShape()->draw(winM, entity.get<TransformComponent&>());

    winM.display();

    winM.clear();
}