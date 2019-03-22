#include <GGSim/Application.h>

Application Application::app;

Application& Application::instance()
{
	return app;
}