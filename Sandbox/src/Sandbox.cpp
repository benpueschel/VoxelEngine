#include <pch.h>
#include "Voxel.h"
#include "Voxel/EntryPoint.h"

#include "Voxel/ImGui/ImGuiLayer.h"
#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

using namespace Voxel;

class RenderLayer : public Layer
{
public:

	RenderLayer() : Layer("RenderLayer"), m_CameraController(16.0f / 9.0f), m_ShaderLibrary()
	{

		m_SquareVA = VertexArray::Create();

		float squareVertices[4 * 5] = {
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		};

		uint32_t squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		squareVB->SetLayout({
			{ ShaderDataType::Float3, "position" },
			{ ShaderDataType::Float2, "texCoord" },
		});
		squareVB->Unbind();
		m_SquareVA->AddVertexBuffer(squareVB);

		Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(squareIndices[0]));
		squareIB->Unbind();
		m_SquareVA->SetIndexBuffer(squareIB);

		auto FlatColorShader = m_ShaderLibrary.LoadShader("assets/shaders/ColorShader.glsl");
		auto TextureShader = m_ShaderLibrary.LoadShader("assets/shaders/TextureShader.glsl");

		//m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_Texture = Texture2D::Create("assets/textures/ChernoLogo.png");

		TextureShader->Bind();
		TextureShader->SetInt("u_Texture", 0);
	}

	~RenderLayer() {}

	void OnAttach() override {}
	void OnDetach() override {}
	void OnEvent(Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::Text("Camera Position: %f %f %f",
			m_CameraController.GetCamera().GetTransform().GetPosition().x,
			m_CameraController.GetCamera().GetTransform().GetPosition().y,
			m_CameraController.GetCamera().GetTransform().GetPosition().z
		);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	void OnUpdate(Timestep& timestep) override
	{
		m_CameraController.OnUpdate(timestep);

		Renderer::BeginScene(m_CameraController.GetCamera());

		auto FlatColorShader = m_ShaderLibrary.GetShader("ColorShader");
		FlatColorShader->Bind();
		FlatColorShader->SetFloat4("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				Transform transform;
				transform.SetScale(glm::vec3(0.1f));
				transform.SetPosition({ x * 0.11f, y * 0.11f, 0.0f });
				Renderer::Submit(FlatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Renderer::Submit(m_ShaderLibrary.GetShader("TextureShader"), m_SquareVA);


		Renderer::EndScene();
	}

private:

	ShaderLibrary m_ShaderLibrary;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	OrthographicCameraController m_CameraController;

	Ref<VertexArray> m_SquareVA;

	Ref<Texture2D> m_Texture;

};

class Sandbox : public Application
{

public:
	Sandbox()
	{
		LOG_INFO("Sandbox Application loading");
		//PushLayer(new RenderLayer());
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