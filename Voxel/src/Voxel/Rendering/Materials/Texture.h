#pragma once

#include "Voxel/Core/Core.h"
#include <string>

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

		virtual void SetWrapMode(const TextureWrapMode& wrapMode) const = 0;
		virtual void SetFilterMode(const TextureFilterMode& filterMode) const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);

	};

}
