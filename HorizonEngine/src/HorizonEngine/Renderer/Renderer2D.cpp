#include "pch.h"
#include "Renderer2D.h"

#include "FrameBuffer.h"

namespace Hzn
{
	bool Renderer2D::s_Initialized = false;
	Renderer2DStats Renderer2D::s_Stats;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texSlot;
	};

	struct RenderData
	{
		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<ElementBuffer> ebo;
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture2D> defaultTexture;

		static constexpr uint32_t mxquads = 1000;
		static constexpr uint32_t mxvertices = 4 * mxquads;
		static constexpr uint32_t mxindices = 6 * mxquads;
		static constexpr uint32_t mxtextureSlots = 32;

		Vertex* ptr = nullptr;
		uint32_t curidx = 0;
		// 0 is bound to default texture.
		uint32_t textureidx = 1;
		uint32_t draws = 0;
		uint32_t quads = 0;

		Vertex* buffer = nullptr;
		// each texture is bound to one of the texture slots in this.
		std::array<std::shared_ptr<Texture2D>, mxtextureSlots> textureSlots; // TODO: have a UUID that represent assets.
		std::array<glm::vec4, 4> quadPositions =
		{
			glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
			glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f),
			glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),
			glm::vec4(0.5f, -0.5f, 0.0f, 1.0f)
		};
		std::array<glm::vec2, 4> quadTexCoords =
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f)
		};
	};

	constexpr uint32_t RenderData::mxtextureSlots;
	constexpr uint32_t RenderData::mxquads;
	constexpr uint32_t RenderData::mxvertices;
	constexpr uint32_t RenderData::mxindices;

	// static because we don't want to expose data used by the 2D renderer to other
	// other translation units.
	static RenderData data;

	void Renderer2D::init()
	{
		// allocate data for buffer.
		data.ptr = data.buffer = new Vertex[data.mxvertices];

		// shader creation.
		data.shader = Shader::create({
			{Hzn::ShaderType::VertexShader, "api_assets/shaders/TextureVertex.glsl"},
			{Hzn::ShaderType::FragmentShader, "api_assets/shaders/TextureFragment.glsl"}
			});

		// default-texture creation.
		uint32_t whiteTexture = 0xffffffff;
		data.defaultTexture = Texture2D::create(1, 1);
		data.defaultTexture->setData(&whiteTexture, sizeof(uint32_t));
		// bind default texture to slot 0.
		data.textureSlots[0] = data.defaultTexture;

		// this array of samplers is set as uniform for textures.
		// it holds a value (index) which tells GPU, which texture slot to sample the texture from.
		std::array<int32_t, data.mxtextureSlots> samplerArray = {};
		for (int32_t i = 0; i < data.mxtextureSlots; ++i) samplerArray[i] = i; // store the slot value (index) in the sampler array.
		data.shader->bind();
		data.shader->setUniform("u_Textures", samplerArray.data(), data.mxtextureSlots);

		data.vao = VertexArray::create();

		data.vbo = VertexBuffer::create(data.mxvertices * sizeof(Vertex));

		BufferLayout layout =
		{
			{Hzn::ShaderDataType::Vec3f, "a_Pos"},
			{Hzn::ShaderDataType::Vec4f, "a_Color"},
			{Hzn::ShaderDataType::Vec2f, "a_TexCoord"},
			{Hzn::ShaderDataType::Float, "a_TexSlot"}
		};

		data.vbo->setBufferLayout(layout);

		/*data.vbo->setData(vertices, sizeof(vertices));*/

		data.vao->addVertexBuffer(data.vbo);

		uint32_t* indices = new uint32_t[data.mxindices];

		uint32_t offset = 0;
		for (int i = 0; i < data.mxindices; i += 6)
		{
			indices[i] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;
			indices[i + 3] = 0 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 2 + offset;

			offset += 4;
		}

		data.ebo = ElementBuffer::create(indices, data.mxindices);
		data.vao->setElementBuffer(data.ebo);
		delete[] indices;
	}

	void Renderer2D::destroy()
	{
		data.ptr = nullptr;
		delete[] data.buffer;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		data.shader->bind();
		data.shader->setUniform("u_Projection", camera.getProjectionMatrix());
		data.shader->setUniform("u_View", camera.getViewMatrix());

		// initialize few stat variables.
		data.draws = 0;
		data.quads = 0;

		beginBatch();
	}

	void Renderer2D::beginScene(const SceneCamera2D& camera, const glm::mat4& transform)
	{
		data.shader->bind();
		data.shader->setUniform("u_Projection", camera.getProjectionMatrix());
		data.shader->setUniform("u_View", glm::inverse(transform));

		data.draws = 0;
		data.quads = 0;

		beginBatch();
	}

	void Renderer2D::beginBatch()
	{
		data.ptr = data.buffer;
		data.curidx = 0;
		data.textureidx = 1;
	}

	void Renderer2D::endScene()
	{
		endBatch();

		// record stats.
		Renderer2D::s_Stats.draws = data.draws;
		Renderer2D::s_Stats.quads = data.quads;
		Renderer2D::s_Stats.vertices = 4 * data.quads;
		Renderer2D::s_Stats.indices = 6 * data.quads;
	}

	void Renderer2D::submitBatch()
	{
		// bind the textures we have in our array to the texture units in GPU before we draw.
		for (int i = 0; i < data.textureidx; ++i)
		{
			data.textureSlots[i]->bind(i);
		}

		RenderCall::drawElements(data.vao, data.curidx);
		data.draws++;
	}

	void Renderer2D::endBatch()
	{
		uint32_t size = (data.ptr - data.buffer) * sizeof(Vertex);
		data.vao->bind();
		data.vbo->setData(data.buffer, size);
		submitBatch();
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		constexpr float textureIndex = 0.0f;

		if (data.curidx >= Hzn::RenderData::mxindices)
		{
			endBatch();
			beginBatch();
		}

		// build up a transformation matrix and apply those transformations to
		// all the vertex positions.

		for (int i = 0; i < data.quadPositions.size(); ++i)
		{
			data.ptr->position = transform * data.quadPositions[i];
			data.ptr->color = color;
			data.ptr->texCoord = data.quadTexCoords[i];
			data.ptr->texSlot = textureIndex;
			data.ptr++;
		}

		data.curidx += 6;
		data.quads++;
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color)
	{
		if (data.curidx >= data.mxindices)
		{
			endBatch();
			beginBatch();
		}
		// texture slot that the quad will be using.
		float textureIndex = 0.0f;

		// if this texture is already one of the bound textures, we just use that texture slot for
		// this quad.
		for (int i = 0; i < data.textureidx; ++i)
		{
			if (data.textureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		// if this is found to be a new texture, we bind it to the available slot and increment
		// the slot pointer.
		if (textureIndex == 0.0f)
		{
			textureIndex = data.textureidx;
			data.textureSlots[data.textureidx++] = texture;
		}


		for (int i = 0; i < data.quadPositions.size(); ++i)
		{
			data.ptr->position = transform * data.quadPositions[i];
			data.ptr->color = color;
			data.ptr->texCoord = data.quadTexCoords[i];
			data.ptr->texSlot = textureIndex;
			data.ptr++;
		}

		data.curidx += 6;
		data.quads++;
	}

	void Renderer2D::drawSprite(const glm::mat4& transform, const std::shared_ptr<Sprite2D>& sprite, const glm::vec4& color)
	{
		if (data.curidx >= data.mxindices)
		{
			endBatch();
			beginBatch();
		}

		// texture slot that the quad will be using.
		float textureIndex = 0.0f;

		// if this texture is already one of the bound textures, we just use that texture slot for
		// this quad.
		for (int i = 0; i < data.textureidx; ++i)
		{
			if (data.textureSlots[i] == sprite->getSpriteSheet())
			{
				textureIndex = (float)i;
				break;
			}
		}

		// if this is found to be a new texture, we bind it to the available slot and increment
		// the slot pointer.
		if (textureIndex == 0.0f)
		{
			textureIndex = data.textureidx;
			data.textureSlots[data.textureidx++] = sprite->getSpriteSheet();
		}

		for (int i = 0; i < data.quadPositions.size(); ++i)
		{
			data.ptr->position = transform * data.quadPositions[i];
			data.ptr->color = color;
			data.ptr->texCoord = sprite->getTexCoords()[i];
			data.ptr->texSlot = textureIndex;
			data.ptr++;
		}

		data.curidx += 6;
		data.quads++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color)
	{
		drawQuad(glm::vec3(position, 0.0f), size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
		drawQuad(transform, color);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, float angle, const glm::vec3& size, const glm::vec4 color)
	{
		drawQuad(glm::vec3(position, 0.0f), angle, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, float angle, const glm::vec3& size, const glm::vec4 color)
	{
		const glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), size);

		drawQuad(transform, color);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec3& size, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color)
	{
		drawQuad(glm::vec3(position, 0.0f), size, texture, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec3& size, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color)
	{
		const glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), size);

		drawQuad(transform, texture, color);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, float angle, const glm::vec3& size, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color)
	{
		drawQuad(glm::vec3(position, 0.0f), angle, size, texture, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, float angle, const glm::vec3& size, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color)
	{
		const glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), size);

		drawQuad(transform, texture, color);
	}

	void Renderer2D::drawSprite(const glm::vec2& position, const glm::vec3& size, const std::shared_ptr<Sprite2D>& sprite, const glm::vec4& color)
	{
		drawSprite(glm::vec3(position, 0.0f), size, sprite, color);
	}

	void Renderer2D::drawSprite(const glm::vec3& position, const glm::vec3& size, const std::shared_ptr<Sprite2D>& sprite, const glm::vec4& color)
	{
		const glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), size);
		drawSprite(transform, sprite, color);
	}

	void Renderer2D::drawSprite(const glm::vec2& position, float angle, const glm::vec3& size, const std::shared_ptr<Sprite2D>& sprite, const glm::vec4& color)
	{
		drawSprite(glm::vec3(position, 0.0f), angle, size, sprite, color);
	}

	void Renderer2D::drawSprite(const glm::vec3& position, float angle, const glm::vec3& size, const std::shared_ptr<Sprite2D>& sprite, const glm::vec4& color)
	{
		const glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), size);

		drawSprite(transform, sprite, color);
	}
}

