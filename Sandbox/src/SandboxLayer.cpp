#include <pch.h>
#include "Voxel.h"
#include "SandboxLayer.h"

#include <ImGui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

SandboxLayer::SandboxLayer()
	: Layer("SandboxLayer"), m_ShaderLibrary()
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

void SandboxLayer::OnAttach() {}
void SandboxLayer::OnDetach() {}
void SandboxLayer::OnEvent(Event& event) 
{
}

void SandboxLayer::OnImGuiRender()
{
	ImGui::Begin("Debug");

	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void SandboxLayer::OnUpdate(Timestep& timestep)
{

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
