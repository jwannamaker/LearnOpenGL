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
	float dx = 1.0f, dy = 1.0f, dz = 1.0f;
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

	GLfloat lineVertices[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 7.0f, 0.0f, // y-axis

		0.0f, 0.0f, 0.0f,
		7.0f, 0.0f, 0.0f, // x-axis

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 7.0f, // z-axis
	};

	GLuint lineVAO, lineVBO;
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	unsigned int colorLoc = glGetUniformLocation(myShader.ID, "customColor");
	const vec3 bgColor = normalize(vec4(13.0f, 0.0f, 26.0f, 255.0f));
	const vec3 startColor = normalize(vec4(110.0f, 81.0f, 129.0f, 255.0f));
	const vec3 endColor = normalize(vec4(108.0f, 237.0f, 237.0f, 255.0f));
	const vec3 thirdColor = normalize(vec4(108.0f, 185.0f, 201.0f, 255.0f));
	const vec3 fourthColor = normalize(vec4(111.0f, 29.0f, 92.0f, 255.0f));
	vec4 customColor = vec4(startColor, 1.0f);

	float sphereRadius = 7.0f, t = 0.0f, rotationAmount = 0.0f;
	vec3 spherePosition = vec3(0.0f), scaleAmount = vec3(1.0f), translation = vec3(0.0f), rotationAxis = vec3(0.0f);
	while (!glfwWindowShouldClose(window))
	{ 
		processInput(window);

		glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(lineVAO);
		
		myShader.use();
		t = glfwGetTime();
		view = mat4(1.0f);
		view = translate(view, vec3(0.0f, 0.0f, -40.0f));
		view = rotate(view, radians(15.0f), vec3(1.0f, 0.0f, 0.0f));
		//view = rotate(view, radians(90.0f), vec3(cos(t), 1.0f, sin(t)));
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, value_ptr(view));
		
		rotationAxis = vec3(sin(t), 0.0f, cos(t));
		rotationAmount = radians(90.0f);

		model = mat4(1.0f);
		model = rotate(model, rotationAmount, rotationAxis);
		
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, value_ptr(model));
		
		glLineWidth(2.0f);
		
		// y-axis: red
		customColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glUniform4fv(colorLoc, 1, value_ptr(customColor));
		glDrawArrays(GL_LINES, 0, 2);

		// x-axis: green
		customColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
		glUniform4fv(colorLoc, 1, value_ptr(customColor));
		glDrawArrays(GL_LINES, 2, 2);

		// z-axis: blue
		customColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
		glUniform4fv(colorLoc, 1, value_ptr(customColor));
		glDrawArrays(GL_LINES, 4, 2);

		glBindVertexArray(VAO);
		
		// outer cubes
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			spherePosition = sphereRadius * normalize(positions[i]);
			if (i % 2 == 0)
			{
				customColor = vec4(mix(thirdColor, endColor, sin(t) + 1.0f), 0.2f);
				
				translation = 1.3f * spherePosition;
				//translation = mix(positions[i], 1.3f * spherePosition, sin(t) + 1.5f);
				rotationAmount = radians(90.0f);
				scaleAmount = vec3(1.0f);
			}
			else
			{
				customColor = vec4(mix(startColor, endColor, sin(t) + 1.0f), 0.6f);
				
				translation = spherePosition;
				//translation = mix(positions[i], spherePosition, sin(t) + 1.5f);
				rotationAmount = 0.0f;
				//scaleAmount = vec3(cos(t) + 1.0f);
			}

			glUniform4fv(colorLoc, 1, value_ptr(customColor));
			
			model = mat4(1.0f);
			model = rotate(model, rotationAmount, rotationAxis);
			model = translate(model, translation);
			model = scale(model, scaleAmount);
			glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, value_ptr(model));
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// inner cubes
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			spherePosition = 0.75f * sphereRadius * normalize(positions[i]);
			if (i % 2 == 0)
			{
				customColor = vec4(mix(fourthColor, startColor, sin(t) + 1.0f), 0.2f);
				
				translation = 1.25f * spherePosition;
				//translation = mix(positions[i], 1.25f * spherePosition, sin(t) + 1.0f);
				rotationAmount = radians(90.0f);
				scaleAmount = vec3(1.0f);
			}
			else
			{
				customColor = vec4(mix(endColor, startColor, sin(t) + 1.0f), 0.6f);
				
				translation = spherePosition;
				//translation = mix(positions[i], spherePosition, sin(t) + 1.0f);
				rotationAmount = 0.0f;
				//scaleAmount = vec3(cos(t) + 1.0f);
			}
			
			glUniform4fv(colorLoc, 1, value_ptr(customColor));
			
			model = mat4(1.0f);
			model = rotate(model, rotationAmount, rotationAxis);
			model = translate(model, translation);
			model = scale(model, scaleAmount);
			glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, value_ptr(model));

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
