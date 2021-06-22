#pragma once

#include <glm/glm.hpp>

namespace Voxel {

	struct Timestep
	{

		Timestep() : deltaTime(0), totalTime(0) {}

		operator float() const { return deltaTime; }
		operator glm::vec3() const { return glm::vec3(deltaTime); }

		Timestep(float _totalTime, float _deltaTime)
			: deltaTime(_deltaTime), totalTime(_totalTime)
		{
		}

		float deltaTime;
		float totalTime;

	};

}