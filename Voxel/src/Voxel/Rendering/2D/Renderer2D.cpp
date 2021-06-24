#include <pch.h>
#include "Renderer2D.h"

#include "Voxel/Rendering/Shaders/VertexArray.h"
#include "Voxel/Rendering/Shaders/Shader.h"
#include "Voxel/Rendering/RenderCommand.h"

namespace Voxel {

	struct RenderData2D
	{
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		Ref<VertexArray> QuadVertexArray;
		glm::mat4 ViewProjectionMatrix;
	};

	static RenderData2D* s_Data;

	void Renderer2D::Init()
	{
		PROFILE_FUNCTION();

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

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		auto& TextureShader = Shader::Create("assets/shaders/TextureShader.glsl");
		TextureShader->Bind();
		TextureShader->SetInt("u_Texture", 0);

		s_Data->TextureShader = TextureShader;
	}

	void Renderer2D::Shutdown()
	{
		PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		PROFILE_FUNCTION();

		s_Data->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(Transform& transform, const glm::vec4& color)
	{
		DrawQuad(transform, s_Data->WhiteTexture, color);
	}

	void Renderer2D::DrawQuad(Transform& transform, const Ref<Texture2D>& texture)
	{
		DrawQuad(transform, texture, glm::vec4(1.0f));
	}

	void Renderer2D::DrawQuad(Transform& transform, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		PROFILE_FUNCTION();

		texture->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetMat4("u_Transform", transform.GetTransform());

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}