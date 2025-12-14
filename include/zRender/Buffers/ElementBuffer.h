#pragma once
#include <vector>
#include <iostream>

namespace zRender
{
	class ElementBuffer
	{
	private:
		uint32_t ID;

	public:
		ElementBuffer();

		const uint32_t GetID() const { return ID; }

		void Data(std::vector<uint32_t>& indices, bool dynamic);

		void Bind();
		void Unbind();
		void Delete();
	};
}