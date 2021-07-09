#include <pch.h>
#include "Transform.h"

namespace Voxel {

	glm::mat4 TransformComponent::GetTransform()
	{
		//glm::mat4 rotation = glm::toMat4(glm::quat(LocalRotation));
		//return glm::translate(glm::mat4(1.0f), LocalPosition) * rotation * glm::scale(glm::mat4(1.0f), LocalScale);

		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), LocalRotation.x, { 1, 0, 0 })
			* glm::rotate(glm::mat4(1.0f), LocalRotation.y, { 0, 1, 0 })
			* glm::rotate(glm::mat4(1.0f), LocalRotation.z, { 0, 0, 1 });

		return glm::translate(glm::mat4(1.0f), LocalPosition)
			* rotation
			* glm::scale(glm::mat4(1.0f), LocalScale);
	}

	void TransformComponent::OnImGuiRender()
	{
		UI::DrawVec3Control("Position", LocalPosition);

		glm::vec3 rotation = glm::degrees(LocalRotation);
		UI::DrawVec3Control("Rotation", rotation);
		LocalRotation = glm::radians(rotation);

		UI::DrawVec3Control("Scale", LocalScale, 1.0f, 0.001f);
	}

}