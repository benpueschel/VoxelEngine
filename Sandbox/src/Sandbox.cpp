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

	RenderLayer() : Layer("RenderLayer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_ShaderLibrary()
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
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(RenderLayer::OnWindowResize));
	}

	bool OnWindowResize(WindowResizeEvent& event)
	{

		float w = (float)event.GetWidth() / 1000.0f;
		float h = (float)event.GetHeight() / 1000.0f;

		LOG_DEBUG("Window Resize: {0}/{1}", w, h);

		m_Camera.RecalculateProjectionMatrix(-w, w, -h, h);
		return false;
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

		m_FrameTime = timestep * 1000;

		float cameraSpeed = 2 * timestep;
		float cameraRotationSpeed = 90 * timestep;

		if (Input::IsKeyPressed(KeyCode::KEY_W))
			m_CameraPosition.y += cameraSpeed;
		if (Input::IsKeyPressed(KeyCode::KEY_S))
			m_CameraPosition.y -= cameraSpeed;
		if (Input::IsKeyPressed(KeyCode::KEY_A))
			m_CameraPosition.x -= cameraSpeed;
		if (Input::IsKeyPressed(KeyCode::KEY_D))
			m_CameraPosition.x += cameraSpeed;

		if (Input::IsKeyPressed(KeyCode::KEY_Q))
			m_CameraRotation.y += cameraRotationSpeed;
		if (Input::IsKeyPressed(KeyCode::KEY_E))
			m_CameraRotation.y -= cameraRotationSpeed;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Renderer::BeginScene(m_Camera);

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

	OrthographicCamera m_Camera;
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