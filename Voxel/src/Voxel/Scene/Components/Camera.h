#pragma once

#include "Voxel/Scene/SceneCamera.h"
#include <Imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Voxel {

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void OnImGuiRender();
	};

}