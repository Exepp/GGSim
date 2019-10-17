#include <ECSpp/ECSWorld.h>
#include <ECSpp/Utility/Time.h>
#include <GGSim/Application.h>
#include <GGSim/DisplaySystem.h>
#include <GGSim/PhysicsSystem.h>
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    // cworld.performDiscreteCollisionDetection();

    // auto disp = cworld.getDispatcher();

    // for (int i = 0; i < disp->getNumManifolds(); ++i)
    // {
    //     btPersistentManifold* mfld = disp->getManifoldByIndexInternal(i);
    //     mfld->refreshContactPoints(mfld->getBody0()->getWorldTransform(), mfld->getBody1()->getWorldTransform());

    //     for (int j = 0; j < mfld->getNumContacts(); ++j)
    //     {
    //         btManifoldPoint& pt  = mfld->getContactPoint(j);
    //         btVector3        r1  = pt.getPositionWorldOnA();
    //         btVector3        r2  = pt.getPositionWorldOnB();
    //         double           pen = pt.getDistance();
    //     }
    // }

    // check for memory leaks
    // App::shutDown();
    // App::startUp();

    WindowModule& winM = App::getModules().windowM;
    epp::ECSWorld world;

    world.makeSystem<PhysicsSystem>();
    world.makeSystem<DisplaySystem>();

    auto box = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
    box.getComponent<ShapeComponent>()->setShape(Box({ 200, 1, 200 }));
    box.getComponent<PhysicsComponent>()->setMass(INFINITY);

    auto box2 = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
    box2.getComponent<ShapeComponent>()->setShape(Box({ 1, 1, 1 }));
    box2.getComponent<TransformComponent>()->pos += Vec3_t(0.f, 10.f, 0.f);
    box2.getComponent<TransformComponent>()->rotateGlobal(-M_PI * 0.9f, { 0.f, 0.f, 1.f });
    box2.getComponent<PhysicsComponent>()->angularVel = -Vec3_t(-0.1, 0.1, -M_PI_2);
    // box2.getComponent<PhysicsComponent>()->setMass(1000);
    // for (int i = 0; i < 100; ++i)
    // {
    //     auto obj = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
    //     obj.getComponent<ShapeComponent>()->setShape(Box({ 2, 0.5f, 4 }));
    //     obj.getComponent<TransformComponent>()->pos += Vec3_t(0.f + i, 10.f + i, i + 0.f);
    // }

    winM.pos += Vec3_t(1, 1.f, 10);
    epp::SimulationClock::update();

    float timeRate = 1.f;
    bool  click    = false;
    bool  click2   = false;

    while (winM.isOpen())
    {
        auto catchUpNum = epp::SimulationClock::update();
        for (int i = 0; i < catchUpNum; ++i)
            world.update(epp::SimulationClock::getDeltaTime() * timeRate, true);
        world.update(epp::SimulationClock::getDeltaTime() * timeRate, false);
        std::this_thread::sleep_for(std::chrono::nanoseconds((int)1e7));

        winM.input();
        if (glfwGetKey(winM.win, GLFW_KEY_R) == GLFW_PRESS)
        {
            box2.getComponent<TransformComponent>()->pos = { 0, 2.5f, 0 };
            // box2.getComponent<PhysicsComponent>()->angularVel = Vec3_t(0, 0, 0);
            box2.getComponent<PhysicsComponent>()->vel = Vec3_t(5, 0, 0);
        }
        if (glfwGetKey(winM.win, GLFW_KEY_T) == GLFW_PRESS)
        {
            if (!click)
                timeRate /= 2.f;
            click = true;
        }
        else
            click = false;
        if (glfwGetKey(winM.win, GLFW_KEY_Y) == GLFW_PRESS)
        {
            if (!click2)
                timeRate *= 2.f;
            click2 = true;
        }
        else
            click2 = false;

        if (glfwGetKey(winM.win, GLFW_KEY_RIGHT) == GLFW_PRESS)
            box.getComponent<TransformComponent>()->rotateGlobal(0.01f, { 0, 0, -1 });
        if (glfwGetKey(winM.win, GLFW_KEY_LEFT) == GLFW_PRESS)
            box.getComponent<TransformComponent>()->rotateGlobal(0.01f, { 0, 0, 1 });
    }

    return 0;
}
