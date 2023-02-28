#pragma once

#ifndef HZN_BUFFER_H
#define HZN_BUFFER_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace Hzn
{
	/// <summary>
	/// Represents different data types used by the Shader Programs.
	/// </summary>
	enum class ShaderDataType : uint8_t
	{
		None = 0,
		Float, Vec2f, Vec3f, Vec4f,
		Mat3f, Mat4f, Mat3i, Mat4i,
		Int, Vec2i, Vec3i, Vec4i,
		Bool
	};

	/// <summary>
	/// Get the size of the 'ShaderDataType' in bytes.
	/// </summary>
	/// <param name="type"> 'ShaderDataType'.</param>
	/// <returns>Size of the 'ShaderDataType' in bytes.</returns>
	static unsigned int ShaderDataByteSize(ShaderDataType type)
	{

		switch (type)
		{
		case ShaderDataType::None:  return	0U;
		case ShaderDataType::Bool:  return  sizeof(bool);
		case ShaderDataType::Float:  return  sizeof(float);
		case ShaderDataType::Vec2f: return  sizeof(float) * 2U;
		case ShaderDataType::Vec3f: return  sizeof(float) * 3U;
		case ShaderDataType::Vec4f: return  sizeof(float) * 4U;
		case ShaderDataType::Int:  return  sizeof(int);
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

	/// <summary>
	/// Represents a Buffer Element. Holds information about the Attribute/Elements in a Vertex. Buffer Element
	/// is made up of the name that would be used to represent the Buffer Element of a Vertex in Shader,
	/// the 'ShaderDataType' used to represent the element, size of the 'ShaderDataType', offset of the Buffer Element, which
	/// is the starting position of the element in a Vertex and a bool value that indicates 
	/// if the values of the element would be normalized.
	/// </summary>
	struct BufferElements
	{
		std::string m_Name;
		ShaderDataType m_Type;
		unsigned int m_ByteSize = 0U;
		unsigned int m_Offset = 0U;
		bool m_Normalized;

		/// <summary>
		/// Create a Buffer Element.
		/// </summary>
		/// <param name="type"> 'ShaderDataType' of the Buffer Element.</param>
		/// <param name="name"> Name to represent the Buffer Element in the Shader Program.</param>
		/// <param name="normalized"> Boolean to normalize the values that would be represented by the Buffer Element.</param>
		BufferElements(const ShaderDataType& type, const std::string& name, bool normalized = false)
			: m_Name(name), m_Type(type), m_ByteSize(ShaderDataByteSize(type)), m_Normalized(normalized) {}

		/// <summary>
		/// Get the number of values (usually int, floats or a bool) that are used to represent one Buffer Element.
		/// It is represented by the number of elements in the 'ShaderDataType' held by the Buffer Element.
		/// Example: Float, Int or Bool would return 1, while a Vector of 3 Floats or Ints would return 3.
		/// </summary>
		/// <returns>Number of Values that represent the Buffer Element.</returns>
		uint32_t getCount() const
		{
			switch (m_Type)
			{
			case ShaderDataType::None:  return	0U;
			case ShaderDataType::Bool:  return  1U;
			case ShaderDataType::Float: return  1U;
			case ShaderDataType::Vec2f: return  2U;
			case ShaderDataType::Vec3f: return  3U;
			case ShaderDataType::Vec4f: return  4U;
			case ShaderDataType::Int:   return  1U;
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

	/// <summary>
	/// Define the layout of a Vertex Buffer using the Buffer Layout. Buffer Layouts are
	/// required because Vertex Buffers are essentially an array of primitive data (usually Float).
	/// We need to tell the GPU, how should it interpret this data. Hence, the Buffer Layouts define to the GPU
	/// how to interpret the data held by the Vertex Buffer. A Buffer Layout is represented by the list of
	/// Buffer Elements. It is important to note that the Layout is interpreted in the order by which the user
	/// has added Buffer Elements to the Layout.
	/// </summary>
	class BufferLayout
	{
	public:
		using BufferLayoutIterator = std::vector<BufferElements>::iterator;
		using BufferLayoutConstIterator = std::vector<BufferElements>::const_iterator;
		using BufferLayoutRevIterator = std::vector<BufferElements>::reverse_iterator;
		using BufferLayoutConstRevIterator = std::vector<BufferElements>::const_reverse_iterator;
		
		/// <summary>
		/// Creates empy Buffer Layout.
		/// </summary>
		BufferLayout() = default;

		/// <summary>
		/// Create a Buffer Layout from the list of Buffer Elements.
		/// </summary>
		/// <param name="layout">List of Buffer Elements.</param>
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

		/// <summary>
		/// Get a list of Buffer Elements in the Buffer Layout.
		/// </summary>
		/// <returns>Returns a vector of Buffer Elements. </returns>
		const std::vector<BufferElements>& getElements() const { return m_Layout; }
		
		/// <summary>
		/// Stride represents the distance between any particular Buffer Element of
		/// two consecutive vertices in a Vertex Buffer. This distance is represented by the bytes between them.
		/// </summary>
		/// <returns>An Int that represents the Stride of Buffer Layout in bytes.</returns>
		uint32_t getStride() const { return m_Stride; }

	private:
		std::vector<BufferElements> m_Layout;
		uint32_t m_Stride = 0;
	};

	/// <summary>
	/// Vertex Buffer is essentially a list of vertices. Each Vertex has Attributes such as position
	/// (where it would be drawn on the screen) and many more Attributes such as the color of the Vertex,
	/// Texture Coordinate that maps to the Vertex, etc. But the data held by a Vertex Buffer is essentially
	/// just a list of floats. We use a Buffer Layout to define how this array of floats should be interpreted
	/// by the Shaders and the Graphics Card to use and perform operations based on the data provided to the
	/// Vertex Buffer.
	/// </summary>
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		/// <summary>
		/// Bind the vertex buffer. Only one Vertex Buffer can be bound at a time.
		/// If you bind a different Vertex Buffer through your program, the Vertex Buffer that was bound befored
		/// is unbound.
		/// </summary>
		virtual void bind() const = 0;

		/// <summary>
		/// Unbind the Vertex Buffer.
		/// </summary>
		virtual void unbind() const = 0;

		/// <summary>
		/// Define the layout of the Vertex Buffer. Since a Vertex Buffer is just an array of floats. A layout
		/// is required to determine how to interpret the data held by the Vertex Buffer. This is used by the
		/// Shaders and the Graphics Card to use and perform operations based on the data provided to the
		/// Vertex Buffer.
		/// </summary>
		/// <param name="layout">Buffer Layout object to define the layout of the Vertex Buffer.</param>
		virtual void setBufferLayout(const BufferLayout& layout) = 0;

		/// <summary>
		/// Get the layout of the Vertex Buffer.
		/// </summary>
		/// <returns>Buffer Layout object</returns>
		virtual BufferLayout getBufferLayout() const = 0;

		/// <summary>
		/// set the data of the Vertex Buffer object.
		/// </summary>
		/// <param name="data">Array of Vertices (which can either be an array of primitive scalar / compound types (int, floats, pointers)
		/// or an aggregate/literal class that can represented by primitve scalar / compound types
		/// (int, float pointers)), or a pointer to the starting value of the array.</param>
		/// <param name="size">Size of the array in bytes (number of vertices * (size of each vertex)).</param>
		virtual void setData(void* data, uint32_t size) = 0;

		/// <summary>
		/// Create a Vertex Buffer object, and set the data upon creation.
		/// Use this when the data is known and available during the creation of the object.
		/// </summary>
		/// <param name="vertices">Array of Vertices (which can either be an array of primitive scalar / compound types (int, floats, pointers)
		/// or an aggregate/literal class that can represented by primitve scalar / compound types
		/// (int, float pointers)), or a pointer to the starting value of the array.</param>
		/// <param name="size">Size of the buffer in bytes (number of vertices * size of each vertex).</param>
		/// <returns>A shared pointer to the Vertex Buffer object.</returns>
		static std::shared_ptr<VertexBuffer> create(float* vertices, uint32_t size);

		/// <summary>
		/// Create a Vertex Buffer object, without setting the data upon creation.
		/// Use this when the data is not known during the creation of the object.
		/// In this case the size you provide must be the size you deem to be the maximum size
		/// of data you may set later in the program.
		/// </summary>
		/// <param name="size">Size of the buffer in bytes (number of vertices * size of each vertex).</param>
		/// <returns>A shared pointer to the Vertex Buffer object</returns>
		static std::shared_ptr<VertexBuffer> create(uint32_t size);
	};

	/// <summary>
	/// An Element Buffer represents the order in which, the vertices from a Vertex Buffer should be used
	/// to draw the vertices. Each Element in an Element Buffer is an Index that represents position of
	/// some Vertex in the Vertex Buffer. When Renderer makes a call to draw the elements (or drawElements method specifically),
	/// it uses the elements from the Element Buffer (which are positions of vertices in the Vertex Buffer) to draw the vertices
	/// in the Vertex Buffer in a specific order (values from left to the right in Element Buffer), and based on the type of
	/// 'Primitive' provided.
	/// </summary>
	class ElementBuffer
	{
	public:
		virtual ~ElementBuffer() = default;
		/// <summary>
		/// Bind an Element Buffer. Only one Element Buffer can be bound at a time. 
		/// When called, it unbinds the previously bound Element Buffer if any and binds this Element Buffer object.
		/// </summary>
		virtual void bind() const = 0;

		/// <summary>
		/// Unbind the Element Buffer object.
		/// </summary>
		virtual void unbind() const = 0;

		/// <summary>
		/// Get the number of elements in the Element Buffer.
		/// </summary>
		/// <returns></returns>
		virtual uint32_t size() const = 0;

		/// <summary>
		/// Create an Element Buffer object.
		/// </summary>
		/// <param name="indices">Array of indices (ints). Or a pointer to starting value of the array.</param>
		/// <param name="count">Number of elements in the Array.</param>
		/// <returns></returns>
		static std::shared_ptr<ElementBuffer> create(uint32_t* indices, uint32_t count);
	};
}

#endif // !HZN_VERTEX_BUFFER_H
