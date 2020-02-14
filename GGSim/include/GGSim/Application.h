#include <GGSim/ShaderProgram.h>
#include <GGSim/Window.h>

class App {
  struct Modules {
    GlfwModule glfwM;
    WindowModule windowM;
    GladModule gladM;
    ShaderModule shaderM;
  };

public:
  static Modules &getModules();

  static void startUp();
  static void shutDown();

private:
  RULEOF5_BAN_ALL(App);

private:
  static Modules modules;
  static bool inited;
};