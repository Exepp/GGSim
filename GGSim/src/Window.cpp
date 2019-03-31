#include <GGSim/Window.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GGSim/Application.h>


Window::Window(size_t width, size_t height, std::string const& title)
{
    makeWindow(width, height, title);
}

void Window::makeWindow(size_t width, size_t height, std::string const& title)
{
    GLFWwindow* win = Application::instance().get<WindowModule>().win;
    glfwSetWindowSize(win, width, height);
    glfwSetWindowTitle(win, title.c_str());
}

void Window::clear()
{
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::draw()
{
    Application::instance().get<ShaderModule>().use();
}

void Window::display()
{
    glfwSwapBuffers(Application::instance().get<WindowModule>().win);
}

void Window::close()
{
    glfwSetWindowShouldClose(Application::instance().get<WindowModule>().win, true);
}

bool Window::isOpen() const
{
    return !glfwWindowShouldClose(Application::instance().get<WindowModule>().win);
}