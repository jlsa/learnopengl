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

unsigned int LoadTexture(const char *path);

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
	light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

	


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
	int verticesSize = 8;
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, verticesSize * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, verticesSize * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attributes (only position data for the lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, verticesSize * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int diffuseMap = LoadTexture("Assets/Textures/container2.png");
	unsigned int specularMap = LoadTexture("Assets/Textures/container2_specular.png");
	// shader configuration
	ourShader.use();
	ourShader.setInt("material.diffuse", 0);
	ourShader.setInt("material.specular", 1);

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


		ourShader.use();
		ourShader.setVec3("light.position", light.position);
		ourShader.setVec3("viewPos", camera.Position);

		ourShader.setVec3("light.ambient", light.ambient);
		ourShader.setVec3("light.diffuse", light.diffuse);
		ourShader.setVec3("light.specular", light.specular);

		ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		ourShader.setFloat("material.shininess", 64.0f);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, nearPlane, farPlane);
		glm::mat4 view = camera.GetViewMatrix();

		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// world transformation
		glm::mat4 model;
		ourShader.setMat4("model", model);
		
		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// draw the sum'bitch
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / verticesSize);
		

		// draw the lamp object
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		//light.position.x = sin((float)glfwGetTime()) * 2.5f;
		//light.position.y = cos((float)glfwGetTime()) * 2.5f;
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

unsigned int LoadTexture(const char *path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}