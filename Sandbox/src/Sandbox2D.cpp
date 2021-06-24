#include <pch.h>
#include "Sandbox2D.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

using namespace Voxel;

Sandbox2D::Sandbox2D() 
	: Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f)
{
	m_Texture = Texture2D::Create("assets/textures/ChernoLogo.png");
}

void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Timestep& timestep)
{
	PROFILE_FUNCTION();

	{
		m_CameraController.OnUpdate(timestep);
	}

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	{
		PROFILE_SCOPE("Draw 400 Quads");
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				Transform transform;
				transform.SetScale(glm::vec3(0.1f));
				transform.SetPosition(glm::vec3(x * 1.1f, y * 1.1f, 0.0f) * transform.GetScale());

				Renderer2D::DrawQuad(transform, m_SquareColor);
			}
		}
	}
	{
		PROFILE_SCOPE("Draw 1 Quad");
		Transform transform;
		transform.SetPosition({ 0, 0, 10.0f });
		Renderer2D::DrawQuad(transform, m_Texture);
	}

	Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	PROFILE_FUNCTION();
	ImGui::Begin("Debug");

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::Text("Camera Position: %.2f %.2f %.2f",
		m_CameraController.GetCamera().GetTransform().GetPosition().x,
		m_CameraController.GetCamera().GetTransform().GetPosition().y,
		m_CameraController.GetCamera().GetTransform().GetPosition().z
	);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
