#pragma once

#include "Voxel.h"

using namespace Voxel;

class Sandbox2D : public Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() {}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep& timestep) override;
	virtual void OnEvent(Event& event) override;

	virtual void OnImGuiRender() override;

private:
	OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	Ref<Texture2D> m_Texture;

};