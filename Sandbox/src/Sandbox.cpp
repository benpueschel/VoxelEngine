#include <pch.h>
#include "Voxel.h"


#include "Voxel/ImGui/ImGuiLayer.h"
#include <ImGui/imgui.h>

#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/Shaders/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <sstream>

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

		//auto colorShader = m_ShaderLibrary.LoadShader("assets/shaders/ColorShader.glsl");
		//auto textureShader = m_ShaderLibrary.LoadShader("assets/shaders/TextureShader.glsl");

		auto colorShader = Shader::Create("assets/shaders/ColorShader.glsl");
		auto textureShader = Shader::Create("assets/shaders/TextureShader.glsl");

		m_ShaderLibrary.AddShader(colorShader);
		m_ShaderLibrary.AddShader(textureShader);

		//m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_Texture = Texture2D::Create("assets/textures/ChernoLogo.png");

		textureShader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

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

		std::stringstream frameTime;
		frameTime << "Frame Time: " << m_FrameTime;
		ImGui::Text(frameTime.str().c_str());

		ImGui::End();
	}

	void OnUpdate(Timestep timestep) override
	{

		m_CameraController.OnUpdate(timestep);

		m_FrameTime = timestep * 1000;

		Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto colorShader = m_ShaderLibrary.GetShader("ColorShader");

		colorShader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(colorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos - glm::vec3(1.0f)) * scale;
				Renderer::Submit(colorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Renderer::Submit(m_ShaderLibrary.GetShader("TextureShader"), m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		Renderer::EndScene();
	}

private:

	ShaderLibrary m_ShaderLibrary;

	glm::vec3 m_CameraPosition = { 0, 0, 0 };
	glm::vec3 m_CameraRotation = { 0, 0, 0.001f };

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	OrthographicCameraController m_CameraController;
	//PerspectiveCamera m_Camera;

	Ref<VertexArray> m_SquareVA;

	Ref<Texture2D> m_Texture;

	float m_FrameTime = 0;

};

class Sandbox : public Application
{

public:
	Sandbox()
	{

		LOG_INFO("Sandbox Application loading");
		PushLayer(new RenderLayer());

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