#include <GGSim/Application.h>
#include <GGSim/Window.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static Vec2_t lastPos(xpos, ypos);
    Vec2_t        currPos(xpos, ypos);
    Vec2_t        delta = currPos - lastPos;
    WindowModule& win   = Application::instance().get<WindowModule>();

    win.rotate(delta.x / 100.f, { 0, 1, 0 });
    win.rotate(delta.y / 100.f, win.getTransform().rot[0]);
    lastPos = currPos;
}


WindowModule::WindowModule()
    : Size(800, 600)
{
    win = glfwCreateWindow(Size.x, Size.y, "GGSim", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(win, mouse_callback);
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

void WindowModule::draw(VertexArray const& va, Transform const& model)
{
    ShaderModule& shader = Application::instance().get<ShaderModule>();
    shader.setUniform<Mat4_t>("view", getTransform().inversed().asMatrix());
    shader.setUniform<Mat4_t>("model", model.asMatrix());

    shader.setUniform<bool>("forceBlack", true);
    glLineWidth(5.f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    va.draw();
    shader.setUniform<bool>("forceBlack", false);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    va.draw();
}

void WindowModule::draw(Drawable const& toDraw)
{
    toDraw.draw(*this);
}

void WindowModule::display()
{
    glfwSwapBuffers(Application::instance().get<WindowModule>().win);
}

void WindowModule::input()
{
    glfwPollEvents();
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        move({ 0, 0, -0.1 });
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        move({ 0, 0, 0.1 });
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        move({ -0.1, 0, 0 });
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        move({ 0.1, 0, 0 });
    if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS)
        translate({ 0, 0.1, 0 });
    if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
        translate({ 0, -0.1, 0 });
}

void WindowModule::close()
{
    glfwSetWindowShouldClose(Application::instance().get<WindowModule>().win, true);
}

bool WindowModule::isOpen() const
{
    return !glfwWindowShouldClose(Application::instance().get<WindowModule>().win);
}