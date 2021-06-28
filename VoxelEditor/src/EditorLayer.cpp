#include "EditorLayer.h"

#include <ImGui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

namespace Voxel {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(16.0f / 9.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		m_Texture = Texture2D::Create("assets/textures/ChernoLogo.png");
		m_Texture->SetScale(glm::vec2(2.0f));

		FramebufferSpecification viewportSpec;
		viewportSpec.Width = 1280;
		viewportSpec.Height = 720;

		m_Framebuffer = Framebuffer::Create(viewportSpec);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep& timestep)
	{


		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t) m_ViewportSize.x, (uint32_t) m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}

		if (m_ViewportFocused)
			m_CameraController.OnUpdate(timestep);

		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f ,0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

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

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool opt_fullscreen = true;
		static bool dockspaceOpen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, { 0.0f, 0.0f }, dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Debug");

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::Text("Camera Position: %.2f %.2f %.2f",
			m_CameraController.GetCamera().GetTransform().GetPosition().x,
			m_CameraController.GetCamera().GetTransform().GetPosition().y,
			m_CameraController.GetCamera().GetTransform().GetPosition().z
		);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		// TODO: Viewport

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		m_ViewportSize = ImGui::GetContentRegionAvail();

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, m_ViewportSize, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();


		ImGui::End();
	}

}