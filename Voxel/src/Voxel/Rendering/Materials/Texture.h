#pragma once

#include "Voxel/Core/Core.h"
#include <string>

#include <glm/glm.hpp>

namespace Voxel {

	enum class TextureWrapMode
	{
		REPEAT = 0,
		CLAMP,
		MIRROR
	};

	enum class TextureFilterMode
	{
		POINT = 0,
		BILLINEAR,
		TRILLINEAR
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual glm::vec2& GetScale() = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void SetScale(const glm::vec2& scale) = 0;

		virtual void SetWrapMode(const TextureWrapMode& wrapMode) const = 0;
		virtual void SetFilterMode(const TextureFilterMode& filterMode) const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool operator==(const Texture& other) const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
	};

}
