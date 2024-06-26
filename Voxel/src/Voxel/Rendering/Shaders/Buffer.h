#pragma once

namespace Voxel {

	enum class ShaderDataType : uint8_t
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3x3, Mat4x4,
		Int, Int2, Int3, Int4,
		Bool,
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:  return sizeof(float);
			case ShaderDataType::Float2: return 2 * sizeof(float);
			case ShaderDataType::Float3: return 3 * sizeof(float);
			case ShaderDataType::Float4: return 4 * sizeof(float);
			case ShaderDataType::Mat3x3: return 3 * 3 * sizeof(float);
			case ShaderDataType::Mat4x4: return 4 * 4 * sizeof(float);
			case ShaderDataType::Int:    return sizeof(int);
			case ShaderDataType::Int2:   return 2 * sizeof(int);
			case ShaderDataType::Int3:   return 3 * sizeof(int);
			case ShaderDataType::Int4:   return 4 * sizeof(int);
			case ShaderDataType::Bool:   return sizeof(bool);
		}

		CORE_ASSERT(false, "Unknown ShaderDataType: {0}", type);
		return 0;
	}

	struct BufferElement
	{

		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:  return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Mat3x3: return 3 * 3;
			case ShaderDataType::Mat4x4: return 4 * 4;
			case ShaderDataType::Int:    return 1;
			case ShaderDataType::Int2:   return 2;
			case ShaderDataType::Int3:   return 3;
			case ShaderDataType::Int4:   return 4;
			case ShaderDataType::Bool:   return 1;
			}
			CORE_ASSERT(false, "Unknown ShaderDataType: {0}", Type);
			return 0;
		}

	};

	class BufferLayout
	{
	public:

		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{

			CalculateOffsetsAndStride();

		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual void UploadData(const void* data, uint32_t size) = 0;

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
		static Ref<VertexBuffer> Create(uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t size);
	};

}