#pragma once
#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Buffers/ElementBuffer.h"

#include "Camera2D.h"
#include "../Essentials/Color.h"
#include "../Essentials/Shader.h"

namespace zRender
{
	template <typename Batch, typename Glyph>
	class Batcher
	{
	private:
		VertexArray VAO;
		VertexBuffer VBO;
		ElementBuffer EBO;

	protected:
		std::vector<Batch> batches;
		std::vector<Glyph> glyphs;

		inline VertexBuffer GetVBO() const { return VBO; }
		inline ElementBuffer GetEBO() const { return EBO; }

		void SetVertexAttribute(uint32_t layoutPosition, uint32_t numComponents, GLenum type, size_t stride, void* offset);

	public:
		Batcher();
		~Batcher();

		void Begin();

		virtual void End() = 0;

		virtual void Render(Shader* shader, Camera2D& camera) = 0;
	};
	template<typename Batch, typename Glyph>
	inline Batcher<Batch, Glyph>::Batcher() : VAO{}, VBO{}, EBO{}, batches{}, glyphs{}
	{
	}
	template<typename Batch, typename Glyph>
	inline Batcher<Batch, Glyph>::~Batcher()
	{
		VAO.Delete();
		VBO.Delete();
		EBO.Delete();
	}
	template<typename Batch, typename Glyph>
	inline void Batcher<Batch, Glyph>::SetVertexAttribute(uint32_t layoutPosition, uint32_t numComponents, GLenum type, size_t stride, void* offset)
	{
		VAO.Bind();

		VAO.LinkAttrib(VBO, layoutPosition, numComponents, type, stride, offset);

		VAO.Unbind();
	}
	template<typename Batch, typename Glyph>
	inline void Batcher<Batch, Glyph>::Begin()
	{
		batches.clear();
		glyphs.clear();
	}
}