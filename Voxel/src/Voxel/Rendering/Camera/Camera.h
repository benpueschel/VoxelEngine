#pragma once

#include <glm/glm.hpp>

namespace Voxel {

	class Camera
	{
	public:
		virtual const glm::vec3& GetPosition() const = 0;

		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;

	};

}