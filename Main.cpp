#include <glad/glad.h>
#include <glfw3.h>

#include "Shader.h"
#include "Camera.h"

#include <iostream>

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Color.h"

struct Material {
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};


struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLight {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Light {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct Transform {
	glm::vec3 position;
	float angle;
	glm::vec3 rotation;
	glm::vec3 scale;
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



DirectionalLight directionalLight;
PointLight pointLights[4];
SpotLight spotLight;

// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 backgroundColor = glm::vec4(color::black, 1.0f);

int main()
{
	// directional light
	directionalLight.direction = glm::vec3(-0.2f, 10.0f, -0.3f);
	directionalLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f); 
	directionalLight.diffuse = color::darkmagenta;
	directionalLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);

	// point lights
	pointLights[0].position = pointLightPositions[0];
	pointLights[0].ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	pointLights[0].diffuse = color::tomato;
	pointLights[0].specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLights[0].constant = 1.0f;
	pointLights[0].linear = 0.09f;
	pointLights[0].quadratic = 0.032f;

	pointLights[1].position = pointLightPositions[1];
	pointLights[1].ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	pointLights[1].diffuse = color::cadetblue;
	pointLights[1].specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLights[1].constant = 1.0f;
	pointLights[1].linear = 0.09f;
	pointLights[1].quadratic = 0.032f;

	pointLights[2].position = pointLightPositions[2];
	pointLights[2].ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	pointLights[2].diffuse = color::khaki;
	pointLights[2].specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLights[2].constant = 1.0f;
	pointLights[2].linear = 0.09f;
	pointLights[2].quadratic = 0.032f;

	pointLights[3].position = pointLightPositions[3];
	pointLights[3].ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	pointLights[3].diffuse = color::gold;
	pointLights[3].specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLights[3].constant = 1.0f;
	pointLights[3].linear = 0.09f;
	pointLights[3].quadratic = 0.032f;

	// spot light
	spotLight.position = camera.Position;
	spotLight.direction = camera.Front;
	spotLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	spotLight.diffuse = color::white;
	spotLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLight.constant	= 1.0f;
	spotLight.linear	= 0.09f;
	spotLight.quadratic = 0.032f;
	spotLight.cutOff	= glm::cos(glm::radians(12.5f));
	spotLight.outerCutOff = glm::cos(glm::radians(15.0f));


	


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

	Shader shader("LightVertexShader.vs", "LightFragmentShader.fs");
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
		 0.0f,  1.0f,  0.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,		// top (front)
		-1.0f, -1.0f,  1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,				// left (front)
		 1.0f, -1.0f,  1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 0.0f,				// right (front)

		 0.0f,  1.0f,  0.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,				// top (right)
		 1.0f, -1.0f,  1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 1.0f,				// left (right)
		 1.0f, -1.0f, -1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,			// right (right)
		
		 0.0f,  1.0f,  0.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,			// top (back)
		 1.0f, -1.0f, -1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,			// left (back)
		-1.0f, -1.0f, -1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 0.0f,			// right (back)

		 0.0f,  1.0f,  0.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,				// top (left)
		-1.0f, -1.0f, -1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 1.0f,				// left (left)
		-1.0f, -1.0f,  1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,				// right (left)
		
		
		// bottom
		-1.0f, -1.0f, -1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,				// front-left (bottom)
		 1.0f, -1.0f, -1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 1.0f,				// front-right (bottom)
		 1.0f, -1.0f,  1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,				// back-right (bottom)

		 1.0f, -1.0f,  1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 1.0f,				// back-right (bottom)
		-1.0f, -1.0f,  1.0f,	0.5f, 0.5f, 0.5f,	1.0f, 0.0f,				// back-left (bottom)
		-1.0f, -1.0f, -1.0f,	0.5f, 0.5f, 0.5f,	0.0f, 0.0f,				// front-left (bottom)
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

	/*Transform transforms[] = {

	};*/

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
	shader.use();
	shader.setInt("material.diffuse", 0);
	shader.setInt("material.specular", 1);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		shader.use();
		shader.setVec3("viewPos", camera.Position);
		shader.setFloat("material.shininess", 32.0f);

		/*
		Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		shader.setVec3("dirLight.direction", directionalLight.direction);
		shader.setVec3("dirLight.ambient", directionalLight.ambient);
		shader.setVec3("dirLight.diffuse", directionalLight.diffuse);
		shader.setVec3("dirLight.specular", directionalLight.specular);

		// point lights
		for (int i = 0; i < 4; i++) {
			shader.setVec3("pointLights[" + std::to_string(i) + "].position",	pointLights[i].position);
			shader.setVec3("pointLights[" + std::to_string(i) + "].ambient",	pointLights[i].ambient);
			shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse",	pointLights[i].diffuse);
			shader.setVec3("pointLights[" + std::to_string(i) + "].specular",	pointLights[i].specular);
			shader.setFloat("pointLights[" + std::to_string(i) + "].constant",	pointLights[i].constant);
			shader.setFloat("pointLights[" + std::to_string(i) + "].linear",	pointLights[i].linear);
			shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i].quadratic);
		}
		// spotLight
		shader.setVec3("spotLight.position", camera.Position);
		shader.setVec3("spotLight.direction", camera.Front);
		shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("spotLight.constant", 1.0f);
		shader.setFloat("spotLight.linear", 0.09);
		shader.setFloat("spotLight.quadratic", 0.032);
		shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		// world transformation
		glm::mat4 model;
		shader.setMat4("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// render containers
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// also draw the lamp object(s)
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader.setMat4("model", model);
			lampShader.setVec3("color", pointLights[i].diffuse);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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