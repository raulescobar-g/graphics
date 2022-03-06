#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <unordered_map>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Camera.h"
#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"
#include "Material.h"
#include "Light.h"
#include <string>

using namespace std;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "./"; // Where the resources are loaded from
bool OFFLINE = false;

shared_ptr<Camera> camera;
shared_ptr<Program> prog_p;
shared_ptr<Program> prog_n;
shared_ptr<Program> prog_s;
shared_ptr<Program> prog_c;
shared_ptr<Shape> shape;
shared_ptr<Shape> tp;

bool keyToggles[256] = {false}; // only for English keyboards!

int prog_pointer;
int prog_amount;

int mat_pointer;
int mat_amount;

int light_pointer;
int light_amount;

vector<Material> materials;
vector<Light> lights;


// This function is called when a GLFW error occurs
static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

// This function is called when a key is pressed
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

// This function is called when the mouse is clicked
static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	// Get the current mouse position.
	double xmouse, ymouse;
	glfwGetCursorPos(window, &xmouse, &ymouse);
	// Get current window size.
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if(action == GLFW_PRESS) {
		bool shift = (mods & GLFW_MOD_SHIFT) != 0;
		bool ctrl  = (mods & GLFW_MOD_CONTROL) != 0;
		bool alt   = (mods & GLFW_MOD_ALT) != 0;
		camera->mouseClicked((float)xmouse, (float)ymouse, shift, ctrl, alt);
	}
}

// This function is called when the mouse moves
static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if(state == GLFW_PRESS) {
		camera->mouseMoved((float)xmouse, (float)ymouse);
	}
}

static void char_callback(GLFWwindow *window, unsigned int key)
{
	keyToggles[key] = !keyToggles[key];

	switch ((char)key) {
		case 's':
			prog_pointer += 1;
			break;
		case 'S':
			prog_pointer -= 1;
			break;
		case 'm':
			mat_pointer += 1;
			break;
		case 'M':
			mat_pointer -= 1;
			break;
		case 'l':
			light_pointer += 1;
			break;
		case 'L':
			light_pointer -= 1;
			break;
		case 'x':
			lights[light_pointer % light_amount].position.x += 0.1;
			break;
		case 'X':
			lights[light_pointer % light_amount].position.x -= 0.1;
			break;
		case 'y':
			lights[light_pointer % light_amount].position.y += 0.1;
			break;
		case 'Y':
			lights[light_pointer % light_amount].position.y -= 0.1;
			break;
	}
	if (prog_pointer == 2) {prog_pointer = 100000;}
	if (mat_pointer < 10 && mat_pointer%mat_amount == 0) {mat_pointer = mat_amount*10000;}
}

// If the window is resized, capture the new size and reset the viewport
static void resize_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/
static void saveImage(const char *filepath, GLFWwindow *w)
{
	int width, height;
	glfwGetFramebufferSize(w, &width, &height);
	GLsizei nrChannels = 3;
	GLsizei stride = nrChannels * width;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * height;
	std::vector<char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
	stbi_flip_vertically_on_write(true);
	int rc = stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
	if(rc) {
		cout << "Wrote to " << filepath << endl;
	} else {
		cout << "Couldn't write to " << filepath << endl;
	}
}

// This function is called once to initialize the scene and OpenGL
static void init()
{
	// Initialize time.
	glfwSetTime(0.0);
	
	// Set background color.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	prog_p = make_shared<Program>();
	prog_p->setShaderNames(RESOURCE_DIR + "phong_vert.glsl", RESOURCE_DIR + "phong_frag.glsl");
	prog_p->setVerbose(true);
	prog_p->init();
	prog_p->addAttribute("aPos");
	prog_p->addAttribute("aNor");
	prog_p->addUniform("MV");
	prog_p->addUniform("iMV");
	prog_p->addUniform("P");
	prog_p->addUniform("lightPos1");
	prog_p->addUniform("lightPos2");
	prog_p->addUniform("l1");
	prog_p->addUniform("l2");
	prog_p->addUniform("ka");
	prog_p->addUniform("kd");
	prog_p->addUniform("ks");
	prog_p->addUniform("s");
	prog_p->setVerbose(false);

	prog_n = make_shared<Program>();
	prog_n->setShaderNames(RESOURCE_DIR + "normal_vert.glsl", RESOURCE_DIR + "normal_frag.glsl");
	prog_n->setVerbose(true);
	prog_n->init();
	prog_n->addAttribute("aPos");
	prog_n->addAttribute("aNor");
	prog_n->addUniform("MV");
	prog_n->addUniform("P");
	prog_n->setVerbose(false);

	prog_s = make_shared<Program>();
	prog_s->setShaderNames(RESOURCE_DIR + "silhouette_vert.glsl", RESOURCE_DIR + "silhouette_frag.glsl");
	prog_s->setVerbose(true);
	prog_s->init();
	prog_s->addAttribute("aPos");
	prog_s->addAttribute("aNor");
	prog_s->addUniform("MV");
	prog_s->addUniform("P");
	prog_s->addUniform("iMV");
	prog_s->setVerbose(false);

	prog_c = make_shared<Program>();
	prog_c->setShaderNames(RESOURCE_DIR + "cel_vert.glsl", RESOURCE_DIR + "cel_frag.glsl");
	prog_c->setVerbose(true);
	prog_c->init();
	prog_c->addAttribute("aPos");
	prog_c->addAttribute("aNor");
	prog_c->addUniform("MV");
	prog_c->addUniform("iMV");
	prog_c->addUniform("P");
	prog_c->addUniform("lightPos1");
	prog_c->addUniform("lightPos2");
	prog_c->addUniform("l1");
	prog_c->addUniform("l2");
	prog_c->addUniform("ka");
	prog_c->addUniform("kd");
	prog_c->addUniform("ks");
	prog_c->addUniform("s");
	prog_c->setVerbose(false);

	prog_amount = 4;
	prog_pointer = prog_amount * 100000;

	Light l1("lightPos1","l1", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.8f, 0.8f, 0.8f));
	Light l2("lightPos2","l2", glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.2f, 0.2f, 0.0f));
	lights = {l1, l2};
	light_pointer = 200000;
	light_amount = lights.size();
	
	camera = make_shared<Camera>();
	camera->setInitDistance(2.0f); // Camera's initial Z translation
	
	shape = make_shared<Shape>();
	shape->loadMesh(RESOURCE_DIR + "bunny.obj");
	shape->init();

	tp = make_shared<Shape>();
	tp->loadMesh(RESOURCE_DIR + "teapot.obj");
	tp->init();

	
	// edit these to get desired mats
	Material normal(glm::vec3(0.2f,0.2f,0.2f),glm::vec3(0.8f,0.7f,0.7f),glm::vec3(1.0f,0.9f,0.8f),200.0f);
	Material deep_blue(glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.1f,0.2f,0.8f),glm::vec3(0.05f,0.95f,0.05f),200.0f);
	Material chalky(glm::vec3(0.02f,0.02f,0.02f),glm::vec3(0.6f,0.6f,0.65f),glm::vec3(0.01f,0.01f,0.02f),0.0f);

	materials = {normal, deep_blue, chalky};
	mat_pointer = materials.size() * 100000;
	mat_amount = materials.size();
	
	GLSL::checkError(GET_FILE_LINE);
}

// This function is called every frame to draw the scene.
static void render()
{
	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(keyToggles[(unsigned)'c']) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if(keyToggles[(unsigned)'z']) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	double t = glfwGetTime();
	if(!keyToggles[(unsigned)' ']) {
		// Spacebar turns animation on/off
		t = 0.0f;
	}

	// Matrix stacks
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);

	
	MV->pushMatrix();

	MV->translate(-0.5f, -0.5f, 0.0f);
	MV->rotate(t,0.0f,1.0f,0.0f);
	MV->scale(0.5f,0.5f,0.5f);

	glm::mat4 iMV = glm::inverse(glm::transpose(glm::mat4(MV->topMatrix())));

	
	switch (prog_pointer % prog_amount){
		case 0:
			prog_n->bind();
			glUniformMatrix4fv(prog_n->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog_n->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			shape->draw(prog_n);
			prog_n->unbind();
			break;
		case 1:
			prog_p->bind();
			glUniformMatrix4fv(prog_p->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog_p->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			glUniformMatrix4fv(prog_p->getUniform("iMV"), 1, GL_FALSE, glm::value_ptr(iMV));

			for (Light l : lights) {
				glUniform3f(prog_p->getUniform(l.pos_name), l.position.x, l.position.y, l.position.z);
				glUniform3f(prog_p->getUniform(l.color_name), l.color.r, l.color.g, l.color.b);
			}

			glUniform3f(prog_p->getUniform("ka"), materials[mat_pointer % mat_amount].ka.x, materials[mat_pointer % mat_amount].ka.y, materials[mat_pointer % mat_amount].ka.z);
			glUniform3f(prog_p->getUniform("kd"), materials[mat_pointer % mat_amount].kd.x, materials[mat_pointer % mat_amount].kd.y, materials[mat_pointer % mat_amount].kd.z);
			glUniform3f(prog_p->getUniform("ks"), materials[mat_pointer % mat_amount].ks.x, materials[mat_pointer % mat_amount].ks.y, materials[mat_pointer % mat_amount].ks.z);
			glUniform1f(prog_p->getUniform("s"), materials[mat_pointer % mat_amount].s );

			shape->draw(prog_p);
			prog_p->unbind();
			break;
		case 2:
			prog_s->bind();
			glUniformMatrix4fv(prog_s->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog_s->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			glUniformMatrix4fv(prog_s->getUniform("iMV"), 1, GL_FALSE, glm::value_ptr(iMV));
			shape->draw(prog_s);
			prog_s->unbind();
			break;
		case 3:
			prog_c->bind();
			glUniformMatrix4fv(prog_c->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog_c->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			glUniformMatrix4fv(prog_c->getUniform("iMV"), 1, GL_FALSE, glm::value_ptr(iMV));

			for (Light l : lights) {
				glUniform3f(prog_c->getUniform(l.pos_name), l.position.x, l.position.y, l.position.z);
				glUniform3f(prog_c->getUniform(l.color_name), l.color.r, l.color.g, l.color.b);
			}

			glUniform3f(prog_c->getUniform("ka"), materials[mat_pointer % mat_amount].ka.x, materials[mat_pointer % mat_amount].ka.y, materials[mat_pointer % mat_amount].ka.z);
			glUniform3f(prog_c->getUniform("kd"), materials[mat_pointer % mat_amount].kd.x, materials[mat_pointer % mat_amount].kd.y, materials[mat_pointer % mat_amount].kd.z);
			glUniform3f(prog_c->getUniform("ks"), materials[mat_pointer % mat_amount].ks.x, materials[mat_pointer % mat_amount].ks.y, materials[mat_pointer % mat_amount].ks.z);
			glUniform1f(prog_c->getUniform("s"), materials[mat_pointer % mat_amount].s );

			shape->draw(prog_c);
			prog_c->unbind();
			break;
	}
	
	MV->popMatrix();
	MV->pushMatrix();

	MV->translate(0.5f, 0.0f, 0.0f);
	MV->rotate(3.14159,glm::vec3(0.0f, 1.0f, 0.0f));
	MV->scale(0.5f, 0.5f, 0.5f);

	glm::mat4 S(1.0f);
	S[0][1] = -0.5f*cos(t);
	MV->multMatrix(S);

	iMV = glm::inverse(glm::transpose(glm::mat4(MV->topMatrix())));
	
	switch (prog_pointer % prog_amount){
		case 0:
			prog_n->bind();
			glUniformMatrix4fv(prog_n->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog_n->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			tp->draw(prog_n);
			prog_n->unbind();
			break;
		case 1:
			prog_p->bind();
			glUniformMatrix4fv(prog_p->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog_p->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			glUniformMatrix4fv(prog_p->getUniform("iMV"), 1, GL_FALSE, glm::value_ptr(iMV));

			for (Light l : lights) {
				glUniform3f(prog_p->getUniform(l.pos_name), l.position.x, l.position.y, l.position.z);
				glUniform3f(prog_p->getUniform(l.color_name), l.color.r, l.color.g, l.color.b);
			}

			glUniform3f(prog_p->getUniform("ka"), materials[mat_pointer % mat_amount].ka.x, materials[mat_pointer % mat_amount].ka.y, materials[mat_pointer % mat_amount].ka.z);
			glUniform3f(prog_p->getUniform("kd"), materials[mat_pointer % mat_amount].kd.x, materials[mat_pointer % mat_amount].kd.y, materials[mat_pointer % mat_amount].kd.z);
			glUniform3f(prog_p->getUniform("ks"), materials[mat_pointer % mat_amount].ks.x, materials[mat_pointer % mat_amount].ks.y, materials[mat_pointer % mat_amount].ks.z);
			glUniform1f(prog_p->getUniform("s"), materials[mat_pointer % mat_amount].s );

			tp->draw(prog_p);
			prog_p->unbind();
			break;
		case 2:
			prog_s->bind();
			glUniformMatrix4fv(prog_s->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog_s->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			glUniformMatrix4fv(prog_s->getUniform("iMV"), 1, GL_FALSE, glm::value_ptr(iMV));
			tp->draw(prog_s);
			prog_s->unbind();
			break;
		case 3:
			prog_c->bind();
			glUniformMatrix4fv(prog_c->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog_c->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			glUniformMatrix4fv(prog_c->getUniform("iMV"), 1, GL_FALSE, glm::value_ptr(iMV));

			for (Light l : lights) {
				glUniform3f(prog_c->getUniform(l.pos_name), l.position.x, l.position.y, l.position.z);
				glUniform3f(prog_c->getUniform(l.color_name), l.color.r, l.color.g, l.color.b);
			}

			glUniform3f(prog_c->getUniform("ka"), materials[mat_pointer % mat_amount].ka.x, materials[mat_pointer % mat_amount].ka.y, materials[mat_pointer % mat_amount].ka.z);
			glUniform3f(prog_c->getUniform("kd"), materials[mat_pointer % mat_amount].kd.x, materials[mat_pointer % mat_amount].kd.y, materials[mat_pointer % mat_amount].kd.z);
			glUniform3f(prog_c->getUniform("ks"), materials[mat_pointer % mat_amount].ks.x, materials[mat_pointer % mat_amount].ks.y, materials[mat_pointer % mat_amount].ks.z);
			glUniform1f(prog_c->getUniform("s"), materials[mat_pointer % mat_amount].s );

			tp->draw(prog_c);
			prog_c->unbind();
			break;
	}
	MV->popMatrix();
	MV->popMatrix();
	P->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
	
	if(OFFLINE) {
		saveImage("output.png", window);
		GLSL::checkError(GET_FILE_LINE);
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Usage: A3 RESOURCE_DIR" << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");
	
	// Optional argument
	if(argc >= 3) {
		OFFLINE = atoi(argv[2]) != 0;
	}

	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "Raul Escobar", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	GLSL::checkVersion();
	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	// Set char callback.
	glfwSetCharCallback(window, char_callback);
	// Set cursor position callback.
	glfwSetCursorPosCallback(window, cursor_position_callback);
	// Set mouse button callback.
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// Set the window resize call back.
	glfwSetFramebufferSizeCallback(window, resize_callback);
	// Initialize scene.
	init();
	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(window)) {
		// Render scene.
		render();
		// Swap front and back buffers.
		glfwSwapBuffers(window);
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
