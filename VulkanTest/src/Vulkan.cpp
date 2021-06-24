#include "Voxel.h"
#include "Voxel/EntryPoint.h"

#include "VulkanRenderLayer.h"

using namespace Voxel;

class VulkanApp : public Application
{
public:
	VulkanApp()
	{
		LOG_INFO("Sandbox Application loading");
		PushLayer(new VulkanRenderLayer());
	}

	virtual ~VulkanApp() = default;

	void OnEvent(Event& event) { }

};

Application* Voxel::CreateApplication()
{
	return new VulkanApp();
}