#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define M_PI acos(-1.0)
using namespace std;
using namespace glm;

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 800;

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup textures
	int imageWidth, imageHeight;
	unsigned int koiTexture = loadTexture("perfect-koi-fish.png", &imageWidth, &imageHeight);

	// Build and compile shader program
	Shader myShader("shader.vert", "shader.frag");

	// Setup vertex data/buffers and configure vertex attributes
	GLfloat x = imageWidth;
	GLfloat y = imageHeight;
	GLfloat koiVertices[] = {
		// positions	// texture coords
		-x, +y, 0.0f,	0.0f, 1.0f,	// top left
		+x, +y, 0.0f,	1.0f, 1.0f,	// top right
		-x, -y, 0.0f,	0.0f, 0.0f,	// bottom left
		+x, -y, 0.0f,	1.0f, 0.0f,	// bottom right
	};
	unsigned int koiIndices[] = {
		0, 1, 3,
		0, 2, 3
	};

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(koiVertices), koiVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(koiIndices), koiIndices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	myShader.use();
	glUniform1f(glGetUniformLocation(myShader.ID, "koiTexture"), 0);
	
	unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");
	mat4 trans = mat4(1.0f);

	unsigned int darkenLoc = glGetUniformLocation(myShader.ID, "darken");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		float t = glfwGetTime();
		trans = mat4(1.0f);
		trans = scale(trans, vec3(0.5f, 0.5f, 1.0f));
		trans = translate(trans, vec3(sin(t), cos(t), 0.0));
		trans = rotate(trans, -t + radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(trans));

		glClearColor(0.4f, 0.45f, 0.502f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, koiTexture);
		
		glBindVertexArray(VAO);

		glUniform1i(darkenLoc, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		trans = mat4(1.0f);
		//trans = rotate(trans, (float)M_PI, vec3(0.0f, 0.0f, 1.0f));
		trans = scale(trans, vec3(0.5f, 0.5f, 1.0f));
		trans = translate(trans, vec3(sin(t + M_PI), cos(t + M_PI), 0.0f));
		trans = rotate(trans, -t + radians(-270.0f), vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(trans));

		glUniform1i(darkenLoc, 1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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