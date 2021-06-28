#pragma once

#include "Voxel/Rendering/Camera.h"
#include <glm/glm.hpp>

namespace Voxel {

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		SceneCamera(const glm::mat4& projection);
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, const glm::vec2& clipingPlane);
		void SetPerspective(float fov, const glm::vec2& clipingPlane);

		void SetViewportSize(uint32_t width, uint32_t height);

		glm::vec2& GetClippingPlane() { return m_ClippingPlane; }

	private:
		void RecalculateProjection();

	private:
		bool m_Orthographic = false;
		float m_PerspectiveFOV = 90.0f;
		float m_OrthographicSize = 10.0f;
		float m_AspectRatio = 0.0f;
		glm::vec2 m_ClippingPlane { 0.001f, 1000.0f };
	};

}