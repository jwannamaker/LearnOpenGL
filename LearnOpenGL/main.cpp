#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define M_PI acos(-1.0)

using namespace std;


const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"out vec4 vertexColor;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.xyz, 1.0);\n"
	"	vertexColor = vec4((aPos.x), sin(aPos.y), sin(aPos.z), 1.0);\n"
	"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 customColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = customColor;\n"
	"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void validateShaderCompilation(unsigned int shaderID, const char* shaderType);
void validateProgramCompilation(unsigned int programID);

int main()
{
	// Initialize and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window with GLFW
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// Build and compile shader program for left and right triangle
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	validateShaderCompilation(vertexShader, "VERTEX");

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	validateShaderCompilation(fragmentShader, "FRAGMENT");

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	validateProgramCompilation(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	// Setup vertex data/buffers and configure vertex attributes
	GLfloat leftVertices[] = {
		-0.75f, 0.0f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		-0.25f, -0.5f, 0.0f
	};
	GLfloat rightVertices[] = {
		0.75f, 0.0f, 0.0f,
		0.25f, 0.5f, 0.0f,
		0.25f, -0.5f, 0.0f
	};

	GLuint VAO[2];
	GLuint VBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftVertices), leftVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightVertices), rightVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	GLfloat timeValue;
	GLfloat redValue;
	GLfloat blueValue;
	int vertexColorLocation;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.4f, 0.45f, 0.502f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		timeValue = glfwGetTime();
		redValue = (sin(timeValue) / 2.0f) + 0.5f;	// Scale & translate sin(x) --> range [0.0f - 1.0f]
		blueValue = (sin(timeValue + M_PI) / 2.0f) + 0.5f; 
		vertexColorLocation = glGetUniformLocation(shaderProgram, "customColor");
		glUseProgram(shaderProgram);

		glUniform4f(vertexColorLocation, redValue, 0.0f, blueValue, 1.0f);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUniform4f(vertexColorLocation, blueValue, 0.0f, redValue, 1.0f);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void validateShaderCompilation(unsigned int shaderID, const char* shaderType)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << endl;
	}
}

void validateProgramCompilation(unsigned int programID)
{
	int success;
	char infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		cout << "ERROR::SHADER_PROGRAM::COMPILATION_FAILED\n" << infoLog << endl;
	}
}
