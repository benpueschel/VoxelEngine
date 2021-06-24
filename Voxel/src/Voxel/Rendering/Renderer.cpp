#include <pch.h>
#include "Renderer.h"

#include "Voxel/Rendering/Shaders/VertexArray.h"
#include "Voxel/Rendering/2D/Renderer2D.h"

namespace Voxel {

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowFramebufferResize(WindowFramebufferResizeEvent& event)
	{
		RenderCommand::SetViewport({ 0, 0, event.GetWidth(), event.GetHeight() });
	}


	void Renderer::BeginScene(const Camera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, Transform& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform.GetTransform());

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}