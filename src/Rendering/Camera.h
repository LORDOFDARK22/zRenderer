#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace EWorld
{
	class Camera
	{
	private:
		glm::vec2 position;
		float scale;
		//glm::mat4 view;
		glm::mat4 projection;

	public:
		Camera(const glm::vec2& position, int window_width, int window_height, float scale);
		~Camera() = default;

		inline const glm::vec2 GetPosition() const { return position; }
		inline const float GetScale() const { return scale; }
		//inline const glm::mat4 GetView() const { return view; }
		inline glm::mat4& GetProjection() { return projection; }

		void Update(int window_width, int window_height);

	};
}