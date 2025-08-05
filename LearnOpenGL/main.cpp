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
	projection = perspective(radians(60.0f), 1.0f, 0.1f, 100.0f);
	//projection = ortho(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 100.0f);

	myShader.use();
	glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, value_ptr(projection));

	// Setup vertex data/buffers and configure vertex attributes
	float x = 0.5f, y = 0.5f, z = 0.5f;
	float dx = 2.0f, dy = 2.0f, dz = 2.0f;
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

	array<pair<size_t, size_t>, 54> connections;

	size_t dim = 3;
	size_t i = 0;
	size_t current = 0;
	size_t neighbor = 0;
	for (unsigned int z = 0; z < dim; z++)
	{
		for (unsigned int y = 0; y < dim; y++)
		{
			for (unsigned int x = 0; x < dim; x++)
			{
				current = x + (y * dim) + (z * dim * dim);

				if (x + 1 < dim)
				{
					neighbor = (x + 1) + (y * dim) + (z * dim * dim);
					connections[i] = { current, neighbor };
					i++;
				}

				if (y + 1 < dim)
				{
					neighbor = x + ((y + 1) * dim) + (z * dim * dim);
					connections[i] = { current, neighbor };
					i++;
				}

				if (z + 1 < dim)
				{
					neighbor = x + (y * dim) + ((z + 1) * dim * dim);
					connections[i] = { current, neighbor };
					i++;
				}
			}
		}
	}

	for (size_t i = 0; i < connections.size(); i++)
	{
		vec3 a = positions[connections[i].first];
		vec3 b = positions[connections[i].second];
		cout << "Connection " << i << ":";
		cout << "\t" << connections[i].first << " (" << a.x << ", " << a.y << ", " << a.z << ") and ";
		cout << "\t" << connections[i].second << " (" << b.x << ", " << b.y << ", " << b.z << ")" << endl;
	}

	GLuint lineVAO, lineVBO;
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(connections), connections.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	unsigned int colorLoc = glGetUniformLocation(myShader.ID, "customColor");
	const vec3 bgColor = normalize(vec4(13.0f, 0.0f, 26.0f, 255.0f));
	const vec3 bgColor2 = normalize(vec4(59.0f, 57.0f, 42.0f, 255.0f));
	const vec3 startColor = normalize(vec4(110.0f, 81.0f, 129.0f, 255.0f));
	const vec3 endColor = normalize(vec4(108.0f, 237.0f, 237.0f, 255.0f));
	const vec3 thirdColor = normalize(vec4(108.0f, 185.0f, 201.0f, 255.0f));
	const vec3 fourthColor = normalize(vec4(111.0f, 29.0f, 92.0f, 255.0f));
	const vec3 fifthColor = normalize(vec4(82.0f, 122.0f, 118.0f, 255.0f));
	const vec3 sixthColor = normalize(vec4(115.0f, 48.0f, 62.0f, 255.0f));
	vec4 customColor = vec4(startColor, 1.0f);
	
	glLineWidth(2.0f);

	float sphereRadius = 8.0f, t = 0.0f, rotationAmount = 0.0f;
	vec3 spherePosition = vec3(0.0f), scaleAmount = vec3(1.0f), translation = vec3(0.0f), rotationAxis = vec3(0.0f);
	while (!glfwWindowShouldClose(window))
	{ 
		processInput(window);

		glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = mat4(1.0f);
		view = translate(view, vec3(0.0f, 0.0f, -40.0f));
		view = rotate(view, radians(30.0f), vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, value_ptr(view));

		myShader.use();
		t = glfwGetTime();

		// outer cubes
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			spherePosition = sphereRadius * normalize(positions[i]);
			rotationAxis = vec3(sin(t), 0.0f, cos(t));
			rotationAmount = radians(90.0f);
			if (i % 2 == 0)
			{
				customColor = vec4(fifthColor, 0.6f);
				translation = mix(positions[i], 1.5f * spherePosition, sin(half_pi<float>() * sin(t)) * pi<float>() / 9.0f + 1.0f);
			}
			else
			{
				customColor = vec4(sixthColor, 0.6f);
				translation = mix(positions[i], spherePosition, sin(half_pi<float>() * sin(t + pi<float>())) * pi<float>() / 9.0f + 1.0f);
			}

			glUniform4fv(colorLoc, 1, value_ptr(customColor));
			
			model = mat4(1.0f);
			model = rotate(model, rotationAmount, rotationAxis);
			model = translate(model, translation);
			model = scale(model, scaleAmount);
			glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, value_ptr(model));
			
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		
			customColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			glUniform4fv(colorLoc, 1, value_ptr(customColor));
		
			glBindVertexArray(lineVAO);
			glDrawArrays(GL_LINES, 0, 162);
		}

		// inner cubes
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			spherePosition = sphereRadius * normalize(positions[i]);
			rotationAxis = vec3(sin(t), 0.0f, cos(t));
			rotationAmount = radians(76.0f);
			if (i % 2 == 0)
			{
				customColor = vec4(fifthColor.r - 0.1f, fifthColor.g - 0.1f, fifthColor.b - 0.1f, 0.6f);
				translation = mix(positions[i], 1.5f * spherePosition, sin(half_pi<float>() * sin(t)) * pi<float>() / 9.0f + 1.0f);
			}
			else
			{
				customColor = vec4(sixthColor.r - 0.1f, sixthColor.g - 0.1f, sixthColor.b - 0.1f, 0.6f);
				translation = mix(positions[i], spherePosition, sin(half_pi<float>() * sin(t + pi<float>())) * pi<float>() / 9.0f + 1.0f);
			}
			
			glUniform4fv(colorLoc, 1, value_ptr(customColor));
			
			model = mat4(1.0f);
			model = rotate(model, rotationAmount, rotationAxis);
			model = translate(model, translation);
			model = scale(model, vec3(0.5f));
			glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, value_ptr(model));

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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