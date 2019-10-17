#include <GGSim/Application.h>
#include <assert.h>


App::Modules App::modules;
bool         App::inited = true;


App::Modules& App::getModules()
{
    assert(inited);
    return modules;
}

void App::startUp()
{
    if (!inited)
    {
        inited = true;
        new (&modules) Modules();
    }
}

void App::shutDown()
{
    if (inited)
    {
        inited = false;
        modules.~Modules();
    }
}
