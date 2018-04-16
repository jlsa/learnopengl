#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

#include <stdexcept>


#include "stb_image.h"
#include "Shader.h"
#include "Application.h"


/*
// OKay, how does this work inside a class.. #TODO come back to this
// currently fixed by function outside of class
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}*/



Application::Application(std::string appTitle)
	: _appTitle(appTitle), _windowWidth(800), _windowHeight(600)
{
	// should give error if it fails to load anything
	// do nothing yet
	_isRunning = false;
	//_appTitle = appTitle;
	//_windowWidth = 800;
	//_windowHeight = 600;

	bool initialized = Init();
}

Application::Application(std::string appTitle, int windowWidth, int windowHeight)
	: _appTitle(appTitle),
	_windowWidth(windowWidth), _windowHeight(windowHeight)
{
	_isRunning = false;
	//_appTitle = appTitle;
	//_windowWidth = windowWidth;
	//_windowHeight = windowHeight;

	bool initialized = Init();
}


Application::~Application()
{
	// call destroy here
	Cleanup();
}

void Application::Start()
{
	if (_isRunning) {
		// do nothing
	}
	else {
		_isRunning = true;
	}
}

void Application::Stop()
{
	// currently just sets _isRunning to false.
	_isRunning = false;
}

void Application::Cleanup()
{
	// clean up every object created
	glfwTerminate();
}

void Application::Render(float deltaTime)
{
	// do all the rendering here
}

void Application::Update(float deltaTime)
{
	// do all the updates here
}

GLFWwindow *Application::GetWindow()
{
	return _window;
}

bool Application::Init()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	_window = glfwCreateWindow(_windowWidth, _windowHeight, _appTitle.c_str(), NULL, NULL);
	if (_window == NULL)
	{
		std::string message = "Failed to create GLFW window";
		glfwTerminate();
		//return -1;
		throw std::runtime_error(message);
		return false;
	}
	glfwMakeContextCurrent(_window);
	
	//glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::string message = "Failed to initialize GLAD";
		throw std::runtime_error(message);
		//return -1;
		return false;
	}

	return true;
}

void Application::Tick(float deltaTime)
{
	Update(deltaTime);
	Render(deltaTime);
}

/*
void Application::ProcessInput(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
*/

/*
void Application::HandleResizeEvent(GLFWwindow* window, int w, int h)
{
	std::cout << "Resizing..." << std::endl;
	glViewport(0, 0, w, h);
}
*/