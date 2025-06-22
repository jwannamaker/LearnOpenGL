#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define M_PI acos(-1.0)

using namespace std;


const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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

	// Build and compile shader program
	Shader myShader("shader.vert", "shader.frag");

	// Setup vertex data/buffers and configure vertex attributes
	GLfloat leftVertices[] = {
		// positions			// colors
		-0.75f,  0.0f,  0.0f,	1.0f, 0.0f, 0.0f,
		-0.25f,  0.5f,  0.0f,	0.0f, 1.0f, 0.0f,
		-0.25f, -0.5f,  0.0f,	0.0f, 0.0f, 1.0f
	};
	GLfloat rightVertices[] = {
		// positions			// colors
		0.75f,  0.0f,  0.0f,	1.0f, 0.0f, 0.0f,
		0.25f,  0.5f,  0.0f,	0.0f, 1.0f, 0.0f,
		0.25f, -0.5f,  0.0f,	0.0f, 0.0f, 1.0f
	};

	GLuint VAO[2];
	GLuint VBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftVertices), leftVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightVertices), rightVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

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

		myShader.use();

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
