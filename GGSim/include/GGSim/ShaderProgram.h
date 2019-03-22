#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>

#include <cstdint>
#include <glad/glad.h>


class Shader
{
public:

	enum class Type : GLenum { Vertex = GL_VERTEX_SHADER, Fragment = GL_FRAGMENT_SHADER };

public:

	Shader() = default;

	Shader(Shader const &other) = delete;

	Shader &operator=(Shader const &other) = delete;

	explicit Shader(char const *path, Type type);

	Shader(Shader&& other);

	Shader& operator=(Shader&& other);

	~Shader();


	void setSrc(Type type);

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

private:

	bool isSuccessful() const;

private:

	GLuint idx = 0;

	bool compiled = false;
};



class ShaderModule : public ShaderProgram
{
public:

	ShaderModule()
	{
		vertexShader = Shader("./shaders/vertex.glsl", Shader::Type::Vertex);
		fragmentShader = Shader("./shaders/fragment.glsl", Shader::Type::Fragment);
		if(vertexShader.compile() && fragmentShader.compile())
		{
			addShader(vertexShader);
			addShader(fragmentShader);
			compile();
		}
	}

private:

	Shader vertexShader;

	Shader fragmentShader;
};

#endif // SHADER_PROGRAM_H