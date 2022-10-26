#pragma once

#ifndef HZN_BUFFER_H
#define HZN_BUFFER_H

namespace Hzn
{
	enum class ShaderDataType : unsigned char
	{
		None = 0,
		Vecf, Vec2f, Vec3f, Vec4f,
		Mat3f, Mat4f, Mat3i, Mat4i,
		Veci, Vec2i, Vec3i, Vec4i,
		Bool
	};

	static unsigned int ShaderDataByteSize(ShaderDataType type)
	{

		switch (type)
		{
		case ShaderDataType::None:  return	0U;
		case ShaderDataType::Bool:  return  sizeof(bool);
		case ShaderDataType::Vecf:  return  sizeof(float);
		case ShaderDataType::Vec2f: return  sizeof(float) * 2U;
		case ShaderDataType::Vec3f: return  sizeof(float) * 3U;
		case ShaderDataType::Vec4f: return  sizeof(float) * 4U;
		case ShaderDataType::Veci:  return  sizeof(int);
		case ShaderDataType::Vec2i: return  sizeof(int) * 2U;
		case ShaderDataType::Vec3i: return  sizeof(int) * 3U;
		case ShaderDataType::Vec4i: return  sizeof(int) * 4U;
		case ShaderDataType::Mat3f: return  sizeof(float) * 3U * 3U;
		case ShaderDataType::Mat3i: return  sizeof(int) * 3U * 3U;
		case ShaderDataType::Mat4f: return  sizeof(float) * 4U * 4U;
		case ShaderDataType::Mat4i: return  sizeof(int) * 3U * 3U;
		}

		HZN_CORE_ASSERT(false, "Invalid Shader Data Type!");
		return 0U;
	}

	//! Represents an Attribute of a vertex in the Buffer.
	struct BufferElements
	{
		std::string m_Name;
		ShaderDataType m_Type;
		unsigned int m_ByteSize = 0U;
		unsigned int m_Offset = 0U;
		bool m_Normalized;

		BufferElements(const ShaderDataType& type, const std::string& name, bool normalized = false)
			: m_Name(name), m_Type(type), m_ByteSize(ShaderDataByteSize(type)), m_Normalized(normalized) {}

		unsigned int getCount() const
		{
			switch (m_Type)
			{
			case ShaderDataType::None:  return	0U;
			case ShaderDataType::Bool:  return  1U;
			case ShaderDataType::Vecf:  return  1U;
			case ShaderDataType::Vec2f: return  2U;
			case ShaderDataType::Vec3f: return  3U;
			case ShaderDataType::Vec4f: return  4U;
			case ShaderDataType::Veci:  return  1U;
			case ShaderDataType::Vec2i: return  2U;
			case ShaderDataType::Vec3i: return  3U;
			case ShaderDataType::Vec4i: return  4U;
			case ShaderDataType::Mat3f: return  3U * 3U;
			case ShaderDataType::Mat3i: return  3U * 3U;
			case ShaderDataType::Mat4f: return  4U * 4U;
			case ShaderDataType::Mat4i: return  3U * 3U;
			}
			HZN_CORE_ASSERT(false, "Invalid Shader Data Type!");
			return 0U;
		}
	};

	//! Create of layout for your vertex buffer which would be used by your shader.
	class BufferLayout
	{
	public:
		using BufferLayoutIterator = std::vector<BufferElements>::iterator;
		using BufferLayoutConstIterator = std::vector<BufferElements>::const_iterator;
		using BufferLayoutRevIterator = std::vector<BufferElements>::reverse_iterator;
		using BufferLayoutConstRevIterator = std::vector<BufferElements>::const_reverse_iterator;
		//! empty buffer layout
		BufferLayout() = default;

		//! create buffer layout with initializer lists. 
		BufferLayout(const std::initializer_list<BufferElements>& layout)
			: m_Layout(layout)
		{
			for (auto& element : m_Layout)
			{
				element.m_Offset = m_Stride;
				m_Stride += ShaderDataByteSize(element.m_Type);
			}
		}

		BufferLayoutIterator begin() { return m_Layout.begin(); }
		BufferLayoutIterator end() { return m_Layout.end(); }

		BufferLayoutConstIterator begin() const { return m_Layout.begin(); }
		BufferLayoutConstIterator end() const { return m_Layout.end(); }

		BufferLayoutConstIterator cbegin() const { return m_Layout.begin(); }
		BufferLayoutConstIterator cend() const { return m_Layout.end(); }

		BufferLayoutRevIterator rbegin() { return m_Layout.rbegin(); }
		BufferLayoutRevIterator rend() { return m_Layout.rend(); }

		BufferLayoutConstRevIterator rbegin() const { return m_Layout.rbegin(); }
		BufferLayoutConstRevIterator rend() const { return m_Layout.rend(); }

		BufferLayoutConstRevIterator crbegin() const { return m_Layout.rbegin(); }
		BufferLayoutConstRevIterator crend() const { return m_Layout.rend(); }

		//! returns a list of Buffer Elements in your layout.
		const std::vector<BufferElements>& getElements() const { return m_Layout; }
		//! returns what would be the difference between any attribute of two consecutive vertices in your
		//! vertex Buffer.
		unsigned int getStride() const { return m_Stride; }

	private:
		std::vector<BufferElements> m_Layout;
		unsigned int m_Stride = 0;
	};

	//! Interface for implementing concrete Vertex Buffers.
	//! Concrete implementation is bound at the runtime.
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void setBufferLayout(const BufferLayout& layout) = 0;
		virtual BufferLayout getBufferLayout() const = 0;

		static std::shared_ptr<VertexBuffer> create(std::vector<float> vertices);

	protected:
		VertexBuffer() {}

	};

	//! Interface for implementing concrete Element Buffers.
	//! Concrete implementation is bound at the runtime.
	class ElementBuffer
	{
	public:
		virtual ~ElementBuffer() {};
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual unsigned int size() const = 0;

		static std::shared_ptr<ElementBuffer> create(std::vector<unsigned int> indices);

	protected:
		ElementBuffer() {}

	};
}

#endif // !HZN_VERTEX_BUFFER_H
