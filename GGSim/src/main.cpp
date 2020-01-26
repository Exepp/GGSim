#include <ECSpp/ECSWorld.h>
#include <ECSpp/Utility/Time.h>
#include <GGSim/Application.h>
#include <GGSim/DisplaySystem.h>
#include <GGSim/PhysicsSystem.h>
#include <chrono>
#include <iostream>
#include <thread>

int main() {
  WindowModule &winM = App::getModules().windowM;
  epp::ECSWorld world;

  world.makeSystem<PhysicsSystem>();
  world.makeSystem<DisplaySystem>();

  auto box = world.entityManager
                 .spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
  box.getComponent<ShapeComponent>()->setShape(Box({200, 200, 200}));
  box.getComponent<PhysicsComponent>()->setMass(INFINITY);
  box.getComponent<TransformComponent>()->pos += Vec3_t(0.f, -100.f, 0.f);

  auto boxf = world.entityManager.spawn<TransformComponent, ShapeComponent,
                                        PhysicsComponent, NoDrawTag>();
  boxf.getComponent<ShapeComponent>()->setShape(Box({200, 400, 200}));
  boxf.getComponent<PhysicsComponent>()->setMass(INFINITY);
  boxf.getComponent<TransformComponent>()->pos += Vec3_t(0.f, 0.f, -200.f);

  auto boxb = world.entityManager.spawn<TransformComponent, ShapeComponent,
                                        PhysicsComponent, NoDrawTag>();
  boxb.getComponent<ShapeComponent>()->setShape(Box({200, 400, 200}));
  boxb.getComponent<PhysicsComponent>()->setMass(INFINITY);
  boxb.getComponent<TransformComponent>()->pos += Vec3_t(0.f, 0.f, 200.f);

  auto boxl = world.entityManager.spawn<TransformComponent, ShapeComponent,
                                        PhysicsComponent, NoDrawTag>();
  boxl.getComponent<ShapeComponent>()->setShape(Box({200, 400, 200}));
  boxl.getComponent<PhysicsComponent>()->setMass(INFINITY);
  boxl.getComponent<TransformComponent>()->pos += Vec3_t(-200.f, 0.f, 0.f);

  auto boxr = world.entityManager.spawn<TransformComponent, ShapeComponent,
                                        PhysicsComponent, NoDrawTag>();
  boxr.getComponent<ShapeComponent>()->setShape(Box({200, 400, 200}));
  boxr.getComponent<PhysicsComponent>()->setMass(INFINITY);
  boxr.getComponent<TransformComponent>()->pos += Vec3_t(200.f, 0.f, 0.f);

  for (int i = 0; i < 0; ++i) {
    auto obj =
        world.entityManager
            .spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
    obj.getComponent<ShapeComponent>()->setShape(Sphere(0.2));
    obj.getComponent<TransformComponent>()->pos += Vec3_t(0, i, 0);
  }

  for (int i = 20; i < 22; ++i) {
    auto obj =
        world.entityManager
            .spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
    obj.getComponent<ShapeComponent>()->setShape(Box({1, 1, 1}));
    obj.getComponent<TransformComponent>()->pos += Vec3_t(0, i, 0);
    // obj.getComponent<PhysicsComponent>()->vel = { 5, 0, 0 };

    // obj.getComponent<PhysicsComponent>()->angularVel = { 0, 0, 1 };
    // obj.getComponent<TransformComponent>()->rotateLocal(M_PI_4 / 2.f, { 0, 0,
    // 3 }); obj.getComponent<TransformComponent>()->rotateLocal(M_PI_4, { 0, 1,
    // 0 });
  }

  winM.pos += Vec3_t(1, 1.f, 10);
  epp::SimulationClock::update();

  float timeRate = 1.f;
  bool click = false;
  bool click2 = false;

  while (winM.isOpen()) {
    // auto catchUpNum = epp::SimulationClock::update();
    float step = 1 / 650.f * timeRate;
    world.update(step, false);
    // world.update(epp::SimulationClock::getDeltaTime() * timeRate, false);
    // std::this_thread::sleep_for(std::chrono::nanoseconds((int)(1e6)));

    winM.input();
    if (glfwGetKey(winM.win, GLFW_KEY_R) == GLFW_PRESS)
      box.getComponent<TransformComponent>()->pos += Vec3_t{0, step * 0.1f, 0};

    if (glfwGetKey(winM.win, GLFW_KEY_F) == GLFW_PRESS)
      box.getComponent<TransformComponent>()->pos -= Vec3_t{0, step * 0.1f, 0};
    if (glfwGetKey(winM.win, GLFW_KEY_T) == GLFW_PRESS) {
      if (!click)
        timeRate /= 1.25f;
      click = true;
    } else
      click = false;
    if (glfwGetKey(winM.win, GLFW_KEY_Y) == GLFW_PRESS) {
      if (!click2)
        timeRate *= 1.25f;
      click2 = true;
    } else
      click2 = false;

    step *= 50.f;
    // step *= 0.1f;
    static bool spawn = false;
    if (glfwGetMouseButton(winM.win, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
      spawn = false;
    }
    if (glfwGetMouseButton(winM.win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
        !spawn) {
      spawn = true;
      auto box =
          world.entityManager
              .spawn<TransformComponent, ShapeComponent, PhysicsComponent>();
      box.getComponent<ShapeComponent>()->setShape(Box({1, 3, 1}));
      box.getComponent<PhysicsComponent>()->setMass(150);
      box.getComponent<PhysicsComponent>()->vel =
          winM.orient * Vec3_t(0, 0, -1) * 5.f;
      box.getComponent<TransformComponent>()->pos = winM.pos;
    }

    if (glfwGetKey(winM.win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      // box.getComponent<TransformComponent>()->rotateGlobal(step, { 1, 0, 0
      // });
      boxf.getComponent<TransformComponent>()->moveLocal({0, 0, step});
      boxb.getComponent<TransformComponent>()->moveLocal({0, 0, -step});
      boxl.getComponent<TransformComponent>()->moveLocal({step, 0, 0});
      boxr.getComponent<TransformComponent>()->moveLocal({-step, 0, 0});
    }
    if (glfwGetKey(winM.win, GLFW_KEY_LEFT) == GLFW_PRESS) {
      // box.getComponent<TransformComponent>()->rotateGlobal(-step, { 1, 0, 0
      // });
      boxf.getComponent<TransformComponent>()->moveLocal({0, 0, -step});
      boxb.getComponent<TransformComponent>()->moveLocal({0, 0, step});
      boxl.getComponent<TransformComponent>()->moveLocal({-step, 0, 0});
      boxr.getComponent<TransformComponent>()->moveLocal({step, 0, 0});
    }
  }

  return 0;
}
