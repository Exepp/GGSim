#include <GGSim/Application.h>
#include <GGSim/ShaderProgram.h>
#include <GGSim/Window.h>
#include <cstdio>
#include <fstream>
#include <glad/glad.h>
#include <sstream>

static GLchar errLog[1024] = { 0 };


Shader::Shader(char const* path, Type type)
{
    loadSrc(path, type);
}

Shader::Shader(Shader&& other)
{
    (*this) = std::move(other);
}

Shader& Shader::operator=(Shader&& other)
{
    idx      = other.idx;
    src      = std::move(other.src);
    srcPtr   = other.srcPtr;
    compiled = other.compiled;

    other.srcPtr   = nullptr;
    other.idx      = 0;
    other.compiled = false;

    return *this;
}

Shader::~Shader()
{
    clear();
}

void Shader::setSrc(std::string source, Type type)
{
    clear();
    idx    = glCreateShader(GLenum(type));
    src    = std::move(source);
    srcPtr = src.c_str();
    glShaderSource(idx, 1, &srcPtr, NULL);
}

bool Shader::loadSrc(char const* path, Type type)
{
    std::ifstream stream(path);
    if (stream)
    {
        std::stringstream strStream;
        strStream << stream.rdbuf();
        setSrc(strStream.str(), type);
        return true;
    }
    else
        printf("Shader loading error: No file found: %s\n", path);
    return false;
}

bool Shader::compile()
{
    if (!compiled && idx)
    {
        glCompileShader(idx);
        if (isSuccessful())
            compiled = true;
    }
    return compiled;
}

void Shader::clear()
{
    if (idx)
    {
        glDeleteShader(idx);
        src.clear();
        srcPtr   = nullptr;
        idx      = 0;
        compiled = false;
    }
}

bool Shader::isSuccessful() const
{
    int_t status = 0;
    glGetShaderiv(idx, GL_COMPILE_STATUS, &status);

    if (!status)
    {
        glGetShaderInfoLog(idx, sizeof(errLog), NULL, errLog);
        printf("Shader error: %s%i\n", errLog, idx);
    }
    return status;
}


ShaderProgram::ShaderProgram()
{
    idx = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(idx);
}

bool ShaderProgram::addShader(Shader const& shader)
{
    if (shader.compiled)
        glAttachShader(idx, shader.idx);
    return shader.compiled;
}

bool ShaderProgram::isSuccessful() const
{
    int_t status = 0;
    glGetProgramiv(idx, GL_LINK_STATUS, &status);

    if (!status)
    {
        glGetProgramInfoLog(idx, sizeof(errLog), NULL, errLog);
        printf("ShaderProgram error: %s\n", errLog);
    }
    return status;
}

bool ShaderProgram::compile()
{
    if (!compiled)
    {
        glLinkProgram(idx);
        compiled = isSuccessful();
    }
    return compiled;
}

void ShaderProgram::use()
{
    glUseProgram(idx);
}


ShaderModule::ShaderModule()
{
    Shader vertexShader   = Shader("./shaders/vertex.glsl", Shader::Type::Vertex);
    Shader fragmentShader = Shader("./shaders/fragment.glsl", Shader::Type::Fragment);

    if (vertexShader.compile() && fragmentShader.compile())
    {
        addShader(vertexShader);
        addShader(fragmentShader);
        if (compile())
        {
            use();
            auto winSize = App::getModules().windowM.Size;
            setUniform<Mat4_t>("proj", glm::perspective(glm::radians(45.f), winSize.x / winSize.y, 0.1f, 1000.f));
        }
    }

    glEnable(GL_DEPTH_TEST);
}