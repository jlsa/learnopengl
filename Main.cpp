#include <glad/glad.h>
#include <glfw3.h>

#include "Shader.h"
#include "Camera.h"

#include <iostream>

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct Light {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

// callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

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
glm::vec3 darkGray(0.3f, 0.3f, 0.3f);
glm::vec4 backgroundColor(0.694f, 0.878f, 0.682f, 1.0f);


Light light;

float specularStrength = 0.5f;
int specularPower = 2;

int main()
{
	light.position = glm::vec3(1.2f, 1.0f, 2.0f);
	light.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

	Material emeraldMat;
	emeraldMat.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
	emeraldMat.diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);
	emeraldMat.specular = glm::vec3(0.633f, 0.727811f, 0.633f);
	emeraldMat.shininess = 256.0f;

	Material jadeMat;
	jadeMat.ambient = glm::vec3(0.135f, 0.2225f, 0.1575f);
	jadeMat.diffuse = glm::vec3(0.54f, 0.89f, 0.63f);
	jadeMat.specular = glm::vec3(0.316228f, 0.316228f, 0.316228f);
	jadeMat.shininess = 32.0f;

	Material obsidianMat;
	obsidianMat.ambient = glm::vec3(0.05375f, 0.05f, 0.06625f);
	obsidianMat.diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f);
	obsidianMat.specular = glm::vec3(0.332741f, 0.328634f, 0.346435f);
	obsidianMat.shininess = 32.0f;

	Material pearlMat;
	pearlMat.ambient = glm::vec3(0.25f, 0.829f, 0.829f);
	pearlMat.diffuse = glm::vec3(1.0f, 0.61424f, 0.07568f);
	pearlMat.specular = glm::vec3(0.296648f, 0.296648f, 0.296648f);
	pearlMat.shininess = 32.0f;

	Material rubyMat;
	rubyMat.ambient = glm::vec3(0.1745f, 0.01175f, 0.01175f);
	rubyMat.diffuse = glm::vec3(0.61424f, 0.04136f, 0.04136f);
	rubyMat.specular = glm::vec3(0.727811f, 0.626959f, 0.626959f);
	rubyMat.shininess = 32.0f;

	Material yellowRubberMat;
	yellowRubberMat.ambient = glm::vec3(0.05f, 0.05f, 0.0f);
	yellowRubberMat.diffuse = glm::vec3(0.5f, 0.5f, 0.4f);
	yellowRubberMat.specular = glm::vec3(0.7f, 0.7f, 0.04f);
	yellowRubberMat.shininess = 32.0f;

	Material yellowPlasticMat;
	yellowPlasticMat.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	yellowPlasticMat.diffuse = glm::vec3(0.5f, 0.5f, 0.0f);
	yellowPlasticMat.specular = glm::vec3(0.60f, 0.60f, 0.50f);
	yellowPlasticMat.shininess = 32.0f;

	Material blackPlasticMat;
	blackPlasticMat.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	blackPlasticMat.diffuse = glm::vec3(0.01f, 0.01f, 0.01f);
	blackPlasticMat.specular = glm::vec3(0.50f, 0.50f, 0.50f);
	blackPlasticMat.shininess = 4.0f;

	Material bronzeMat;
	bronzeMat.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
	bronzeMat.diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);
	bronzeMat.specular = glm::vec3(0.633f, 0.727811f, 0.633f);
	bronzeMat.shininess = 32.0f;

	Material chromeMat;
	chromeMat.ambient = glm::vec3(0.0f, 0.1f, 0.06f);
	chromeMat.diffuse = glm::vec3(0.0f, 0.50980392f, 0.50980392f);
	chromeMat.specular = glm::vec3(0.50196078f, 0.50196078f, 0.50196078f);
	chromeMat.shininess = 32.0f;


	backgroundColor = glm::vec4(coral * darkGray, 1.0f);

	glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // 1.0f, 1.0f, 1.0f
	glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
	glm::vec3 result = lightColor * objectColor; // = (0.0f, 0.5f, 0.0f);

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
	glfwSetKeyCallback(window, keyboard_callback);
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

	Material materials[] = {
		emeraldMat,
		jadeMat,
		obsidianMat,
		pearlMat,
		rubyMat,
		yellowRubberMat,
		yellowPlasticMat,
		blackPlasticMat,
		bronzeMat,
		chromeMat
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

		glm::vec3 lightColor;
		//lightColor.x = sin(glfwGetTime() * 2.0f);
		//lightColor.y = sin(glfwGetTime() * 0.7f);
		//lightColor.z = sin(glfwGetTime() * 1.3f);

		//glm::vec3 diffuseColor = lightColor	  * glm::vec3(0.5f); // decrease the influence
		//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		
		//light.ambient = ambientColor;
		//light.diffuse = diffuseColor;

		// activate shader
		ourShader.use();
		ourShader.setVec3("objectColor", objectColor);
		ourShader.setVec3("lightColor", lightColor);
		ourShader.setVec3("lightPos", light.position);
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setFloat("specularStrength", specularStrength);
		ourShader.setInt("specularPower", specularPower);


		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, nearPlane, farPlane);
		glm::mat4 view = camera.GetViewMatrix();

		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		ourShader.setVec3("light.ambient", light.ambient);
		ourShader.setVec3("light.diffuse", light.diffuse);
		ourShader.setVec3("light.specular", light.specular);

		// render container
		glBindVertexArray(VAO);
		glm::mat4 model;
		for (unsigned int x = 0; x < 3; x++) 
		{
			for (unsigned int y = 0; y < 3; y++)
			{
				model = glm::mat4();

				float angle; // { 20.0f * i * (float)glfwGetTime() };//* i + 1 };
				angle = 0.0f;

				model = glm::translate(model, glm::vec3(1.1f * x, 1.1f * y, 0.0f));//cubePositions[i]);
				//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				ourShader.setMat4("model", model);

				// material
				ourShader.setVec3("material.ambient", materials[x * 3 + y].ambient);
				ourShader.setVec3("material.diffuse", materials[x * 3 + y].diffuse);
				ourShader.setVec3("material.specular", materials[x * 3 + y].specular);
				ourShader.setFloat("material.shininess", materials[x * 3 + y].shininess);

				// draw them sum'bitches
				glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / verticesSize);
			}
		}

		// draw the lamp object
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		light.position.x = sin((float)glfwGetTime()) * 2.5f;
		light.position.y = cos((float)glfwGetTime()) * 2.5f;
		//light.position.z = cos((float)glfwGetTime()) * 2.5f;

		model = glm::mat4();
		model = glm::translate(model, light.position);
		float lightAngle{ 5.0f * (float)glfwGetTime() };
		model = glm::rotate(model, lightAngle, glm::vec3(-1.0f, 0.0f, 0.0f));
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
	
	/*
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	*/

	float cameraSpeed = moveSpeed * (float)deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);


	/*if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		lightPos.x -= 1.0f * (float)deltaTime;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		lightPos.x += 1.0f * (float)deltaTime;
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		lightPos = camera.Position;
		*/
	
	
}

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		light.position = camera.Position;
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
	/*if (yoffset > 0) {
		specularStrength += 0.1f *(float)deltaTime;
	}
	else if (yoffset < 0) {
		specularStrength -= 0.1f *(float)deltaTime;
	}*/
	camera.ProcessMouseScroll((float)yoffset);
}