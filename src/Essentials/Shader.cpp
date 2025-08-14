#include "Shader.h"
#include <fstream>
#include <sstream>
#include <glad/glad.h>

namespace zRender
{
	std::string get_file_contents(const char* filepath)
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

	uint32_t Shader::GetUniformLocation(const std::string& uniformName)
	{
		auto uniform = uniformLocationMap.find(uniformName);
		if (uniform != uniformLocationMap.end())
			return uniform->second;

		GLuint location = glGetUniformLocation(ID, uniformName.c_str());

		if (location == GL_INVALID_INDEX)
		{
			std::cout << "Unifrom { " << uniformName << " } not found in the shader!" << std::endl;
			return -1;
		}

		uniformLocationMap.emplace(uniformName, location);
		return location;
	}

	Shader::Shader(const char* vertexFile, const char* fragmentFile)
	{
		std::string vertexSource = get_file_contents(vertexFile);
		std::string fragmentSource = get_file_contents(fragmentFile);

		uint32_t vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
		uint32_t fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		if (vertex == 0 || fragment == 0)
		{
			std::cout << "Failed to Compile Shader!" << std::endl;
			return;
		}

		ID = glCreateProgram();

		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);

		glLinkProgram(ID);

		glValidateProgram(ID);

		int success;
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	Shader::~Shader()
	{
		if (ID != 0)
			glDeleteProgram(ID);
	}
	void Shader::SetUniformBool(const std::string& name, bool value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}
	void Shader::SetUniformInt(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}
	void Shader::SetUniformFloat(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}
	void Shader::SetUniformVector2(const std::string& name, const zMath::Vector2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}
	void Shader::SetUniformVector3(const std::string& name, const zMath::Vector3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}
	void Shader::SetUniformVector4(const std::string& name, const zMath::Vector4& value)
	{
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}
	void Shader::SetUniformMatrix4x4(const std::string& name, const zMath::Matrix4x4& value)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value.m[0][0]);
	}
	void Shader::SetUniformIntArray(const std::string& name, int size, int array[])
	{
		//glUniform1iv(GetUniformLocation(name), sizeof(array) / sizeof(int), array);
		glUniform1iv(GetUniformLocation(name), size, array);
	}
	void Shader::Bind()
	{
		glUseProgram(ID);
	}
	void Shader::Unbind()
	{
		glUseProgram(0);
	}
}
