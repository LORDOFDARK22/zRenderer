#pragma once
#include <vector>

#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Buffers/ElementBuffer.h"
#include "../Essentials/Shader.h"
#include "../Essentials/Texture.h"
#include "Camera2D.h"

namespace zRender
{
	class Mesh
	{
	private:
		VertexArray VAO;
		VertexBuffer VBO;
		ElementBuffer EBO;
		bool isStatic = false;

	public:
		std::vector<zRender::Vertex> vertices;
		std::vector<uint32_t> indices;

		Mesh(bool isStatic);
		Mesh();
		~Mesh();

		void Begin();
		void End();

		void Render(Shader* shader, Texture* texture, Camera2D& camera);

	};
}