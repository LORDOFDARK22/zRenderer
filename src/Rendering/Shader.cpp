#include "Shader.h"
#include <fstream>
#include <sstream>
#include <glad/glad.h>

namespace EWorld
{
	uint32_t Shader::CompileShader(uint32_t type, const std::string& source)
	{
		uint32_t shader = glCreateShader(type);

		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);

		glCompileShader(shader);

		int32_t result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* infoLog = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(shader, length, &length, infoLog);
			std::cout << "Failed to compile Shader!" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
			std::cout << infoLog << std::endl;
			glDeleteShader(shader);
			return static_cast<uint32_t>(0);
		}

		return shader;
	}
	std::string Shader::ReadFile(const char* filepath)
	{
		std::ifstream fs(filepath, std::ios::in);

		if (!fs.is_open())
		{
			std::cout << "Failed to Read file [" << filepath << "]" << std::endl;
			return "";
		}

		std::stringstream buffer;
		std::string line;
		while (std::getline(fs, line))
		{
			buffer << line << "\n";
		}

		fs.close();
		return buffer.str();
	}
	uint32_t Shader::GetUniformLocationByName(const std::string& name)
	{
		uint32_t loc = glGetUniformLocation(shaderProgram, name.c_str());

		if (loc == GL_INVALID_INDEX)
		{
			std::cout << "Failed to Get Uniform Location [" << name << "]" << std::endl;
			return -1;
		}

		return loc;
	}
	Shader::~Shader()
	{
		if (shaderProgram != 0)
			glDeleteProgram(shaderProgram);
	}
	void Shader::Create(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexSource = ReadFile(vertexPath);
		std::string fragmentSource = ReadFile(fragmentPath);

		uint32_t vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
		uint32_t fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		if (vertex == 0 || fragment == 0)
		{
			std::cout << "Failed to Compile Shader!" << std::endl;
			return;
		}

		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertex);
		glAttachShader(shaderProgram, fragment);

		glLinkProgram(shaderProgram);

		glValidateProgram(shaderProgram);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	void Shader::SetUniformMat4(const std::string& name, glm::mat4& mat)
	{
		glUniformMatrix4fv(GetUniformLocationByName(name), 1, GL_FALSE, &mat[0][0]);
	}
	void Shader::SetUniform1f(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocationByName(name), value);
	}
	void Shader::SetUniform1i(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocationByName(name), value);
	}
	void Shader::Enable()
	{
		glUseProgram(shaderProgram);
	}
	void Shader::Disable()
	{
		glUseProgram(0);
	}
}
