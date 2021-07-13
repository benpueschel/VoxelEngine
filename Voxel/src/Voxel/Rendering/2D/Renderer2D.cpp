#include <pch.h>
#include "Renderer2D.h"

#include "Voxel/Rendering/Shaders/UniformBuffer.h"
#include "Voxel/Rendering/Shaders/VertexArray.h"
#include "Voxel/Rendering/Shaders/Shader.h"
#include "Voxel/Rendering/RenderCommand.h"

namespace Voxel {

	struct QuadVertex
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec4 Color = { 1.0f , 1.0f , 1.0f, 1.0f };
		glm::vec2 TexCoord = { 0.0f, 0.0f };
		float TexIndex = 0;
		glm::vec2 TexScale = { 1.0f, 1.0f };

		// Editor-only
		int EntityID = -1;
	};

	struct RenderData2D
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
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
		const glm::vec4 QuadVertexPositions[4]
		{
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;

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
			{ ShaderDataType::Int,	  "_EntityID" },
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

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(RenderData2D::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RenderData2D::CameraData));

		StartBatch();
	}


	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RenderData2D::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0) return; // Nothing to draw

		s_Data.QuadVertexArray->Bind();
		s_Data.TextureShader->Bind();
		
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& spriteRenderer, int entityID)
	{
		DrawQuad(transform, spriteRenderer.Color, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		DrawQuad(transform, nullptr, color, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, int entityID)
	{
		DrawQuad(transform, texture, glm::vec4(1.0f), entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, int entityID)
	{
		PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		int textureIndex = 0;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= RenderData2D::MaxIndices)
			NextBatch();

		if (texture)
		{
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i] == *texture)
				{
					textureIndex = (int) i;
					break;
				}
			}

			if (textureIndex == 0)
			{
				if (s_Data.TextureSlotIndex >= RenderData2D::MaxTextureSlots)
					NextBatch();

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
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
	}

}