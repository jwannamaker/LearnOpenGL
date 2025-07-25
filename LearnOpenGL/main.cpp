#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <iostream>

#define M_PI acos(-1.0)

using namespace std;
using namespace glm;

const unsigned int WINDOW_WIDTH = 550;
const unsigned int WINDOW_HEIGHT = 550;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path, int* imageWidth, int* imageHeight);

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Build and compile shader program
	Shader myShader("shader.vert", "shader.frag");

	// Setup textures
	int imageWidth, imageHeight;
	unsigned int texture0 = loadTexture("smoke.png", &imageWidth, &imageHeight);
	unsigned int texture1 = loadTexture("sulcata-front-view.png", &imageWidth, &imageHeight);

	// Setup model, view, and projection matrices
	mat4 model = mat4(1.0f);
	model = rotate(model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
	
	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.0f, -5.0f));

	mat4 projection = mat4(1.0f);
	projection = perspective(radians(60.0f), 0.5f, 0.1f, 100.0f);

	myShader.use();
	glUniform1i(glGetUniformLocation(myShader.ID, "texture0"), 0);
	glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 1);
	glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, value_ptr(projection));

	// Setup vertex data/buffers and configure vertex attributes
	GLfloat x = 0.5f;
	GLfloat y = 0.5f;
	GLfloat z = 0.5f;
	GLfloat vertices[] = {
		// positions	// texture coords
		-x, -y, -z,		0.0f, 0.0f,
		 x, -y, -z,		1.0f, 0.0f,
		 x,  y, -z,		1.0f, 1.0f,
		 x,  y, -z,		1.0f, 1.0f,
		-x,  y, -z,		0.0f, 1.0f,
		-x, -y, -z,		0.0f, 0.0f,
		 
		-x, -y,  z,		0.0f, 0.0f,
		 x, -y,  z,		1.0f, 0.0f,
		 x,  y,  z,		1.0f, 1.0f,
		 x,  y,  z,		1.0f, 1.0f,
		-x,  y,  z,		0.0f, 1.0f,
		-x, -y,  z,		0.0f, 0.0f,
		 
		-x,  y,  z,		1.0f, 0.0f,
		-x,  y, -z,		1.0f, 1.0f,
		-x, -y, -z,		0.0f, 1.0f,
		-x, -y, -z,		0.0f, 1.0f,
		-x, -y,  z,		0.0f, 0.0f,
		-x,  y,  z,		1.0f, 0.0f,
		 
		 x,  y,  z,		1.0f, 0.0f,
		 x,  y, -z,		1.0f, 1.0f,
		 x, -y, -z,		0.0f, 1.0f,
		 x, -y, -z,		0.0f, 1.0f,
		 x, -y,  z,		0.0f, 0.0f,
		 x,  y,  z,		1.0f, 0.0f,
		 
		-x, -y, -z,		0.0f, 1.0f,
		 x, -y, -z,		1.0f, 1.0f,
		 x, -y,  z,		1.0f, 0.0f,
		 x, -y,  z,		1.0f, 0.0f,
		-x, -y,  z,		0.0f, 0.0f,
		-x, -y, -z,		0.0f, 1.0f,
		 
		-x,  y, -z,		0.0f, 1.0f,
		 x,  y, -z,		1.0f, 1.0f,
		 x,  y,  z,		1.0f, 0.0f,
		 x,  y,  z,		1.0f, 0.0f,
		-x,  y,  z,		0.0f, 0.0f,
		-x,  y, -z,		0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	array<vec3, 8> positions = {
		vec3(-2.0f * x, -2.0f * y, -2.0f * z),
		vec3( 2.0f * x, -2.0f * y, -2.0f * z),
		vec3(-2.0f * x, -2.0f * y,  2.0f * z),
		vec3( 2.0f * x, -2.0f * y,  2.0f * z),
		vec3(-2.0f * x,  2.0f * y,  2.0f * z),
		vec3( 2.0f * x,  2.0f * y,  2.0f * z),
		vec3( 2.0f * x,  2.0f * y, -2.0f * z),
		vec3(-2.0f * x,  2.0f * y, -2.0f * z)
	};

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	float cameraSpeed = 0.05f;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.4f, 0.45f, 0.502f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
		myShader.use();
		float t = glfwGetTime();
		view = mat4(1.0f);
		view = translate(view, vec3(0.0f, 0.0f, -5.0f));
		view = rotate(view, radians(t * 20.0f), vec3(0.0f, 1.0f, 0.0f));
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			model = mat4(1.0f);
			model = translate(model, positions[i]);
			model = rotate(model, t * radians(50.0f), vec3(0.25 * sin(t), 0.25 * cos(t), 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, value_ptr(view));
		
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

unsigned int loadTexture(const char* path, int* imageWidth, int* imageHeight)
{
	unsigned int textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int numChannels;
	unsigned char* imageData = stbi_load(path, imageWidth, imageHeight, &numChannels, 0);
	if (!imageData)
	{
		cout << "Failed to load texture" << endl;
		return -1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *imageWidth, *imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);

	return textureID;
}