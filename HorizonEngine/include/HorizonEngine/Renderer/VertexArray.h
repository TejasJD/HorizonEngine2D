#pragma once

#ifndef HZN_VERTEX_ARRAY_H
#define HZN_VERTEX_ARRAY_H

#include <memory>
#include <vector>

#include "HorizonEngine/Renderer/Buffer.h"

namespace Hzn
{
	/// <summary>
	/// Represents a Vertex Array. Used to save the type of Vertex Buffers and the Element Buffer, that
	/// needs to be bound before drawing. Binding the vertex array in every iteration of the application loop
	/// reduces the need to bind the Vertex Buffers, Element Buffer and attribute pointers to those vertex buffers
	/// individually per iteration of the application loop.
	/// </summary>
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;
		
		/// <summary>
		/// Bind the Vertex Array object.
		/// </summary>
		virtual void bind() const = 0;

		/// <summary>
		/// Unbind the Vertex Array object.
		/// </summary>
		virtual void unbind() const = 0;

		/// <summary>
		/// Add vertex buffers to the Vertex Array. A Vertex Array maintain attribute pointers, that
		/// point to different attributes in different Vertex Buffers. Adding a Vertex Buffer to a Vertex Array
		/// sets the attribute pointers associated with that Vertex Buffer in the Vertex Array.
		/// </summary>
		/// <param name="vertexBuffer"> Shared pointer to the vertex buffer.</param>
		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;

		/// <summary>
		/// Set a pointer to the Element Buffer.
		/// </summary>
		/// <param name="elementBuffer"> Shared pointer to the element buffer.</param>
		virtual void setElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer) = 0;

		/// <summary>
		/// Get the Element Buffer, associated with the Vertex Array.
		/// </summary>
		/// <returns> A shared pointer to the Element Buffer.</returns>
		virtual std::shared_ptr<ElementBuffer> getElementBuffer() = 0;

		/// <summary>
		/// Get a list of Vertex Buffers associated with the Vertex Array.
		/// </summary>
		/// <returns> A vector of shared pointers to the Vertex Buffers.</returns>
		virtual std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() = 0;

		/// <summary>
		/// Create a Vertex Array object.
		/// </summary>
		/// <returns> A shared pointer to the Vertex Array object.</returns>
		static std::shared_ptr<VertexArray> create();
	};
}

#endif // !HZN_VERTEX_ARRAY_H
