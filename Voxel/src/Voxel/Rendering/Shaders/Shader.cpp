#include <pch.h>
#include "Shader.h"

#include "Voxel/Rendering/Renderer.h"

#include "Platform/OpenGL/Shaders/OpenGLShader.h"

namespace Voxel {

	Ref<Shader> Shader::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::OpenGL:
				return CreateRef<OpenGLShader>(path);
		}

		CORE_ASSERT(false, "Unknown Render API: {0}", Renderer::GetAPI());
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{

		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::OpenGL:
				return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		CORE_ASSERT(false, "Unknown Render API: {0}", Renderer::GetAPI());
		return nullptr;
	}

	void ShaderLibrary::AddShader(const std::string& name, const Ref<Shader>& shader)
	{
		CORE_ASSERT(!HasShader(name), "Shader " + name + " already exists");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::AddShader(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		AddShader(name, shader);
	}


	Ref<Shader> ShaderLibrary::LoadShader(const std::string& path)
	{
		auto shader = Shader::Create(path);
		AddShader(shader);

		return shader;
	}


	Ref<Shader> ShaderLibrary::LoadShader(const std::string& path, const std::string& name)
	{
		auto shader = Shader::Create(path);
		AddShader(shader);

		return shader;
	}


	Ref<Shader> ShaderLibrary::GetShader(const std::string& name)
	{
		CORE_ASSERT(HasShader(name), "Shader " + name + " not found");

		return m_Shaders[name];
	}

	bool  ShaderLibrary::HasShader(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}