#include <pch.h>
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components/Components.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

using namespace YAML;

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec2& value)
		{
			if (!node.IsSequence() || node.size() < 2)
				return false;

			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			node.push_back(value.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& value)
		{
			if (!node.IsSequence() || node.size() < 3)
				return false;

			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			value.z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& value)
		{
			Node node;
			node.push_back(value.x);
			node.push_back(value.y);
			node.push_back(value.z);
			node.push_back(value.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& value)
		{
			if (!node.IsSequence() || node.size() < 4)
				return false;

			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			value.z = node[2].as<float>();
			value.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Voxel {

	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec2& value)
	{
		out << Flow;
		out << BeginSeq << value.x << value.y << EndSeq;
		return out;
	}

	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec3& value)
	{
		out << Flow;
		out << BeginSeq << value.x << value.y << value.z << EndSeq;
		return out;
	}

	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec4& value)
	{
		out << Flow;
		out << BeginSeq << value.x << value.y << value.z << value.w << EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{

	}

	static void SerializeEntity(YAML::Emitter& out, Entity& entity)
	{
		out << BeginMap;
		out << Key << "Entity" << Value << (uint32_t)entity;

		if (entity.HasComponent<TagComponent>())
		{
			out << Key << "TagComponent" << Value << BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << Key << "Tag" << Value << tag;

			out << EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << Key << "TransformComponent" << Value << BeginMap;

			auto& transform = entity.GetComponent<TransformComponent>();
			out << Key << "LocalPosition" << Value << transform.LocalPosition;
			out << Key << "LocalRotation" << Value << transform.LocalRotation;
			out << Key << "LocalScale" << Value << transform.LocalScale;

			out << EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << Key << "CameraComponent" << Value << BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << Key << "Camera" << Value << BeginMap;
			out << Key << "ProjectionType" << Value << (uint32_t)camera.GetProjectionType();
			out << Key << "PerspectiveFOV" << Value << camera.GetPerspectiveFOV();
			out << Key << "PerspectiveClippingPlane" << Value << camera.GetPerspectiveClippingPlane();
			out << Key << "OrthographicClippingPlane" << Value << camera.GetOrthographicClippingPlane();
			out << Key << "OrthographicSize" << Value << camera.GetOrthographicSize();
			out << EndMap;

			out << Key << "Primary" << Value << cameraComponent.Primary;
			out << Key << "FixedAspectRatio" << Value << cameraComponent.FixedAspectRatio;

			out << EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << Key << "SpriteRendererComponent" << Value << BeginMap;

			auto& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
			out << Key << "Color" << Value << spriteRenderer.Color;

			out << EndMap;
		}

		out << EndMap;
	}

	void SceneSerializer::SerializeText(const std::filesystem::path& path)
	{

		YAML::Emitter out;
		out << BeginMap;
		out << Key << "Scene" << Value << m_Scene->GetName();
		out << Key << "Entities" << Value << BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID) {
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});

		out << EndSeq;
		out << EndMap;

		if (path.has_parent_path())
			std::filesystem::create_directories(path.parent_path());

		std::ofstream fout(path);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeBinary(const std::filesystem::path& path)
	{
		CORE_ASSERT(false, "Not implemented");
	}

	bool SceneSerializer::DeserializeText(const std::filesystem::path& path)
	{
		YAML::Node data = YAML::LoadFile(path.string());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		m_Scene->SetName(sceneName);
		LOG_CORE_INFO("Deserializing Scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				LOG_CORE_INFO("Deserialized entity width ID = {0}, name = '{1}'", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetOrAddComponent<TransformComponent>();
					tc.LocalPosition = transformComponent["LocalPosition"].as<glm::vec3>();
					tc.LocalRotation = transformComponent["LocalRotation"].as<glm::vec3>();
					tc.LocalScale = transformComponent["LocalScale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.GetOrAddComponent<CameraComponent>();
					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<uint32_t>());

					cc.Camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveClippingPlane(cameraProps["PerspectiveClippingPlane"].as<glm::vec2>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicClippingPlane(cameraProps["OrthographicClippingPlane"].as<glm::vec2>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.GetOrAddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::filesystem::path& path)
	{
		CORE_ASSERT(false, "Not implemented");
		return false;
	}

}