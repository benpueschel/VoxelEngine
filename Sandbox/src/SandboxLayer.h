#pragma once
#include "Voxel/Core/Layer.h"

using namespace Voxel;

class SandboxLayer : public Layer 
{
public:
	SandboxLayer();
	virtual ~SandboxLayer() = default;

	virtual void OnAttach();
	virtual void OnDetach();
	virtual void OnUpdate(Timestep& timestep);
	virtual void OnEvent(Event& event);

	virtual void OnImGuiRender();

private:
	ShaderLibrary m_ShaderLibrary;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	Ref<VertexArray> m_SquareVA;

	Ref<Texture2D> m_Texture;
};

