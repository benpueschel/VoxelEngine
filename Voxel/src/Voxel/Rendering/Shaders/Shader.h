#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Voxel {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, const int value) = 0;
		virtual void SetIntArray(const std::string& name, int* value, uint32_t count) = 0;

		virtual void SetFloat(const std::string& name, const float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& path);
	};

	class ShaderLibrary
	{
	public:

		void AddShader(const Ref<Shader>& shader);
		void AddShader(const std::string& name, const Ref<Shader>& shader);

		Ref<Shader> LoadShader(const std::string& path);
		Ref<Shader> LoadShader(const std::string& path, const std::string& name);

		Ref<Shader> GetShader(const std::string& name);

		bool HasShader(const std::string& name) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}