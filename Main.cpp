#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Color.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>

unsigned int LoadTexture(const char *path);

// callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1920/2;//800*2;
const unsigned int SCR_HEIGHT = 1080/2;//600*2;
const char * GAME_TITLE = "Learning OpenGL C++ <3";

// extra controls
float interpolationValue{ 0.75f }; // 0.2f
bool perspectiveView{ true };

// global movement speed multiplier
float moveSpeed{ 5.0f }; // 0.001f

// camera
glm::vec3 cameraPositions[] = {
	glm::vec3(0.0f, 0.0f, 5.0f),
	glm::vec3(-13.f, 10.0f, 5.0f),
	glm::vec3(-5.0f, 5.0f, 7.5f)
};
Camera camera(cameraPositions[2], glm::vec3(0.0f, 1.0f, 0.0f), -45.0f, -30.0f);// 0.0f, 0.0f, 5.0f));

bool firstMouse{ true };
float lastX{ (float)SCR_WIDTH / 2.0f };
float lastY{ (float)SCR_HEIGHT / 2.0f };

// timing
double deltaTime{}; // time between current frame and last frame
double lastFrame{}; // time of last frame

float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
float nearPlane{ 0.1f };
float farPlane{ 100.0f };

enum Sides {
	One,
	Two,
	Three,
	Four
};

glm::vec3 backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);//color::navajowhite, 1.0f);

const int GRID_SIZE = 10;

Sides currentSide = One;
glm::vec3 positionOffset(0.0f, 0.001f, 0.0f);
glm::vec3 gridRotation(0.0f, 0.0f, 0.0f);

bool grid[GRID_SIZE][GRID_SIZE][GRID_SIZE];

int main()
{
	camera.MovementSpeed = moveSpeed;
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
	glDepthFunc(GL_LESS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))

	// load textures
	// -------------
	Shader lampShader("LampVertexShader.vs", "LampFragmentShader.fs");
	Model cubeModel("Assets/Models/cube/cube.obj");
	
	for (unsigned int x = 0; x < GRID_SIZE; x++)
	{
		for (unsigned int y = 0; y < GRID_SIZE; y++)
		{
			for (unsigned int z = 0; z < GRID_SIZE; z++)
			{
				grid[x][y][z] = false;
			}
		}
	}

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4();

		lampShader.use();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		
		float angle = 0.0f;
		glm::vec3 move(0.0f, 0.0f, 0.0f);
		if (currentSide == One) 
		{
			move = glm::vec3(0.0f, 0.0f, 0.0f);
			angle = 0.0f;
		}
		if (currentSide == Two) 
		{
			move = glm::vec3(-((float)GRID_SIZE-1.0f), 0.0f, 0.0f);
			angle = 90.0f;
		}
		if (currentSide == Three) 
		{
			move = glm::vec3(-((float)GRID_SIZE - 1.0f), 0.0f, -((float)GRID_SIZE - 1.0f));
			angle = 180.0f;
		}
		if (currentSide == Four) 
		{
			move = glm::vec3(0.0f, 0.0f, -((float)GRID_SIZE - 1.0f));
			angle = 270.0f;
		}

		for (unsigned int x = 0; x < GRID_SIZE; x++) 
		{
			for (unsigned int y = 0; y < GRID_SIZE; y++) 
			{
				for (unsigned int z = 0; z < GRID_SIZE; z++) 
				{
					glm::vec3 margin(0.1f * x, 0.1f * y, 0.1f * z);
					margin = glm::vec3(0.1f, 0.1f, 0.1f);
					glm::vec3 col;
					if (grid[x][y][z]) 
					{
						lampShader.use();
						model = glm::mat4();
						glm::vec3 position;
						position = glm::vec3(float(x), float(y), float(z));
						position += move;

						model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
						model = glm::translate(model, position);
						model = glm::scale(model, glm::vec3(0.99f));
						lampShader.setMat4("model", model);

						col = color::lightyellow;
						lampShader.setVec3("color", col);
						cubeModel.Draw(lampShader);

						model = glm::mat4();
						model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
						model = glm::translate(model, position);
						col = color::maroon;
						lampShader.setVec3("color", col);
						model = glm::scale(model, glm::vec3(0.995f));
						lampShader.setMat4("model", model);
						cubeModel.DrawMesh(lampShader);
					}
					glm::vec3 position;
					position = glm::vec3(float(x), float(y), float(z));
					model = glm::mat4();
					model = glm::translate(model, position);// +margin);

					model = glm::scale(model, glm::vec3(1.001f, 1.001f, 1.001f));// 1.001f, 1.001f, 1.001f));
					lampShader.setMat4("model", model);
					col = color::white;
					lampShader.setVec3("color", col);
					cubeModel.DrawMesh(lampShader);
				}
			}
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
}

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		std::cout << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << std::endl;

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) 
	{
		if (currentSide == One) 
		{
			currentSide = Two;
		}
		else if (currentSide == Two)
		{
			currentSide = Three;
		}
		else if (currentSide == Three)
		{
			currentSide = Four;
		}
		else if (currentSide == Four)
		{
			currentSide = One;
		}
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		if (currentSide == Four)
		{
			currentSide = Three;
		}
		else if (currentSide == Three)
		{
			currentSide = Two;
		}
		else if (currentSide == Two)
		{
			currentSide = One;
		}
		else if (currentSide == One)
		{
			currentSide = Four;
		}
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