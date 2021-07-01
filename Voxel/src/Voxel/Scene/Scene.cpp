#include <pch.h>
#include "Scene.h"

#include "Components/Components.h"
#include "Voxel/Rendering/2D/Renderer2D.h"
#include "Voxel/Rendering/Camera.h"

#include <glm/glm.hpp>

#include "Entity.h"
#include "ScriptableEntity.h"

#include "Voxel/Core/Application.h"
#include "Voxel/Scene/SceneSerializer.h"

namespace Voxel {

	Scene::Scene(const std::string& name)
		: m_Name(name)
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		return entity;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep timestep)
	{
		if (!Application::Get().GetImGuiLayer()->IsBlockingEvents())
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& script) {
				if (!script.Instance)
				{
					script.Instance = script.InstantiateScript();
					script.Instance->m_Entity = { entity, this };
					script.Instance->OnCreate();
				}

				script.Instance->OnUpdate(timestep);
			});
		}

		Camera* mainCamera = nullptr;
		glm::mat4& mainCameraTransform = glm::mat4(1.0f);
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					mainCameraTransform = transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), mainCameraTransform);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad((glm::mat4) transform, sprite.Color);
			}
			Renderer2D::EndScene();
		}

	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (!camera.FixedAspectRatio)
			{
				camera.Camera.SetViewportSize(width, height);
			}

		}
	}

	void Scene::SetName(const std::string& name)
	{
		m_Name = name;
	}

	std::string& Scene::GetName()
	{
		return m_Name;
	}

	Ref<Scene>& Scene::LoadScene(const std::string& path)
	{
		Ref<Scene> scene = CreateRef<Scene>();
		SceneSerializer serializer(scene);
		serializer.DeserializeText(path);
		return scene;
	}

	Ref<Scene>& Scene::SampleScene()
	{
		Ref<Scene> scene = CreateRef<Scene>();

		//auto& camera = scene->CreateEntity("Main Camera");
		//camera.AddComponent<CameraComponent>();

		//auto& square = scene->CreateEntity("Blue Square");
		//square.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.2f, 0.8f, 1.0f));

		return scene;
	}

}