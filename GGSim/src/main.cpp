#include <ECSpp/ECSWorld.h>
#include <GGSim/Application.h>
#include <GGSim/Shape.h>
#include <GGSim/Transformable.h>
#include <GGSim/VertexArray.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <rp3d/reactphysics3d.h>


int main()
{
    epp::ECSWorld world;

    WindowModule& win = Application::instance().get<WindowModule>();
    Box           box({ 100, 1, 100 });
    Sphere        sphere(3.f);
    box.translate({ 0, -5, 0 });
    win.translate({ 0, 0, -10 });
    win.rotate(M_PI, { 0, 1, 0 });

    while (win.isOpen())
    {
        win.clear();
        win.draw(box);
        win.draw(sphere);
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
        if (glfwGetKey(win.win, GLFW_KEY_R) == GLFW_PRESS)
            sphere.rotate(0.01, { 1, 0, 0 });
    }

    return 0;
}
