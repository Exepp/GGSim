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
    box.getComponent<ShapeComponent>()->setShape(Box({ 200, 200, 200 }));
    box.getComponent<PhysicsComponent>()->setMass(INFINITY);
    box.getComponent<TransformComponent>()->pos += Vec3_t(0.f, -100.f, 0.f);

    auto boxf = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent, NoDrawTag>();
    boxf.getComponent<ShapeComponent>()->setShape(Box({ 200, 400, 200 }));
    boxf.getComponent<PhysicsComponent>()->setMass(INFINITY);
    boxf.getComponent<TransformComponent>()->pos += Vec3_t(0.f, 0.f, -200.f);

    auto boxb = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent, NoDrawTag>();
    boxb.getComponent<ShapeComponent>()->setShape(Box({ 200, 400, 200 }));
    boxb.getComponent<PhysicsComponent>()->setMass(INFINITY);
    boxb.getComponent<TransformComponent>()->pos += Vec3_t(0.f, 0.f, 200.f);

    auto boxl = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent, NoDrawTag>();
    boxl.getComponent<ShapeComponent>()->setShape(Box({ 200, 400, 200 }));
    boxl.getComponent<PhysicsComponent>()->setMass(INFINITY);
    boxl.getComponent<TransformComponent>()->pos += Vec3_t(-200.f, 0.f, 0.f);

    auto boxr = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent, NoDrawTag>();
    boxr.getComponent<ShapeComponent>()->setShape(Box({ 200, 400, 200 }));
    boxr.getComponent<PhysicsComponent>()->setMass(INFINITY);
    boxr.getComponent<TransformComponent>()->pos += Vec3_t(200.f, 0.f, 0.f);

    // box = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
    // box.getComponent<ShapeComponent>()->setShape(Box({ 200, 20, 1 }));
    // box.getComponent<PhysicsComponent>()->setMass(INFINITY);
    // box.getComponent<TransformComponent>()->pos += Vec3_t(0.f, 0.f, -100.f);

    // auto box2 = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
    // box2.getComponent<ShapeComponent>()->setShape(Box({ 10, 1, 10 }));
    // box2.getComponent<TransformComponent>()->pos += Vec3_t(0.f, 1.5, 0.f);
    // box2.getComponent<PhysicsComponent>()->setMass(100);

    // box2.getComponent<TransformComponent>()->rotateLocal(M_PI_4 * 1.5, Vec3_t(1, 0, 0));
    // box2.getComponent<TransformComponent>()->rotateLocal(M_PI_4, Vec3_t(0, 1, 0));
    // box2.getComponent<PhysicsComponent>()->vel = Vec3_t(3.f, -20.f, 0.f);

    // auto box3 = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
    // box3.getComponent<ShapeComponent>()->setShape(Box({ 1, 1, 1 }));
    // box3.getComponent<TransformComponent>()->pos += Vec3_t(2.f, 1.f, 0.f);

    // auto box4 = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
    // box4.getComponent<ShapeComponent>()->setShape(Box({ 1, 1, 1 }));
    // box4.getComponent<TransformComponent>()->pos += Vec3_t(-2.f, 1.f, 0.f);


    for (int i = 0; i < 100; ++i)
    {
        auto obj = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
        obj.getComponent<ShapeComponent>()->setShape(Sphere(0.2));
        obj.getComponent<TransformComponent>()->pos += Vec3_t(0, i, 0);
    }

    for (int i = 0; i < 300; ++i)
    {
        auto obj = world.entityManager.spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
        obj.getComponent<ShapeComponent>()->setShape(Box({ 1.5, 1, 1 }));
        obj.getComponent<TransformComponent>()->pos += Vec3_t(0, i, 0);
    }

    winM.pos += Vec3_t(1, 1.f, 10);
    epp::SimulationClock::update();

    float timeRate = 1.f;
    bool  click    = false;
    bool  click2   = false;

    while (winM.isOpen())
    {
        // auto catchUpNum = epp::SimulationClock::update();
        // for (int i = 0; i < catchUpNum; ++i)
        float step = 1 / 650.f * timeRate;
        world.update(step, false);
        // world.update(epp::SimulationClock::getDeltaTime() * timeRate, false);
        // std::this_thread::sleep_for(std::chrono::nanoseconds((int)1e7));

        winM.input();
        if (glfwGetKey(winM.win, GLFW_KEY_R) == GLFW_PRESS)
        {
            // box2.getComponent<TransformComponent>()->pos = { 0, 2.5f, 0 };
            // box2.getComponent<PhysicsComponent>()->angularVel = Vec3_t(0, 0, 0);
            // box2.getComponent<PhysicsComponent>()->vel = Vec3_t(5, 0, 0);
        }
        if (glfwGetKey(winM.win, GLFW_KEY_T) == GLFW_PRESS)
        {
            if (!click)
                timeRate /= 1.25f;
            click = true;
        }
        else
            click = false;
        if (glfwGetKey(winM.win, GLFW_KEY_Y) == GLFW_PRESS)
        {
            if (!click2)
                timeRate *= 1.25f;
            click2 = true;
        }
        else
            click2 = false;

        step *= 50.f;
        if (glfwGetKey(winM.win, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            boxf.getComponent<TransformComponent>()->moveLocal({ 0, 0, step });
            boxb.getComponent<TransformComponent>()->moveLocal({ 0, 0, -step });
            boxl.getComponent<TransformComponent>()->moveLocal({ step, 0, 0 });
            boxr.getComponent<TransformComponent>()->moveLocal({ -step, 0, 0 });
        }
        if (glfwGetKey(winM.win, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            boxf.getComponent<TransformComponent>()->moveLocal({ 0, 0, -step });
            boxb.getComponent<TransformComponent>()->moveLocal({ 0, 0, step });
            boxl.getComponent<TransformComponent>()->moveLocal({ -step, 0, 0 });
            boxr.getComponent<TransformComponent>()->moveLocal({ step, 0, 0 });
        }
    }

    return 0;
}
