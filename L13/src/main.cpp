#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Camera.h"
#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"

using namespace std;
using namespace glm;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "./"; // Where the resources are loaded from

shared_ptr<Camera> camera;
shared_ptr<Program> progShape;
shared_ptr<Program> progSimple;

// Sphere and Ellipse
shared_ptr<Shape> shapeSphere;
vec3 sphere_T;
float sphere_S;
mat4 ellipsoid_E;

// Plane
shared_ptr<Shape> shapePlane;
vec3 plane_n;
vec3 plane_c;

class Hit
{
public:
	Hit() : x(0), n(0), t(0) {}
	Hit(const vec3 &x, const vec3 &n, float t) { this->x = x; this->n = n; this->t = t; }
	vec3 x; // position
	vec3 n; // normal
	float t; // distance
};

vec3 ray[2];
vector<Hit> hits;

bool keyToggles[256] = {false}; // only for English keyboards!

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
		
		if(shift) {
			hits.clear();
			
			// Create projection, view, and camera matrices
			auto MV = make_shared<MatrixStack>();
			MV->loadIdentity();
			camera->applyProjectionMatrix(MV);
			mat4 P = MV->topMatrix();
			MV->loadIdentity();
			camera->applyViewMatrix(MV);
			mat4 V = MV->topMatrix();
			mat4 C = inverse(V);
			
			// Task 1: Update ray
			// ray[0] = vec3(C[3][0], C[3][1], C[3][2]);
			// ray[1] = ?
			
			// Task 2: Find plane intersection
			
			
			// Task 3: Find sphere intersection(s)


			// Task 4: Find scaled/translated sphere intersection(s)
			
			
			// Task 5: Find ellipsoid intersection(s)
			
		}
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
	switch(key) {
	}
}

// If the window is resized, capture the new size and reset the viewport
static void resize_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
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

	progShape = make_shared<Program>();
	progShape->setShaderNames(RESOURCE_DIR + "shape_vert.glsl", RESOURCE_DIR + "shape_frag.glsl");
	progShape->setVerbose(true);
	progShape->init();
	progShape->addAttribute("aPos");
	progShape->addAttribute("aNor");
	progShape->addUniform("P");
	progShape->addUniform("MV");
	progShape->addUniform("MVit");
	progShape->setVerbose(false);

	progSimple = make_shared<Program>();
	progSimple->setShaderNames(RESOURCE_DIR + "simple_vert.glsl", RESOURCE_DIR + "simple_frag.glsl");
	progSimple->setVerbose(true);
	progSimple->init();
	progSimple->addAttribute("aPos");
	progSimple->addAttribute("aNor");
	progSimple->addUniform("P");
	progSimple->addUniform("MV");
	progSimple->setVerbose(false);
	
	camera = make_shared<Camera>();
	camera->setInitDistance(5.0f);
	
	// Ray: position and direction
	ray[0] = vec3(0.0f, 0.0f, 5.0f);
	ray[1] = vec3(0.0f, 0.0f, -1.0f);
	
	shapeSphere = make_shared<Shape>();
	shapeSphere->loadMesh(RESOURCE_DIR + "sphere.obj");
	shapeSphere->init();
	
	shapePlane = make_shared<Shape>();
	shapePlane->loadMesh(RESOURCE_DIR + "square.obj");
	shapePlane->init();
	
	// Plane parameters
	plane_n = vec3(0.0f, 0.0f, 1.0f);
	plane_c = vec3(0.0f, 0.0f, -1.0f);
	
	// Sphere parameters
	sphere_S = 0.7;
	sphere_T = vec3(0.8f, -0.6f, 0.2f);
	
	// Ellipsoid parameters
	auto M = make_shared<MatrixStack>();
	M->translate(0.5f, 1.0f, 0.0f);
	vec3 axis = normalize(vec3(1.0f, 1.0f, 1.0f));
	M->rotate(1.0f, axis);
	M->scale(3.0f, 1.0f, 0.5f);
	ellipsoid_E = M->topMatrix();
	//cout << to_string(ellipsoid_E) << endl;
	
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
	
	// Reset point and line sizes
	glPointSize(1.0f);
	glLineWidth(1.0f);
	
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	// Matrix stacks
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);
	
	// Bind program for shapes
	progShape->bind();
	glUniformMatrix4fv(progShape->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	
	// Draw plane (assume no rotation is needed)
	MV->pushMatrix();
	MV->translate(plane_c);
	MV->scale(5.0f);
	glUniformMatrix4fv(progShape->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniformMatrix4fv(progShape->getUniform("MVit"), 1, GL_FALSE, value_ptr(inverse(transpose(MV->topMatrix()))));
	shapePlane->draw(progShape);
	MV->popMatrix();
	
	// Draw 1st sphere
	MV->pushMatrix();
	glUniformMatrix4fv(progShape->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniformMatrix4fv(progShape->getUniform("MVit"), 1, GL_FALSE, value_ptr(inverse(transpose(MV->topMatrix()))));
	shapeSphere->draw(progShape);
	MV->popMatrix();
	
	// Draw 2nd sphere
	MV->pushMatrix();
	MV->translate(sphere_T);
	MV->scale(sphere_S);
	glUniformMatrix4fv(progShape->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniformMatrix4fv(progShape->getUniform("MVit"), 1, GL_FALSE, value_ptr(inverse(transpose(MV->topMatrix()))));
	shapeSphere->draw(progShape);
	MV->popMatrix();
	
	// Draw ellipsoid
	MV->pushMatrix();
	MV->multMatrix(ellipsoid_E);
	glUniformMatrix4fv(progShape->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniformMatrix4fv(progShape->getUniform("MVit"), 1, GL_FALSE, value_ptr(inverse(transpose(MV->topMatrix()))));
	shapeSphere->draw(progShape);
	MV->popMatrix();
	
	// Unbind program for shapes
	progShape->unbind();
	
	// Bind program for ray and hits
	progSimple->bind();
	
	// Draw ray
	glUniformMatrix4fv(progSimple->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	glUniformMatrix4fv(progSimple->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(&ray[0][0]);
	vec3 ray1 = ray[0] + 100.0f * ray[1];
	glVertex3fv(&ray1[0]);
	glEnd();
	
	// Draw hits
	glPointSize(10.0f);
	glLineWidth(2.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	for(auto hit : hits) {
		glBegin(GL_POINTS);
		glVertex3fv(&hit.x[0]);
		glEnd();
		glBegin(GL_LINES);
		glVertex3fv(&hit.x[0]);
		vec3 x1 = hit.x + 0.5f*hit.n;
		glVertex3fv(&x1[0]);
		glEnd();
	}
	
	// Unbind program for ray and hits
	progSimple->unbind();
	
	MV->popMatrix();
	P->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");

	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "YOUR NAME", NULL, NULL);
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
