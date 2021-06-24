#include <pch.h>
#include "Renderer2D.h"

#include "Voxel/Rendering/Shaders/VertexArray.h"
#include "Voxel/Rendering/Shaders/Shader.h"
#include "Voxel/Rendering/RenderCommand.h"

namespace Voxel {

	struct RenderData2D
	{
		Ref<VertexArray> QuadVertexArray;
		ShaderLibrary ShaderLibrary;
		glm::mat4 ViewProjectionMatrix;
	};

	static RenderData2D* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new RenderData2D();

		s_Data->QuadVertexArray = VertexArray::Create();

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
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(squareIndices[0]));
		squareIB->Unbind();
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		auto& FlatColorShader = s_Data->ShaderLibrary.LoadShader("assets/shaders/ColorShader.glsl");
		
		auto& TextureShader = s_Data->ShaderLibrary.LoadShader("assets/shaders/TextureShader.glsl");
		TextureShader->Bind();
		TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(Transform& transform, const glm::vec4& color)
	{
		const auto& FlatColorShader = s_Data->ShaderLibrary.GetShader("ColorShader");

		FlatColorShader->Bind();
		FlatColorShader->SetMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
		FlatColorShader->SetFloat4("u_Color", color);
		FlatColorShader->SetMat4("u_Transform", transform.GetTransform());

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(Transform& transform, const Ref<Texture2D>& texture)
	{
		const auto& TextureShader = s_Data->ShaderLibrary.GetShader("TextureShader");

		TextureShader->Bind();
		TextureShader->SetMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
		TextureShader->SetMat4("u_Transform", transform.GetTransform());
		texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}