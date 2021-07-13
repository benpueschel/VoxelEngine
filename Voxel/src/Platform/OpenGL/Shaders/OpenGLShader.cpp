#include <pch.h>
#include "OpenGLShader.h"
#include "OpenGLShaderUtils.h"
#include "Voxel/Core/Timer.h"

#include <glad/glad.h>

#include <fstream>
#include <array>
#include <glm/gtc/type_ptr.hpp>

namespace Voxel {

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;

		CompileOrGetVulkanBinaries(shaderSources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();
	}

	OpenGLShader::OpenGLShader(const std::filesystem::path& path)
		: m_Path(path) 
	{
		PROFILE_FUNCTION();

		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(path);
		auto shaderSources = Preprocess(source);

		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			LOG_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}

		m_Name = path.stem().string(); // Returns the file's name stripped of the extension.

	}

	std::string OpenGLShader::ReadFile(const std::filesystem::path& path)
	{
		PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);

		if (!in)
		{
			LOG_CORE_ERROR("Could not open file '{0}'", path);
			return result;
		}

		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::Preprocess(const std::string& source)
	{
		PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			bool hasLineEnding = eol != std::string::npos;
			CORE_ASSERT(hasLineEnding, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type: " + type);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			CORE_ASSERT((nextLinePos != std::string::npos), "Syntax error!");
			pos = source.find(typeToken, nextLinePos);

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		PROFILE_FUNCTION();

		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path cachedPath = cacheDirectory 
				/ (m_Path.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_Path.string().c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					LOG_CORE_ERROR(module.GetErrorMessage());
					CORE_ASSERT(false, "Could not compile Vulkan shader");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			std::filesystem::path cachedPath = cacheDirectory 
				/ (m_Path.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
					source, 
					Utils::GLShaderStageToShaderC(stage), 
					m_Path.string().c_str()
				);

				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					LOG_CORE_ERROR(module.GetErrorMessage());
					CORE_ASSERT(false, "Could not compile OpenGL shader");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			LOG_CORE_ERROR("Shader linking failed ({0}):\n	{1}", m_Path, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		LOG_CORE_INFO("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_Path);
		LOG_CORE_INFO("    {0} uniform buffers", resources.uniform_buffers.size());
		LOG_CORE_INFO("    {0} resources", resources.sampled_images.size());
		if (resources.uniform_buffers.size()) 
		{
			LOG_CORE_INFO("Uniform buffers:");
			for (const auto& resource : resources.uniform_buffers)
			{
				const auto& bufferType = compiler.get_type(resource.base_type_id);
				uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
				uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
				int memberCount = bufferType.member_types.size();

				LOG_CORE_INFO("  {0}", resource.name);
				LOG_CORE_INFO("    Size = {0}", bufferSize);
				LOG_CORE_INFO("    Binding = {0}", binding);
				LOG_CORE_INFO("    Members = {0}", memberCount);
			}
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float value)
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetInt(const std::string& name, const int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* value, uint32_t count)
	{
		UploadUniformIntArray(name, value, count);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::UploadUniformInt2(const std::string& name, glm::ivec2 value)
	{
		glUniform2i(GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::UploadUniformInt3(const std::string& name, glm::ivec3 value)
	{
		glUniform3i(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformInt4(const std::string& name, glm::ivec4 value)
	{
		glUniform4i(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* value, uint32_t count)
	{
		glUniform1iv(GetUniformLocation(name), count, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		CORE_ASSERT((location != -1), "Could not find uniform location: " + name);
		return location;
	}

}
