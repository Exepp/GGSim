
#ifndef WINDOW_H
#define WINDOW_H

#include <GGSim/ShaderProgram.h>
#include <GGSim/Shape.h>
#include <GGSim/Transformable.h>
#include <GGSim/VertexArray.h>
#include <GLFW/glfw3.h>
#include <string>


class GLFWwindow;


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


class WindowModule : public Transformable
{
public:
    WindowModule();

    ~WindowModule();


    void clear();

    void draw(VertexArray const& va, Transform const& model);

    void draw(Drawable const& toDraw);

    void display();

    void input();

    void close();

    bool isOpen() const;

public:
    Vec2_t const Size;

    // private:
    GLFWwindow* win;
};

#endif // WINDOW_H