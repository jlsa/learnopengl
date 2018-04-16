#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <stdexcept>
#include "stb_image.h"

// self defined headers
#include "AppManager.h"
#include "Application.h"
#include "Shader.h"

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const std::string GAME_TITLE = "Learning OpenGL C++ <3";

// default was 2.0f
float interpolationValue = 0.2f;





int main()
{
	auto app = MakeApp<Application>("Test Application", 1024, 768);

	// ------------------------------------------------------------------
	// Computer Stats
	// ------------------------------------------------------------------
	int amountOfMonitors{};
	GLFWmonitor** monitors = glfwGetMonitors(&amountOfMonitors);
	std::cout << amountOfMonitors << " monitors connected." << std::endl;

	const char* name = glfwGetMonitorName(monitors[0]);
	std::cout << "Primary monitor : " << name << std::endl;

	if (amountOfMonitors > 1) {
		for (int i = 1; i < amountOfMonitors; i++) {
			name = glfwGetMonitorName(monitors[i]);
			std::cout << "additional monitor : " << name << std::endl;
		}
	}

	const GLubyte* gl_vendor = glGetString(GL_VENDOR);
	const GLubyte* gl_renderer = glGetString(GL_RENDERER);
	const GLubyte* gl_version = glGetString(GL_VERSION);

	printf("Vendor: %s\n", gl_vendor);
	printf("Renderer: %s\n", gl_renderer);
	printf("Version: %s\n", gl_version);



	Shader ourShader("VertexShader.vs", "FragmentShader.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//positions				colors				texture coords
		 0.5f,	0.5f, 0.0f,		1.0f, 0.0f,	0.0f,	1.0f, 1.0f,	// top right
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f,	0.0f,	1.0f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,	1.0f,	0.0f, 0.0f,	// bottom left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f,	0.0f,	0.0f, 1.0f	// top left
	};

	unsigned int indices[] = {
		0, 1, 3,	// first triangle
		1, 2, 3		// second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// first triangle setup
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// generating a texture
	unsigned int texture1, texture2;

	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Loading an image and generate it
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // flip image on the y-axis.

	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	
	// set texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// loading an image, create texture and generate mipmaps
	// data = stbi_load("container.jpg", &width, &height, &nrChannels, 0); 
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that awesomeface.png has transparency and thus an alpha channel,
		// so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell OpenGL for each sampler to which texture unit it belongs to (only has to be done once)
	ourShader.use(); // dont forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	// or set it via the texture class
	ourShader.setInt("texture2", 1);


	double lastFPStime = glfwGetTime();
	double lastFrameTime = glfwGetTime();

	int frameCounter = 0;

	double t0 = glfwGetTime();
	double t1;

	float deltaTime;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(app->GetWindow()))
	{
		t1 = glfwGetTime();
		deltaTime = (float)(t1 - t0);
		t0 = t1;

		// update fps
		// ----------
		double thisFPStime = glfwGetTime();
		frameCounter++;

		if (thisFPStime - lastFPStime >= 1.0)
		{
			lastFPStime = thisFPStime;

			std::string windowTitle = GAME_TITLE + " (";
			windowTitle += std::to_string(frameCounter);
			windowTitle += " fps - [ ";
			windowTitle += std::to_string(deltaTime);
			windowTitle += " ])";
			const char* windowCaption = windowTitle.c_str();
			glfwSetWindowTitle(app->GetWindow(), windowCaption);

			frameCounter = 0;
		}


		double currTime = glfwGetTime();

		app->Tick(deltaTime);

		// renderer.renderScene(currtime - lastFrameTime); // deltatime
		lastFrameTime = currTime;

		// input
		// -----
		//processInput(app->GetWindow());

		// render
		// ------
		glClearColor(0.694f, 0.878f, 0.682f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.setFloat("InterPolation", interpolationValue);
		// render container
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(app->GetWindow());
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO); // currently 1
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		interpolationValue += 0.001f; // 0.01f is way to fast.. need to slow it down
		if (interpolationValue >= 1.0f) {
			interpolationValue = 1.0f; // lets add a cap to it. It acts strange otherwise.. NOT GOOD
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		interpolationValue -= 0.001f;
		if (interpolationValue <= 0.0f) {
			interpolationValue = 0.0f; // lets add a cap to it. Same as up key
		}
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
/*void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}*/