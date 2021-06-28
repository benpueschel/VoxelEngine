#include <pch.h>
#include "Renderer.h"

#include "Voxel/Rendering/Shaders/VertexArray.h"
#include "Voxel/Rendering/2D/Renderer2D.h"

#include "Voxel/Scene/Entity.h"
#include "Voxel/Scene/Components/Components.h"

namespace Voxel {

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<SceneData>();

	void Renderer::Init()
	{
		PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		PROFILE_FUNCTION();

		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowRestore(WindowRestoreEvent& event)
	{
		PROFILE_FUNCTION();

		RenderCommand::SetViewport({ 0, 0, event.GetWidth(), event.GetHeight() });
	}


	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(Entity& entity, const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
	{
		if (!entity.HasComponent<TransformComponent>()) return;

		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", entity.GetComponent<TransformComponent>().Transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}