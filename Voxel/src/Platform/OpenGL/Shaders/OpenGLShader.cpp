#include <pch.h>
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <fstream>
#include <array>
#include <glm/gtc/type_ptr.hpp>

namespace Voxel {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		CORE_ASSERT(false, "Unknown shader type: " + type);
		return GL_ZERO;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;

		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::filesystem::path& path)
	{
		std::string source = ReadFile(path);
		auto shaderSources = Preprocess(source);
		Compile(shaderSources);

		m_Name = path.stem().string(); // Returns the file's name stripped of the extension.

	}

	std::string OpenGLShader::ReadFile(const std::filesystem::path& path)
	{
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

			CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type: " + type);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();

		CORE_ASSERT((bool)(shaderSources.size() <= 2), "Shader '" + m_Name + "' exceeds the maximum of 2 programs");

		std::array<GLuint, 2> glShaders;
		int glShaderIndex = 0;

		for (auto& kv : shaderSources)
		{

			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* glSource = source.c_str();

			glShaderSource(shader, 1, &glSource, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				LOG_CORE_ERROR("Could not compile shader '{0}': {1}", type, infoLog.data());
				return;
			}

			glAttachShader(program, shader);

			glShaders[glShaderIndex] = shader;
			glShaderIndex += 1;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (auto shader : glShaders)
				glDeleteShader(shader);

			LOG_CORE_ERROR("Could not link shaders: {0}", infoLog.data());
			return;
		}

		for (auto shader : glShaders)
			glDetachShader(program, shader);

		m_RendererID = program;
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
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
