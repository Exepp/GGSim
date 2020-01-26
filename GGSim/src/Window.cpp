#include <GGSim/Application.h>
#include <GGSim/Typedefs.h>
#include <GGSim/Window.h>
#include <GLFW/glfw3.h>

static bool updateLastPos = false;

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static Vec2_t lastPos(xpos, ypos);
    Vec2_t        currPos(xpos, ypos);
    if (updateLastPos)
    {
        lastPos       = currPos;
        updateLastPos = false;
        return;
    }


    Vec2_t        delta = currPos - lastPos;
    WindowModule& win   = App::getModules().windowM;

    win.rotateGlobal(-delta.x / 100.f, { 0, 1, 0 });
    win.rotateLocal(-delta.y / 100.f, { 1, 0, 0 });
    lastPos = currPos;
}

static void focus_callback(GLFWwindow* window, int focused)
{
    if (focused)
        updateLastPos = true;
}


WindowModule::WindowModule()
    : Size(1280, 720)
{
    win = glfwCreateWindow(Size.x, Size.y, "GGSim", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwSwapInterval(0);
    // glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(win, mouse_callback);
    glfwSetWindowFocusCallback(win, focus_callback);
}

WindowModule::~WindowModule()
{
    glfwDestroyWindow(win);
}

void WindowModule::clear()
{
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowModule::draw(VertexArray const& va, Transform const& model, bool withWireframe)
{
    ShaderModule& shader = App::getModules().shaderM;
    shader.setUniform<Mat4_t>("view", inversedMatrix());
    shader.setUniform<Mat4_t>("model", model.matrix());

    if (withWireframe)
    {
        glLineWidth(2.f);
        shader.setUniform<bool>("forceBlack", true);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        va.draw();
        shader.setUniform<bool>("forceBlack", false);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    va.draw();
}

void WindowModule::draw(Drawable const& toDraw)
{
    toDraw.draw(*this);
}

void WindowModule::drawPoint(Vertex const& v, float size)
{
    VertexArray const billboard({ Vertex({ -0.5f, 0.5f, 0 }, v.clr),
                                  Vertex({ -0.5f, -0.5f, 0 }, v.clr),
                                  Vertex({ 0.5f, -0.5f, 0 }, v.clr),
                                  Vertex({ 0.5f, 0.5f, 0 }, v.clr) },
                                { 0, 1, 2,
                                  2, 3, 0 });

    Transform transf;
    transf.orient = orient;
    transf.sca    = Vec3_t(size, size, size);
    transf.pos    = v.pos;

    draw(billboard, transf, false);
}

void WindowModule::display()
{
    glfwSwapBuffers(win);
}

void WindowModule::input()
{
    glfwPollEvents();
    float speed = 1 / 650.f * 50.f;
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        moveLocal({ 0, 0, -speed });
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        moveLocal({ 0, 0, speed });
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        moveLocal({ -speed, 0, 0 });
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        moveLocal({ speed, 0, 0 });
    if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS)
        pos += Vec3_t(0, speed, 0);
    if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
        pos += Vec3_t(0, -speed, 0);
}

void WindowModule::close()
{
    glfwSetWindowShouldClose(win, true);
}

bool WindowModule::isOpen() const
{
    return !glfwWindowShouldClose(win);
}