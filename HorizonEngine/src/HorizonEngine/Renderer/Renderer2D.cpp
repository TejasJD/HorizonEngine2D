#include "pch.h"
#include "Renderer2D.h"

namespace Hzn
{
	struct Renderer2DData
	{
		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> defaultTexture;
	};

	bool Renderer2D::m_Initialized = false;

	static std::shared_ptr<Renderer2DData> s_Data = std::make_shared<Renderer2DData>();

	void Renderer2D::init()
	{
		s_Data->vertexArray = Hzn::VertexArray::create();
		s_Data->shader = Hzn::Shader::create({
			{Hzn::ShaderType::VertexShader, "assets/shaders/Vertex.glsl"},
			{Hzn::ShaderType::FragmentShader, "assets/shaders/Fragment.glsl"}
		});

		std::vector<float> vertices =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};


		std::vector<uint32_t> indices =
		{
			0, 1, 2, // lower-right triangle
			0, 3, 2 // upper-left triangle
		};
		
		auto vertexBuffer = VertexBuffer::create(vertices);
		auto elementBuffer = ElementBuffer::create(indices);

		BufferLayout layout =
		{
			{Hzn::ShaderDataType::Vec3f, "a_Pos"}
		};

		vertexBuffer->setBufferLayout(layout);
		
		s_Data->vertexArray->addVertexBuffer(vertexBuffer);
		s_Data->vertexArray->setElementBuffer(elementBuffer);

		m_Initialized = true;
	}

	void Renderer2D::destroy()
	{
		m_Initialized = false;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		checkInitialized();
		s_Data->shader->bind();
		s_Data->shader->setUniform("view", camera.getViewMatrix());
		s_Data->shader->setUniform("projection", camera.getProjectionMatrix());
	}

	void Renderer2D::endScene()
	{

	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color)
	{
		checkInitialized();
		drawQuad(glm::vec3(position, 1.0f), size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		checkInitialized();
		s_Data->shader->setUniform("a_Color", color);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
		s_Data->shader->setUniform("model", model);
		RenderCall::drawElements(s_Data->vertexArray);
	}

}