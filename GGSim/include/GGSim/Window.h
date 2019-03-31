
#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include "ShaderProgram.h"
#include <GLFW/glfw3.h>

struct GlfwModule
{
    GlfwModule()
    {
        if (glfwInit() == GLFW_TRUE)
        {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
    }

    ~GlfwModule()
    {
        glfwTerminate();
    }
};

struct GladModule
{
    GladModule()
    {
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }
};


class WindowModule
{
public:
    WindowModule()
    {
        win = glfwCreateWindow(defWidth, defHeight, "", NULL, NULL);
        glfwMakeContextCurrent(win);
    }

    ~WindowModule()
    {
        glfwDestroyWindow(win);
    }

private:
    class GLFWwindow* win;

    static int const defWidth = 800;

    static int const defHeight = 600;

    friend class Window;
};


class Window
{
public:
    explicit Window(size_t width, size_t height, std::string const& title);

    void makeWindow(size_t width, size_t height, std::string const& title);


    void clear();

    void draw();

    void display();


    void close();

    bool isOpen() const;
};

#endif // WINDOW_H