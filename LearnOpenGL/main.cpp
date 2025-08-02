#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <cstring>
#include <iostream>

#define M_PI acos(-1.0)

using namespace std;
using namespace glm;

const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 1000;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path, int* imageWidth, int* imageHeight);
float triangleWave(float time, float amplitude, float period, float verticalOffset, float phaseOffset);

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

	// Setup model, view, and projection matrices
	mat4 model = mat4(1.0f), view = mat4(1.0f), projection = mat4(1.0f);
	view = rotate(view, radians(-65.0f), vec3(1.0f, 0.0f, 0.0f));
	view = translate(view, vec3(0.0f, 40.0f, -20.0f));
	projection = ortho(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 100.0f);

	myShader.use();
	glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, value_ptr(projection));

	// Setup vertex data/buffers and configure vertex attributes
	float x = 1.0f, y = 1.0f, z = 1.0f;
	float dx = 3.0f, dy = 3.0f, dz = 3.0f;
	vec3 center = vec3(0.0f, 0.0f, 0.0f);
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
	array<vec3, 27> positions = {
		vec3(center.x -   dx, center.y -   dy, center.z -   dz),
		vec3(center.x + 0.0f, center.y -   dy, center.z -   dz),
		vec3(center.x +   dx, center.y -   dy, center.z -   dz),
		vec3(center.x -   dx, center.y + 0.0f, center.z -   dz),
		vec3(center.x + 0.0f, center.y + 0.0f, center.z -   dz),
		vec3(center.x +   dx, center.y + 0.0f, center.z -   dz),
		vec3(center.x -   dx, center.y +   dy, center.z -   dz),
		vec3(center.x + 0.0f, center.y +   dy, center.z -   dz),
		vec3(center.x +   dx, center.y +   dy, center.z -   dz),

		vec3(center.x -   dx, center.y -   dy, center.z + 0.0f),
		vec3(center.x + 0.0f, center.y -   dy, center.z + 0.0f),
		vec3(center.x +   dx, center.y -   dy, center.z + 0.0f),
		vec3(center.x -   dx, center.y + 0.0f, center.z + 0.0f),
		vec3(center.x + 0.0f, center.y + 0.0f, center.z + 0.0f),
		vec3(center.x +   dx, center.y + 0.0f, center.z + 0.0f),
		vec3(center.x -   dx, center.y +   dy, center.z + 0.0f),
		vec3(center.x + 0.0f, center.y +   dy, center.z + 0.0f),
		vec3(center.x +   dx, center.y +   dy, center.z + 0.0f),

		vec3(center.x -   dx, center.y -   dy, center.z +   dz),
		vec3(center.x + 0.0f, center.y -   dy, center.z +   dz),
		vec3(center.x +   dx, center.y -   dy, center.z +   dz),
		vec3(center.x -   dx, center.y + 0.0f, center.z +   dz),
		vec3(center.x + 0.0f, center.y + 0.0f, center.z +   dz),
		vec3(center.x +   dx, center.y + 0.0f, center.z +   dz),
		vec3(center.x -   dx, center.y +   dy, center.z +   dz),
		vec3(center.x + 0.0f, center.y +   dy, center.z +   dz),
		vec3(center.x +   dx, center.y +   dy, center.z +   dz),
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	unsigned int colorLoc = glGetUniformLocation(myShader.ID, "customColor");
	vec4 customColor = vec4(0.0f);

	float positionScale = 0.0f, positionCheck = 0.0f, t = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		
		myShader.use();
		t = glfwGetTime();
		view = mat4(1.0f);
		view = rotate(view, 2.0f, vec3(cos(1.5f * t), 0.0f, sin(1.5f * t)));
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			model = mat4(1.0f);
			positionScale = 1.0f;
			positionCheck = abs(positions[i].x) + abs(positions[i].y) + abs(positions[i].z);
			customColor = vec4(1.0f);
			// outer corners
			if (positionCheck == abs(dx) + abs(dy) + abs(dz))
			{
				positionScale = triangleWave(t, 1.25f, 10.0f * half_pi<float>(), 2.25f, half_pi<float>() * 1.0f);
			}
			// outer edges
			if (positionCheck == abs(dx) + abs(dy) || positionCheck == abs(dx) + abs(dz) || positionCheck == abs(dy) + abs(dz))
			{
				positionScale = triangleWave(t, 1.5f, 10.0f * half_pi<float>(), 2.0f, half_pi<float>() * 1.0f);
			}
			// outer faces
			if (positionCheck == abs(dx) || positionCheck == abs(dy) || positionCheck == abs(dz))
			{
				positionScale = triangleWave(t, 2.0f, 10.0f * half_pi<float>(), 3.0f, half_pi<float>() * 1.0f);
			}

			vec3 newPosition = view * vec4(positions[i], 1.0f);
			customColor = vec4(newPosition, positionScale * 0.1f);
			model = translate(model, positions[i] * positionScale);
			glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, value_ptr(model));
			glUniform4fv(colorLoc, 1, value_ptr(customColor));
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
	}

	if (strstr(path, ".png"))
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *imageWidth, *imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	}
	if (strstr(path, ".jpg") || strstr(path, ".jpeg"))
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *imageWidth, *imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);

	return textureID;
}

float triangleWave(float time, float amplitude, float period, float verticalOffset, float phaseOffset)
{
	float a = 2.0f * amplitude / pi<float>();
	float k = 2.0f * pi<float>() / period;
	return a * asin(sin(k * time + phaseOffset)) + verticalOffset;
}
