#pragma once
#include <iostream>
#include <string>
#include <glm.hpp>

namespace EWorld
{
	class Shader
	{
	private:
		uint32_t shaderProgram;

		uint32_t CompileShader(uint32_t type, const std::string& source);
		std::string ReadFile(const char* filepath);
		uint32_t GetUniformLocationByName(const std::string& name);

	public:
		Shader() = default;
		~Shader();

		void Create(const char* vertexPath, const char* fragmentPath);

		void SetUniformMat4(const std::string& name, glm::mat4& mat);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform1i(const std::string& name, int value);

		void Enable();
		void Disable();

	};
}