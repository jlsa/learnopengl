#include <glad/glad.h>
#include <glfw3.h>

#include "Shader.h"
#include "Camera.h"

#include <iostream>

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char * GAME_TITLE = "Learning OpenGL C++ <3";

// extra controls
float interpolationValue{ 0.75f }; // 0.2f
bool perspectiveView{ true };

// global movement speed multiplier
float moveSpeed{ 5.0f }; // 0.001f

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
bool firstMouse{ true };
float lastX{ (float)SCR_WIDTH / 2.0f };
float lastY{ (float)SCR_HEIGHT / 2.0f };

// timing
double deltaTime{}; // time between current frame and last frame
double lastFrame{}; // time of last frame

float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
float nearPlane{ 0.1f };
float farPlane{ 100.0f };

glm::vec3 coral(1.0f, 0.5f, 0.31f);
glm::vec3 black(0.0f, 0.0f, 0.0f);
glm::vec3 dargGray(0.3f, 0.3f, 0.3f);
glm::vec4 backgroundColor(0.694f, 0.878f, 0.682f, 1.0f);

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


int main()
{
	backgroundColor = glm::vec4(dargGray, 1.0f);

	glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // 1.0f, 1.0f, 1.0f
	glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
	glm::vec3 result = lightColor * objectColor; // = (0.0f, 0.5f, 0.0f);

	//backgroundColor = glm::vec4(result, 1.0f);


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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, GAME_TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	Shader ourShader("LightVertexShader.vs", "LightFragmentShader.fs");
	Shader lampShader("LampVertexShader.vs", "LampFragmentShader.fs");

	glm::vec3 localTransform = glm::vec3(0.0f, 0.0f, 0.0f);
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	int verticesSize = 6;
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	float vertices3[] = {
		-0.5f + localTransform.x, -0.5f + localTransform.y, -0.5f + localTransform.z,  0.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x, -0.5f + localTransform.y, -0.5f + localTransform.z,  1.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x,  0.5f + localTransform.y, -0.5f + localTransform.z,  1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x,  0.5f + localTransform.y, -0.5f + localTransform.z,  1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x,  0.5f + localTransform.y, -0.5f + localTransform.z,  0.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x, -0.5f + localTransform.y, -0.5f + localTransform.z,  0.0f, 0.0f,	1.0f, 1.0f, 1.0f,

		-0.5f + localTransform.x, -0.5f + localTransform.y,  0.5f + localTransform.z,  0.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x, -0.5f + localTransform.y,  0.5f + localTransform.z,  1.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x,  0.5f + localTransform.y,  0.5f + localTransform.z,  1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x,  0.5f + localTransform.y,  0.5f + localTransform.z,  1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x,  0.5f + localTransform.y,  0.5f + localTransform.z,  0.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x, -0.5f + localTransform.y,  0.5f + localTransform.z,  0.0f, 0.0f,	1.0f, 1.0f, 1.0f,

		-0.5f + localTransform.x,  0.5f + localTransform.y,  0.5f + localTransform.z,  1.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x,  0.5f + localTransform.y, -0.5f + localTransform.z,  1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x, -0.5f + localTransform.y, -0.5f + localTransform.z,  0.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x, -0.5f + localTransform.y, -0.5f + localTransform.z,  0.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x, -0.5f + localTransform.y,  0.5f + localTransform.z,  0.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x,  0.5f + localTransform.y,  0.5f + localTransform.z,  1.0f, 0.0f,	1.0f, 1.0f, 1.0f,

		 0.5f + localTransform.x,  0.5f + localTransform.y,  0.5f + localTransform.z,  1.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x,  0.5f + localTransform.y, -0.5f + localTransform.z,  1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x, -0.5f + localTransform.y, -0.5f + localTransform.z,  0.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x, -0.5f + localTransform.y, -0.5f + localTransform.z,  0.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x, -0.5f + localTransform.y,  0.5f + localTransform.z,  0.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x,  0.5f + localTransform.y,  0.5f + localTransform.z,  1.0f, 0.0f,	1.0f, 1.0f, 1.0f,

		-0.5f + localTransform.x, -0.5f + localTransform.y, -0.5f + localTransform.z,  0.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x, -0.5f + localTransform.y, -0.5f + localTransform.z,  1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x, -0.5f + localTransform.y,  0.5f + localTransform.z,  1.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x, -0.5f + localTransform.y,  0.5f + localTransform.z,  1.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x, -0.5f + localTransform.y,  0.5f + localTransform.z,  0.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x, -0.5f + localTransform.y, -0.5f + localTransform.z,  0.0f, 1.0f,	1.0f, 1.0f, 1.0f,

		-0.5f + localTransform.x,  0.5f + localTransform.y, -0.5f + localTransform.z,  0.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x,  0.5f + localTransform.y, -0.5f + localTransform.z,  1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x,  0.5f + localTransform.y,  0.5f + localTransform.z,  1.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		 0.5f + localTransform.x,  0.5f + localTransform.y,  0.5f + localTransform.z,  1.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x,  0.5f + localTransform.y,  0.5f + localTransform.z,  0.0f, 0.0f,	1.0f, 1.0f, 1.0f,
		-0.5f + localTransform.x,  0.5f + localTransform.y, -0.5f + localTransform.z,  0.0f, 1.0f,	1.0f, 1.0f, 1.0f,
	};

	float vertices2[] = {
		 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,		0.5f, 0.5f, 0.5f,		// top (front)
		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,		0.5f, 0.5f, 0.5f,		// left (front)
		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.5f, 0.5f, 0.5f,		// right (front)

		 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,		0.5f, 0.5f, 0.5f,		// top (right)
		 1.0f, -1.0f,  1.0f,	0.0f, 1.0f,		0.5f, 0.5f, 0.5f,		// left (right)
		 1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.5f, 0.5f, 0.5f,		// right (right)
		
		 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,		0.5f, 0.5f, 0.5f,		// top (back)
		 1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.5f, 0.5f, 0.5f,		// left (back)
		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.5f, 0.5f, 0.5f,		// right (back)

		 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,		0.5f, 0.5f, 0.5f,		// top (left)
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f,		0.5f, 0.5f, 0.5f,		// left (left)
		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,		0.5f, 0.5f, 0.5f,		// right (left)
		
		
		// bottom
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.5f, 0.5f, 0.5f,		// front-left (bottom)
		 1.0f, -1.0f, -1.0f,	0.0f, 1.0f,		0.5f, 0.5f, 0.5f,		// front-right (bottom)
		 1.0f, -1.0f,  1.0f,	1.0f, 1.0f,		0.5f, 0.5f, 0.5f,		// back-right (bottom)

		 1.0f, -1.0f,  1.0f,	1.0f, 1.0f,		0.5f, 0.5f, 0.5f,		// back-right (bottom)
		-1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.5f, 0.5f, 0.5f,		// back-left (bottom)
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.5f, 0.5f, 0.5f		// front-left (bottom)
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	
	
	
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, verticesSize * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, verticesSize * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attributes (only position data for the lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, verticesSize * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		ourShader.use();
		ourShader.setVec3("objectColor", objectColor);
		ourShader.setVec3("lightColor", lightColor);
		ourShader.setVec3("lightPos", lightPos);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, nearPlane, farPlane);
		glm::mat4 view = camera.GetViewMatrix();

		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render container
		glBindVertexArray(VAO);
		glm::mat4 model;
		for (unsigned int i = 0; i < 10; i++) 
		{
			model = glm::mat4();
			
			float angle{ 20.0f * i * (float)glfwGetTime() };//* i + 1 };
			//angle = 0.0f;
			
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			// draw them sum'bitches
			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / verticesSize);
		}

		// draw the lamp object
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / verticesSize);
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO); // currently 
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

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

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		perspectiveView = true;

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		perspectiveView = false;


	float cameraSpeed = moveSpeed * (float)deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);


	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		lightPos.x -= 1.0f * (float)deltaTime;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		lightPos.x += 1.0f * (float)deltaTime;
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		lightPos.y += 1.0f * (float)deltaTime;
		/*interpolationValue += 1.0f * (float)deltaTime; // 0.01f is way to fast.. need to slow it down
		if (interpolationValue >= 1.0f) {
			interpolationValue = 1.0f; // lets add a cap to it. It acts strange otherwise.. NOT GOOD
		}*/
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		lightPos.y -= 1.0f * (float)deltaTime;
		/*interpolationValue -= 1.0f * (float)deltaTime;
		if (interpolationValue <= 0.0f) {
			interpolationValue = 0.0f; // lets add a cap to it. Same as up key
		}*/
	}
	
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coords go from bottom to top
	
	lastX = (float)xpos;
	lastY = (float)ypos;
	
	camera.ProcessMouseMovement((float)xoffset, (float)yoffset);
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}