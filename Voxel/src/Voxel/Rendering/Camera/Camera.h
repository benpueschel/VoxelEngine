#pragma once

#include <glm/glm.hpp>
#include "Voxel/Transform.h"

namespace Voxel {

	class Camera
	{
	public:

		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;
		virtual Transform& GetTransform() = 0;
	};

}