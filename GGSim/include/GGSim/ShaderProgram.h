#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GGSim/Transformable.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
    enum class Type : GLenum
    {
        Vertex   = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    };

public:
    Shader() = default;

    Shader(Shader const& other) = delete;

    Shader& operator=(Shader const& other) = delete;

    explicit Shader(char const* path, Type type);

    Shader(Shader&& other);

    Shader& operator=(Shader&& other);

    ~Shader();


    void setSrc(std::string src, Type type);

    bool loadSrc(char const* path, Type type);


    bool compile();

private:
    bool isSuccessful() const;

    void clear();

private:
    GLuint idx = 0;

    std::string src;

    char const* srcPtr = nullptr;

    bool compiled = false;

    friend class ShaderProgram;
};


class ShaderProgram
{
public:
    ShaderProgram();

    ~ShaderProgram();


    bool addShader(Shader const& shader);

    bool compile();

    void use();

    template<class T>
    bool setUniform(char const* name, T const& value);

private:
    bool isSuccessful() const;

private:
    GLuint idx = 0;

    bool compiled = false;
};


class ShaderModule : public ShaderProgram
{
public:
    ShaderModule();
};


template<class T>
bool ShaderProgram::setUniform(char const* name, T const& value)
{
    int uniLoc = glGetUniformLocation(idx, name);
    if (uniLoc == -1)
        return false;

    if constexpr (std::is_same_v<T, bool>)
        glUniform1i(uniLoc, (GLint)value);
    else if constexpr (std::is_same_v<T, int>)
        glUniform1i(uniLoc, value);
    else if constexpr (std::is_same_v<T, float>)
        glUniform1f(uniLoc, (GLfloat)value);
    else if constexpr (std::is_same_v<T, Vec3_t>)
        glUniform3fv(uniLoc, 1, &value[0]);
    else if constexpr (std::is_same_v<T, Vec4_t>)
        glUniform4fv(uniLoc, 1, &value[0]);
    else if constexpr (std::is_same_v<T, Mat3_t>)
        glUniformMatrix3fv(uniLoc, 1, GL_FALSE, &value[0][0]);
    else if constexpr (std::is_same_v<T, Mat4_t>)
        glUniformMatrix4fv(uniLoc, 1, GL_FALSE, &value[0][0]);
    else
        return false;
    return true;
}
#endif // SHADER_PROGRAM_H