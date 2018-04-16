#pragma once
#include <iostream>
#include <string>

class Application
{
public:

	Application(std::string appTitle);
	Application(std::string appTitle, int windowWidth, int windowHeight);
	~Application();

	void Start();
	void Stop();
	GLFWwindow* GetWindow();

	void Tick(float deltaTime);


	// #TODO refactor this to events and error managers
	void HandleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	void HandleMouseInput(GLFWwindow* window, double xpos, double ypos);
	void HandleResizeEvent(GLFWwindow * window, int w, int h);
	void HandleError(int error, std::string desc);

private:

	bool _isRunning;
	std::string _appTitle;

	GLFWwindow* _window;
	int _windowWidth;
	int _windowHeight;

	float _deltaTime;

	bool Init();
	void Update(float deltaTime);
	void Render(float deltaTime);

	void Cleanup();
};