#include "VertexArray.h"

namespace zRender
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &ID);
	}
	void VertexArray::LinkAttrib(VertexBuffer& vertexBuffer, uint32_t layout, uint32_t numComponents, GLenum type, size_t stride, void* offset)
	{
		vertexBuffer.Bind();

		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);

		vertexBuffer.Unbind();
	}
	void VertexArray::Bind()
	{
		glBindVertexArray(ID);
	}
	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}
	void VertexArray::Delete()
	{
		if (ID)
			glDeleteVertexArrays(1, &ID);
	}
}
