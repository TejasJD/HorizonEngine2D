#include "pch.h"
#include "MSWindow.h"

MSWindow::MSWindow(const std::size_t& width, const std::size_t& height, const char* const& title)
	: Window(width, height, title)
{
	init();
}

MSWindow::~MSWindow()
{
	std::cout << "Destructor invoked!" << std::endl;
	destroy();
}

void MSWindow::update()
{
	glfwPollEvents();
}

void MSWindow::render()
{
	glfwSwapBuffers(m_Window);
}

bool MSWindow::shouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void MSWindow::init()
{
	int glfwSuccess = glfwInit();

	assert(glfwSuccess != 0);

	m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title, nullptr, nullptr);

	assert(m_Window != nullptr);

	glfwMakeContextCurrent(m_Window);

	int gladSuccess = gladLoadGL((GLADloadfunc)glfwGetProcAddress);

	assert(gladSuccess != 0);
}

void MSWindow::destroy()
{
	glfwTerminate();
}
