#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <zMath/Vector2.h>
#include <zMath/Vector3.h>
#include <zMath/Vector4.h>
#include <zMath/Matrix4x4.h>
#include "Color.h"

namespace zRender
{
	class Shader
	{
	private:
		uint32_t ID;
		std::unordered_map<std::string, uint32_t> uniformLocationMap;

		uint32_t CompileShader(uint32_t type, const std::string& source);
		uint32_t GetUniformLocation(const std::string& uniformName);

	public:
		Shader();
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		Shader(const char* vertexFile, const char* fragmentFile);
		~Shader();

		const uint32_t GetID() const { return ID; }

		//set uniforms
		void SetUniformBool(const std::string& name, bool value);
		void SetUniformInt(const std::string& name, int value);
		void SetUniformFloat(const std::string& name, float value);
		//Vectors
		void SetUniformVector2(const std::string& name, const zMath::Vector2& value);
		void SetUniformVector3(const std::string& name, const zMath::Vector3& value);
		void SetUniformVector4(const std::string& name, const zMath::Vector4& value);
		//Matrix
		void SetUniformMatrix4x4(const std::string& name, const zMath::Matrix4x4& value);
		//array
		void SetUniformIntArray(const std::string& name, int size, int array[]);
		//color
		void SetUniformColor(const std::string& name, zRender::Color value);

		void Bind();
		void Unbind();

		//static
		static std::shared_ptr<Shader> CreateSpriteBatchShader();
		static std::shared_ptr<Shader> CreateCircleShader();
		static std::shared_ptr<Shader> CreateDefaultShader();
		static std::shared_ptr<Shader> CreateLineShader();
		static std::shared_ptr<Shader> CreatePixelShader();
		static std::shared_ptr<Shader> CreateSpritesShader();
		static std::shared_ptr<Shader> CreateTextBatchShader();
		static std::shared_ptr<Shader> CreateTextsShader();
		static std::shared_ptr<Shader> CreateTriangleShader();
	};
}