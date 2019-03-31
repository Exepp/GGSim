#include <GGSim/Window.h>
#include <GGSim/Initializer.h>
#include <GGSim/ShaderProgram.h>


class Application
{
private:
    using Initer_t = Initializer<GlfwModule, WindowModule, GladModule, ShaderModule>;

    template<class T>
    using MPtr_t = Initer_t::MPtr_t<T>;

public:
    static Application& instance();


    template<class T>
    T& get()
    {
        return *initer.mPack.get<MPtr_t<T>>();
    }

private:
    Application() = default;

private:
    Initer_t initer;

    static Application app;
};