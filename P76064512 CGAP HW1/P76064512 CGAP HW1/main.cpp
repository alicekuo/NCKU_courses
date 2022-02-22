#define M_PI 3.14159265358979323846
#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void render(Shader shader, unsigned int VAO);
void makevertices(float* vertices, float R, float r);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// torus
const float R = 0.6f;
const float r = 0.2f;

const float _2PI = 2 * (float)M_PI;
const float rad = (float)M_PI / 180;
const int pn = (const int)(_2PI / rad) + 1;

int main() {
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	Shader shader("vshader.vert", "fshader.frag"); // you can name your shader files however you like

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float *vertices = new float[18 * pn * pn];
	makevertices(vertices, R, r);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 18 * pn * pn * 4, vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		shader.use();

		// render torus
		render(shader,VAO);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

void makevertices(float* vertices, float R, float r) {
	float u, v;
	int i = 0;
	for (u = 0.0f; u < _2PI; u += rad) {
		for (v = 0.0f; v < _2PI; v += rad) {
			vertices[i++] = (R + r * cos(u)) * cos(v);
			vertices[i++] = (R + r * cos(u)) * sin(v);
			vertices[i++] = r * sin(u);
			vertices[i++] = (R + r * cos(u)) * cos(v + rad);
			vertices[i++] = (R + r * cos(u)) * sin(v + rad);
			vertices[i++] = r * sin(u);
			vertices[i++] = (R + r * cos(u + rad)) * cos(v);
			vertices[i++] = (R + r * cos(u + rad)) * sin(v);
			vertices[i++] = r * sin(u + rad);
			vertices[i++] = (R + r * cos(u + rad)) * cos(v);
			vertices[i++] = (R + r * cos(u + rad)) * sin(v);
			vertices[i++] = r * sin(u + rad);
			vertices[i++] = (R + r * cos(u + rad)) * cos(v + rad);
			vertices[i++] = (R + r * cos(u + rad)) * sin(v + rad);
			vertices[i++] = r * sin(u + rad);
			vertices[i++] = (R + r * cos(u)) * cos(v + rad);
			vertices[i++] = (R + r * cos(u)) * sin(v + rad);
			vertices[i++] = r * sin(u);
		}
	}
}

void render(Shader shader, unsigned int VAO) {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * pn*pn);

	// view rotate.
	float time = (float)glfwGetTime() / 100.0f;
	glm::mat4 eye(1.0f);
	glm::mat4 model;
	model = glm::translate(eye, glm::vec3(0.0f))
		* glm::rotate(eye, 98.70f * time, glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::rotate(eye, 123.40f * time, glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}