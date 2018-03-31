#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const char * GAME_TITLE = "Project-Mayra";

// Shader sources
const GLchar* vertexShaderSource = R"glsl(
    #version 330 core
	layout (location = 0) in vec3 aPos; // position variable has attribute position 0
	layout (location = 1) in vec3 aColor; // color variable has attribute position 1

	out vec3 ourColor; // output a color to the fragment shader
	void main()
	{
		gl_Position = vec4(aPos, 1.0);
		ourColor = aColor; 
	}
)glsl";

const GLchar* fragmentShaderSource = R"glsl(
    #version 330 core
	out vec4 FragColor;
	
	in vec3 ourColor; // we set this variable in the OpenGL code.

	void main()
	{
		FragColor = vec4(ourColor, 1.0);
		//FragColor = vec4(0.396f, 0.498f, 0.388f, 1.0f);
	}
)glsl";

const GLchar* fragmentShaderSource2 = R"glsl(
    #version 330 core
	out vec4 FragColor;
	
	in vec3 ourColor;

	void main()
	{
		FragColor = vec4(ourColor, 1.0);
		//FragColor = vec4(0.972f, 0.964f, 0.466f, 1.0f);
	}
)glsl";

int main()
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, GAME_TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/*int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
*/
	// build and compile the shader program
	// ------------------------------------
	// vertex shader
	unsigned int vertexShader;
	unsigned int fragmentShaders[2];
	unsigned int shaderPrograms[2];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
	shaderPrograms[0] = glCreateProgram();
	shaderPrograms[1] = glCreateProgram();

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShaders[0], 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaders[0]);

	glShaderSource(fragmentShaders[1], 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShaders[1]);

	// link first program object
	glAttachShader(shaderPrograms[0], vertexShader);
	glAttachShader(shaderPrograms[0], fragmentShaders[0]);
	glLinkProgram(shaderPrograms[0]);
	// link second using a different fragment shader, but the same vertex shader
	glAttachShader(shaderPrograms[1], vertexShader);
	glAttachShader(shaderPrograms[1], fragmentShaders[1]);
	glLinkProgram(shaderPrograms[1]);


	/*// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// first fragment shader
	*/
	

	/*glGetShaderiv(fragmentShaders[0], GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaders[0], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// second fragment shader
	fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaders[1], 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShaders[1]);

	glGetShaderiv(fragmentShaders[1], GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaders[1], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}*/

	// link shaders
	//unsigned int shaderProgram;
	//shaderProgram = glCreateProgram();

	/*glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShaders[0]);
	glAttachShader(shaderProgram, fragmentShaders[1]);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// clean up - delete shaders after they have been linked is good!
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaders[0]);
	glDeleteShader(fragmentShaders[1]);
	*/

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float triangle1[] = {
		//	X		Y		Z		R		G		B
			-0.9f,	-0.5f,	0.0f,	1.0f,	0.0f,	0.0f,	// vertex 1 (left)
			-0.0f,	-0.5f,	0.0f,	0.0f,	1.0f,	0.0f,	// vertex 2 (right)
			-0.45f,  0.5f,	0.0f,	0.0f,	1.0f,	1.0f	// vertex 3 (top)
	};

	float triangle2[] = {
		//	X		Y		Z		R		G		B
			0.0f,	-0.5f,	0.0f,	0.0f,	1.0f,	1.0f,	// vertex 1 (left)
			0.9f,	-0.5f,	0.0f,	0.0f,	1.0f,	0.0f,	// vertex 2 (right)
			0.45f,   0.5f,	0.0f,	1.0f,	0.0f,	0.0f,	// vertex 3 (top)
	};
	
	unsigned int VBOs[2], VAOs[2];//, EBO;
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	//glGenBuffers(1, &EBO);

	// first triangle setup
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second triangle setup
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.694f, 0.878f, 0.682f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		// draw our first triangle
		glUseProgram(shaderPrograms[0]);

		// update the uniform color
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderPrograms[0], "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		// draw first triangle
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderPrograms[1]);
		// draw second triangle
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}