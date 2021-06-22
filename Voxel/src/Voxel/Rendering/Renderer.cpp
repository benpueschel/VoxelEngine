#include <pch.h>
#include "Renderer.h"

#include "Voxel/Rendering/Shaders/VertexArray.h"

// TEMPORARY
#include "Platform/OpenGL/Shaders/OpenGLShader.h"

namespace Voxel {

	Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}


	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		//static_cast<Ref<OpenGLShader>>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		//static_cast<Ref<OpenGLShader>>(shader)->UploadUniformMat4("u_Transform", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}