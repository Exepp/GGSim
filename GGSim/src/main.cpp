#include <ECSpp/ECSWorld.h>
#include <GGSim/Application.h>
#include <GGSim/ShapeComponent.h>
#include <GGSim/TransformComponent.h>
#include <GGSim/VertexArray.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <rp3d/reactphysics3d.h>


int main()
{
    epp::ECSWorld world;

    auto box    = world.entityManager.spawn<TransformComponent, ShapeComponent>();
    auto sphere = world.entityManager.spawn<TransformComponent, ShapeComponent>();

    box.getComponent<ShapeComponent>()->setShape(Box({ 100, 1, 100 }));
    sphere.getComponent<ShapeComponent>()->setShape(Sphere(3.f));

    WindowModule& win = Application::instance().get<WindowModule>();

    // box.translate({ 0, -5, 0 });
    // win.translate({ 0, 0, -10 });
    // win.rotate(M_PI, { 0, 1, 0 });

    while (win.isOpen())
        {
            win.clear();
            box.getComponent<ShapeComponent>()->shape->draw(win, box.getComponent<TransformComponent>()->getTransform());
            sphere.getComponent<ShapeComponent>()->shape->draw(win, sphere.getComponent<TransformComponent>()->getTransform());
            win.display();

            win.input();

            // glfwPollEvents();
            // if (glfwGetKey(win.win, GLFW_KEY_W) == GLFW_PRESS)
            //     box.move({ 0, 0, -0.1 });
            // if (glfwGetKey(win.win, GLFW_KEY_S) == GLFW_PRESS)
            //     box.move({ 0, 0, 0.1 });
            // if (glfwGetKey(win.win, GLFW_KEY_A) == GLFW_PRESS)
            //     box.move({ -0.1, 0, 0 });
            // if (glfwGetKey(win.win, GLFW_KEY_D) == GLFW_PRESS)
            //     box.move({ 0.1, 0, 0 });
            // if (glfwGetKey(win.win, GLFW_KEY_R) == GLFW_PRESS)
            //     sphere.rotate(0.01, { 1, 0, 0 });
        }

    return 0;
}
