#pragma once

#include "Voxel/Core/Layer.h"

using namespace Voxel;

class VulkanRenderLayer : public Layer
{
public:
	VulkanRenderLayer();
	~VulkanRenderLayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnEvent(Event& event) override;

	void OnImGuiRender() override;
	void OnUpdate(Timestep timestep) override;
};