#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
	GLfloat vertices[] = {
		// positions		        // colors					        // texture coords	
		0.71f,	0.71f,  0.0f,	  219.0f, 209.0f, 180.0f,		1.0f, 1.0f, // top right
		-0.71f, 0.71f,  0.0f,	  209.0f, 173.0f, 130.0f,		0.0f, 1.0f, // top left
		-0.71f,	-0.71f, 0.0f,	  152.0f, 166.0f, 129.0f,		0.0f, 0.0f,	// bottom left
		0.71f,	-0.71f, 0.0f,	  106.0f, 148.0f, 144.0f,		1.0f, 0.0f, // bottom right
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// Setup buffers and array attributes
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
  
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// texture coordinate attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	stbi_set_flip_vertically_on_load(true);
	
	// Setup textures
	unsigned int texture0, texture1;

	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int imageWidth, imageHeight, numChannels;
	unsigned char* imageData = stbi_load("container.jpg", &imageWidth, &imageHeight, &numChannels, 0);
	if (!imageData)
	{
		cout << "Failed to load texture" << endl;
		return -1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	imageData = stbi_load("awesomeface.png", &imageWidth, &imageHeight, &numChannels, 0);
	if (!imageData)
	{
		cout << "Failed to load texture" << endl;
		return -1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);

	myShader.use();
	myShader.setInt("texture0", 0);
	myShader.setInt("texture1", 1);

	float mixAmount = 0.2f;
  
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			mixAmount = (mixAmount - 0.0001f >= 0.0f) ? mixAmount - 0.0001f : mixAmount;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			mixAmount = (mixAmount + 0.0001f <= 1.0f) ? mixAmount + 0.0001f : mixAmount;
		}
		myShader.setFloat("mixAmount", mixAmount);

		glClearColor(0.4f, 0.45f, 0.502f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
		glBindVertexArray(VAO);
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
