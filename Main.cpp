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

struct OldMaterial {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

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
	glm::vec3(-4.0f, 5.0f, 6.0f)
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


Light light;
DirectionalLight directionalLight;
PointLight pointLights[4];
SpotLight spotLight;

// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-0.949481f, 1.94278f, 9.54091f),
	glm::vec3(8.04138f, 1.74358f, 9.83926f),
	glm::vec3(9.24493f, 1.46474f, -0.661269f),
	glm::vec3(-1.38883f, 1.81763f, -2.11236f)
};

glm::vec3 backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);//color::navajowhite, 1.0f);

float cubeVertices[] = {
	// positions		  // texture coordinates
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float planeVertices[] = {
	// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

int main()
{
	camera.MovementSpeed = moveSpeed;
	light.position = glm::vec3(1.2f, 1.0f, 2.0f);
	light.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

	OldMaterial emeraldMat;
	emeraldMat.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
	emeraldMat.diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);
	emeraldMat.specular = glm::vec3(0.633f, 0.727811f, 0.633f);
	emeraldMat.shininess = 256.0f;

	glm::vec3 lightColor = color::antiquewhite;
	glm::vec3 objectColor = color::coral;

	// directional light
	directionalLight.direction = glm::vec3(-0.2f, 10.0f, -0.3f);
	directionalLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f); 
	directionalLight.diffuse = color::white;
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
	glDepthFunc(GL_LESS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))

	

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	// load textures
	// -------------
	unsigned int cubeTexture = LoadTexture("Assets/Textures/texture.png");
	unsigned int floorTexture = LoadTexture("Assets/Textures/metal.png");


	Shader depthShader("Assets/Shaders/depth_testing.vs", "Assets/Shaders/depth_testing.fs");
	Shader shader("LightVertexShader.vs", "LightFragmentShader.fs");
	Shader lampShader("LampVertexShader.vs", "LampFragmentShader.fs");
	Shader testShader("VertexShader.vs", "FragmentShader.fs");
	
	Model suzanne("Assets/Models/suzanne/suzanne.obj");
	Model lowpolycharacter("Assets/Models/low-poly-character/character_low_anim.obj");
	Model town("Assets/Models/medieval-town-base/sketchfab.obj");
	Model nanosuit("Assets/Models/nanosuit/nanosuit.obj");
	Model rotatedBox("Assets/Models/rotated-box/rotated-box.obj");

	depthShader.use();
	depthShader.setInt("texture1", 0);

	glm::vec3 positionOffset(0.0f, 0.001f, 0.0f);
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

		depthShader.use();
		depthShader.setMat4("view", view);
		depthShader.setMat4("projection", projection);

		lampShader.use();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		

		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);

		depthShader.use();

		/*for (unsigned int x = 0; x < 10; x++)
		{
			for (unsigned int y = 0; y < 10; y++)
			{
				for (unsigned int z = 0; z < 10; z++)
				{
					model = glm::mat4();
					model = glm::translate(model, glm::vec3(float(x), float(y), float(z)));
					depthShader.setMat4("model", model);
					//glDrawArrays(GL_TRIANGLES, 0, 36);

					//model = glm::mat4();
					model = glm::scale(model, glm::vec3(1.01f, 1.01f, 1.01f));
					lampShader.use();
					lampShader.setMat4("view", view);
					lampShader.setMat4("projection", projection);
					lampShader.setVec3("color", color::white);
					lampShader.setMat4("model", model);
					glDrawArrays(GL_LINE_STRIP, 0, 36);
				}
			}
		}*/
		float angle = 10.0f * (float)glfwGetTime();
		int gridSize = 3;
		for (unsigned int x = 0; x < gridSize; x++) {
			for (unsigned int y = 0; y < 1; y++) {
				for (unsigned int z = 0; z < gridSize; z++) {
					depthShader.use();
					model = glm::mat4();
					model = glm::translate(model, glm::vec3(float(x), float(y), float(z)) + positionOffset);
					//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f));
					depthShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);

					model = glm::mat4();
					model = glm::translate(model, glm::vec3(float(x), float(y), float(z)));
					model = glm::scale(model, glm::vec3(1.001f, 1.001f, 1.001f));
					lampShader.use();
					lampShader.setMat4("model", model);
					lampShader.setVec3("color", color::white);

					glDrawArrays(GL_LINE_STRIP, 0, 36);
				}
			}
		}

		

		// floor plane
		/*glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		depthShader.setMat4("model", glm::mat4());
		glDrawArrays(GL_LINE_STRIP, 0, 6);
		glBindVertexArray(0);*/

		//model = glm::mat4();
		//depthShader.setMat4("model", model);
		//rotatedBox.Draw(depthShader);
		
		/*
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		shader.setVec3("viewPos", camera.Position);
		shader.setFloat("material.shininess", 32.0f);
		*/
		/*
		Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		/*
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
		shader.setFloat("spotLight.linear", 0.09f);
		shader.setFloat("spotLight.quadratic", 0.032f);
		shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		
		shader.setBool("useSpotLight", true);
		
		// render the loaded models
		//glm::mat4 model;

		testShader.use();
		testShader.setMat4("projection", projection);
		testShader.setMat4("view", view);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 3.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		testShader.setMat4("model", model);
		testShader.setVec3("objectColor", objectColor);
		testShader.setVec3("lightColor", lightColor);
		testShader.setVec3("lightPos", light.position);
		testShader.setVec3("viewPos", camera.Position);
		
		testShader.setVec3("light.ambient", light.ambient);
		testShader.setVec3("light.diffuse", light.diffuse);
		testShader.setVec3("light.specular", light.specular);

		testShader.setVec3("material.ambient", emeraldMat.ambient);
		testShader.setVec3("material.diffuse", emeraldMat.diffuse);
		testShader.setVec3("material.specular", emeraldMat.specular);
		testShader.setFloat("material.shininess", emeraldMat.shininess);
		//shader.use();
		//shader.use();
		//suzanne.Draw(testShader);


		shader.use();
		for (int i = 0; i < 1; i++) {
			for (int j = 0; j < 1; j++) {
				model = glm::mat4();
				float angle = (i * j) * 20.0f;
				model = glm::translate(model, glm::vec3(0.8f * i, -0.5f, 0.8f * j));
				model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
				//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				shader.setMat4("model", model);
				//lowpolycharacter.Draw(shader);
			}
		}

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, -0.5f, 2.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setMat4("model", model);
		shader.use();
		//nanosuit.Draw(shader);

		// also draw the lamp object(s)
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader.setMat4("model", model);
			lampShader.setVec3("color", pointLights[i].diffuse);
			//suzanne.Draw(lampShader);
		}
		*/

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up
	// --------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);

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