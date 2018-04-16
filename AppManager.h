#pragma once

#include <memory>
#include <glad/glad.h>
#include <glfw3.h>

#include "Application.h"


template<typename T, typename... ARGS, typename = typename std::enable_if<std::is_base_of<Application, T>::value>::type>
std::unique_ptr<T> MakeApp(ARGS&&... args)
{
	std::unique_ptr<T> app{ new T{ std::forward<ARGS>(args)...} };
	AppManager::StartApp(app.get());

	return app;
}

class AppManager
{
	static Application* _app;

	static void HandleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (_app) {
			std::cout << key << ", " << scancode << ", " << action << ", " << mods << std::endl;
			_app->HandleKeyboardInput(window, key, scancode, action, mods); // #TODO add input class to application
		}
	}

	static void HandleMouseInput(GLFWwindow* window, double xpos, double ypos)
	{
		if (_app) {
			std::cout << "Cursor is at (x: " << xpos << ", y:" << ypos << ")" << std::endl;
			_app->HandleMouseInput(window, xpos, ypos);
		}
	}

	static void HandleError(int error, const char* desc)
	{
		if (_app) {
			_app->HandleError(error, desc); // #TODO add error handling
		}
	}

	static void HandleResize(GLFWwindow* window, int width, int height)
	{
		if (_app) {
			//std::cout << "ResizeEventCalled { width: '" << width << "', height: '" << height << "' }" << std::endl;
			// create window resize event
			//WindowResizeEvent event = new WindowResizeEvent(window, width, height);
			//_app.GetEventManager().HandleEvent(Event::Window::Resize, ); // #TODO create event manager
			_app->HandleResizeEvent(window, width, height);
		}
	}

public:
	static void StartApp(Application* app)
	{
		_app = app;

		glfwSetFramebufferSizeCallback(app->GetWindow(), HandleResize);
		glfwSetErrorCallback(HandleError);

		glfwSetKeyCallback(app->GetWindow(), HandleKeyboardInput);
		glfwSetCursorPosCallback(app->GetWindow(), HandleMouseInput);
	}
};

