#include <pch.h>
#include "Voxel.h"
#include "Voxel/EntryPoint.h"

#include "Voxel/ImGui/ImGuiLayer.h"
#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"
#include "SandboxLayer.h"

using namespace Voxel;

class Sandbox : public Application
{

public:
	Sandbox()
	{
		LOG_INFO("Sandbox Application loading");
		//PushLayer(new SandboxLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{
		LOG_INFO("Sandbox Application unloading");
	}

};

Voxel::Application* Voxel::CreateApplication()
{
	return new Sandbox();
}