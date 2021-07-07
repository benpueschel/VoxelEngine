#include <pch.h>
#include "Renderer2D.h"

#include "Voxel/Rendering/Shaders/VertexArray.h"
#include "Voxel/Rendering/Shaders/Shader.h"
#include "Voxel/Rendering/RenderCommand.h"

namespace Voxel {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		glm::vec2 TexScale;
	};

	struct RenderData2D
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture
		glm::vec4 QuadVertexPositions[4]
		{
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalTriangleCount() { return QuadCount * 6; }
		};

		Statistics Stats;
	};

	static RenderData2D s_Data;

	void Renderer2D::Init()
	{
		PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "_Position" },
			{ ShaderDataType::Float4, "_Color" },
			{ ShaderDataType::Float2, "_TexCoord" },
			{ ShaderDataType::Float,  "_TexIndex" },
			{ ShaderDataType::Float2, "_TexScale" },
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t vertexIndex = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = vertexIndex + 0;
			quadIndices[i + 1] = vertexIndex + 1;
			quadIndices[i + 2] = vertexIndex + 2;

			quadIndices[i + 3] = vertexIndex + 2;
			quadIndices[i + 4] = vertexIndex + 3;
			quadIndices[i + 5] = vertexIndex + 0;

			vertexIndex += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureShader = Shader::Create("assets/shaders/2DTextureShader.glsl");
		s_Data.TextureShader->Bind();

		int32_t samplers[s_Data.MaxTextureSlots];
		for (int i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	}

	void Renderer2D::Shutdown()
	{
		PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetProjection() * glm::inverse(transform));

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}


	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*) s_Data.QuadVertexBufferPtr - (uint8_t*) s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->UploadData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(glm::mat4& transform, const glm::vec4& color)
	{
		DrawQuad(transform, nullptr, color);
	}

	void Renderer2D::DrawQuad(glm::mat4& transform, const Ref<Texture2D>& texture)
	{
		DrawQuad(transform, texture, glm::vec4(1.0f));
	}

	void Renderer2D::DrawQuad(glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = 
		{ 
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f }, 
			{ 1.0f, 1.0f }, 
			{ 0.0f, 1.0f }
		};
		int textureIndex = 0;
		if (texture)
		{
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *texture.get())
				{
					textureIndex = s_Data.TextureSlotIndex;
					break;
				}
			}

			if (textureIndex == 0)
			{
				textureIndex = s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		glm::vec2& textureScale = texture ? texture->GetScale() : glm::vec2(1.0f);

		for (size_t i = 0; i < quadVertexCount; i++)
		{

			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = static_cast<float>(textureIndex);
			s_Data.QuadVertexBufferPtr->TexScale = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		/*
		texture->Bind();
		s_Data.TextureShader->SetMat4("u_Transform", transform.GetTransform());
		s_Data.TextureShader->SetFloat("u_Scale", texture->GetScale());

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray); */
	}

}