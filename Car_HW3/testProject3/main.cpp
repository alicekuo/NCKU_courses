#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#define M_PI 3.14159265358979323846

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
GLFWwindow* createWindow();
void push(glm::mat4 model);
glm::mat4 pop();
void draw(Shader shader, Mesh mesh_car, Mesh mesh_car1, Mesh mesh_tires, Mesh mesh_track, float v);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

vector<glm::mat4> stack;

void makeverticesTires(vector<float> *vertices, float R, float r);
void makeindicesTires(vector<unsigned int> *indices);
void makeverticesTrack(vector<float> *vertices, float a, float b);
void makeindicesTrack(vector<unsigned int> *indices);
const float R = 0.2f;
const float r = 0.1f;
const float M_2PI = (float)2 * M_PI;
const float rad = (float)M_PI / 180;
const int px = (const int)(M_2PI / rad) + 1;

const float a = 36.0f;
const float b = 12.0f;

float rv = glm::radians(130.0f);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = createWindow();
	
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

	Shader shader("car.vert","car.frag");
	// car
	vector<float> vertices = {
		-1.5f,  -0.5f, -0.7f,  // top right
		1.5f,  -0.5f, -0.7f,  // bottom right
		0.5f,  0.5f, -0.7f,  // bottom left
		-0.5f,  0.5f, -0.7f,   // top left 
		-0.5f,  0.5f, 0.7f,
		-1.5f,  -0.5f, 0.7f,
		1.5f,  -0.5f, 0.7f,
		0.5f,  0.5f, 0.7f,

		-1.8f, -1.0f, -0.7f,
		2.5f, -1.0f, -0.7f,
		2.5f, -0.5f, -0.7f,
		-1.8f, -0.5f, -0.7f,
		-1.8f, -0.5f, 0.7f,
		-1.8f, -1.0f, 0.7f,
		2.5f, -1.0f, 0.7f,
		2.5f, -0.5f, 0.7f
	};

	vector<unsigned int> indices = {
		0, 1, 2,  // first Triangle
		2, 3, 0,   // second Triangle
		0, 3, 4,
		4, 5, 0,
		0, 5, 6,
		6, 1, 0,
		7, 6, 5,
		5, 4, 7,
		7, 4, 3,
		3, 2, 7,
		7, 2, 1,
		1, 6, 7,

		8, 9, 10,  // first Triangle
		10, 11, 8,   // second Triangle
		8, 11, 12,
		12, 13, 8,
		8, 13, 14,
		14, 9, 8,
		15, 14, 13,
		13, 12, 15,
		15, 12, 11,
		11, 10, 15,
		15, 10, 9,
		9, 14, 15
	};

	vector<float> vertices1 = {
		-1.48f,  -0.48f, -0.71f,  // top right
		1.48f,  -0.48f, -0.71f,  // bottom right
		0.48f,  0.48f, -0.71f,  // bottom left
		-0.51f,  0.48f, -0.71f,   // top left 
		-0.51f,  0.48f, 0.7f,
		-1.51f,  -0.48f, 0.7f,
		1.51f,  -0.48f, 0.7f,
		//0.5f,  0.5f, 0.5f,

		//-1.8f, -0.5f, -0.5f,
		//2.5f, -0.5f, -0.5f,
		//2.5f, -0.5f, -0.5f,
		//-1.8f, -0.5f, -0.5f,
		//-1.8f, -0.5f, 0.5f,
		//-1.8f, -0.5f, 0.5f,
		//2.5f, -0.8f, 0.5f,
		//2.5f, -0.5f, 0.5f
	};



	vector<unsigned int> indices1 = {
		0, 1, 2,  // first Triangle
		2, 3, 0,   // second Triangle
		0, 3, 4,
		4, 5, 0,
		//0, 5, 6,
		//6, 1, 0,
		//7, 6, 5,
		//5, 4, 7,
		//7, 4, 3,
		//3, 2, 7,
		//7, 2, 1,
		//1, 6, 7,

		//8, 9, 10,  // first Triangle
		//10, 11, 8,   // second Triangle
		//8, 11, 12,
		//12, 13, 8,
		//8, 13, 14,
		//14, 9, 8,
		//15, 14, 13,
		//13, 12, 15,
		//15, 12, 11,
		//11, 10, 15,
		//15, 10, 9,
		//9, 14, 15
	};

	Mesh mesh_car(vertices, indices);
	Mesh mesh_car1(vertices1, indices1);
	// Tires
	vector<float> vertices_tires;
	vertices_tires.reserve(3 * px * px);
	makeverticesTires(&vertices_tires, R, r);

	vector<unsigned int> indices_tires;
	indices_tires.reserve(px * (px - 1) * 6);
	makeindicesTires(&indices_tires);

	Mesh mesh_tires(vertices_tires, indices_tires);

	// Track
	vector<float> vertices_track;
	vertices_track.reserve(10 * (px + 1));
	makeverticesTrack(&vertices_track, a, b);
	 
	vector<unsigned int> indices_track;
	indices_track.reserve(3 * 10 * (px + 1));
	makeindicesTrack(&indices_track);

	Mesh mesh_track(vertices_track, indices_track);
	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glm::radians(0.0f)
	//(float)SCR_WIDTH / (float)SCR_HEIGHT
	//shader.use();
	//glm::mat4 projection = glm::perspective(glm::radians(1.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//shader.setMat4("projection", projection);

	//shader.setVec3("ourColor", 1.0f, 1.0f, 1.0f);
	// render loop
	//glm::mat4 model = glm::mat4(0.0f);
	float v = 0;
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		draw(shader, mesh_car, mesh_car1, mesh_tires, mesh_track,v);
		v += rad/2;
		if (v > M_2PI)
			v = 0;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

GLFWwindow* createWindow() {
	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
	return window;
}

void makeverticesTires(vector<float> *vertices, float R, float r) {
	float u, v;
	int i = 0;
	for (u = 0.0f; u < M_2PI; u += rad) {
		for (v = 0.0f; v < M_2PI; v += rad) {
			vertices->push_back((R + r * cos(u)) * cos(v));
			vertices->push_back((R + r * cos(u)) * sin(v));
			vertices->push_back(r * sin(u));
		}
	}
}

void makeindicesTires(vector<unsigned int> *indices) {
	int vi = 0;
	int vj = px;
	int count = 0;
	int vj_max = (px * px) - 1;
	int i = 0;
	for (; count < px; count++) {
		vi = px * count;
		vj = vi + px;
		if (vj>vj_max)
			vj = 0;
		int vi_max = vj - 1;
		if (vj == 0)
			vi_max = vi + px - 1;
		for (; vi < vi_max;) {
			indices->push_back(vi);
			indices->push_back(vi + 1);
			indices->push_back(vj);
			vi++;

			indices->push_back(vi);
			indices->push_back(vj + 1);
			indices->push_back(vj);
			vj++;
		}
	}
}

void makeverticesTrack(vector<float> *vertices, float a, float b) {
	float v;
	int i = 0;
	for (v = 0.0f; v < M_2PI; v += 2 * rad) {
		vertices->push_back(a * cos(v));
		vertices->push_back(b * sin(v));
		vertices->push_back(0);
		vertices->push_back(a / 1.5f * cos(v));
		vertices->push_back(b / 2.3f * sin(v));
		vertices->push_back(0);
		vertices->push_back(a * cos(v + rad));
		vertices->push_back(b * sin(v + rad));
		vertices->push_back(0);
		vertices->push_back(a / 1.5f * cos(v + rad));
		vertices->push_back(b / 2.3f * sin(v + rad));
		vertices->push_back(0);
	}
}

void makeindicesTrack(vector<unsigned int> *indices) {
	int vi = 0;
	int vj = vi + 1;
	int vj_max = 10 * (px + 1) - 1;
	int i = 0;
	for (; vj < vj_max; vj++) {
		indices->push_back(vi++);
		indices->push_back(vi++);
		indices->push_back(vi);
		vi = vj;
	}
	indices->push_back(vi++);
	indices->push_back(vi);
	indices->push_back(0);
	vi = vj;
	indices->push_back(vi);
	indices->push_back(0);
	indices->push_back(1);

}

void push(glm::mat4 model) {
	stack.push_back(model);
}

glm::mat4 pop() {
	glm::mat4 model = stack[stack.size()-1];
	stack.pop_back();
	return model;
}

void draw(Shader shader,Mesh mesh_car, Mesh mesh_car1, Mesh mesh_tires, Mesh mesh_track,float v) {
	// rendering commands here
	glClearColor(0.2f, 1.0f, 0.5f, 0.3f);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float dat = 0.5f;
	float x =  (a / 1.3f * cos(v));
	float y =  (b / 1.7f * sin(v));

	if (x > 9 && y < 5) {
		rv += rad;
		if (rv >= glm::radians(180.0f))
			rv = glm::radians(180.0f);
	}
	else if (x < -9 && y > -5) {
		rv += rad;
		if (rv >= glm::radians(360.0f))
			rv = glm::radians(360.0f);
	}
	else if ((x > 0 && y > 0) || (x < 0 && y > 0))
		rv = glm::radians(180.0f);
	else if ((x < 0 && y < 0) || (x > 0 && y < 0))
		rv = 0.0f;
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	
	// camera/view transformation
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	// draw our first triangle
	push(model);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f + dat));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(x, y, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -rv, glm::vec3(0.0f, 1.0f, 0.0f));
	shader = Shader("car.vert", "car.frag");
	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);
	//shader.setVec3("ourColor", sin(glfwGetTime()), 1.0f, cos(glfwGetTime()));
	shader.setVec3("ourColor", 0.6f, 0.6f, 0.9f);
	mesh_car.Draw();
	//model = pop();

	//---------------------------------------- test
	//shader = Shader("car.vert", "car.frag");
	//shader.use();
	//shader.setMat4("projection", projection);
	//shader.setMat4("view", view);
	//shader.setMat4("model", model);
	////shader.setVec3("ourColor", sin(glfwGetTime()), 1.0f, cos(glfwGetTime()));
	//shader.setVec3("ourColor", 0.9f, 0.9f, 0.9f);
	//mesh_car1.Draw();

	//----------------------------------------
	shader = Shader("tires.vert", "tires.frag");
	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("ourColor", 0.1f, 0.1f, 0.1f);
	float tp_rv = 0.0f;
	if (rv != glm::radians(360.0f) && rv != glm::radians(180.0f))
		tp_rv = rv / 10;
	push(model);
	model = glm::translate(model, glm::vec3(-1.3f, -3.0f + 1.75f, -0.6f));
	model = glm::rotate(model, -tp_rv, glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	mesh_tires.Draw();
	model = pop();

	//----------------------------------------
	push(model);
	model = glm::translate(model, glm::vec3(-1.3f, -3.0f + 1.75f, 0.6f));
	model = glm::rotate(model, -tp_rv, glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	mesh_tires.Draw();
	model = pop();

	//----------------------------------------
	push(model);
	model = glm::translate(model, glm::vec3(2.0f, -3.0f + 1.75f, -0.6f));
	model = glm::rotate(model, -tp_rv, glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	mesh_tires.Draw();
	model = pop();

	//----------------------------------------
	push(model);
	model = glm::translate(model, glm::vec3(2.0f, -3.0f + 1.75f, 0.6f));
	model = glm::rotate(model, -tp_rv, glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	mesh_tires.Draw();
	model = pop();
	model = pop();
	//----------------------------------------
	shader = Shader("track.vert", "track.frag");
	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("ourColor", 0.3f, 0.2f, 0.0f);

	push(model);
	model = glm::translate(model, glm::vec3(0.0f, -3.5f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f,0.0f,0.0f));
	shader.setMat4("model", model);
	mesh_track.Draw();
	model = pop();
}