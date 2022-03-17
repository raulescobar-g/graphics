#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdlib.h>  
#include <time.h> 
#include <string>

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
#include "Object.h"

using namespace std;

#define OBJECT_AMOUNT 100
#define MATERIAL_COUNT 100
#define X_BOUND 100.0f
#define Z_BOUND 100.0f
#define SCALE_BOUND 5.0f

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "./"; // Where the resources are loaded from

shared_ptr<Camera> camera;
shared_ptr<Program> prog_p;
shared_ptr<Shape> bunny;
shared_ptr<Shape> teapot;
shared_ptr<Shape> sphere;
shared_ptr<Shape> ground;

bool keyToggles[256] = {false}; // only for English keyboards!

vector<Material> materials;
vector<Light> lights;
vector<Object> objects;


static void error_callback(int error, const char *description){ cerr << description << endl;}
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){ if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, GL_TRUE);}}
static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
	double xmouse, ymouse;
	int width, height;
	glfwGetCursorPos(window, &xmouse, &ymouse);
	glfwGetWindowSize(window, &width, &height);
	if(action == GLFW_PRESS) { camera->mouseClicked((float)xmouse, (float)ymouse, (mods & GLFW_MOD_SHIFT) != 0 , (mods & GLFW_MOD_CONTROL) != 0 , (mods & GLFW_MOD_ALT) != 0);}
}

// This function is called when the mouse moves
static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse){
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if(state == GLFW_PRESS) { camera->mouseMoved((float)xmouse, (float)ymouse);}
}

static void char_callback(GLFWwindow *window, unsigned int key){
	keyToggles[key] = !keyToggles[key];
}

// If the window is resized, capture the new size and reset the viewport
static void resize_callback(GLFWwindow *window, int width, int height){ glViewport(0, 0, width, height);}

// This function is called once to initialize the scene and OpenGL
static void init(){
	glfwSetTime(0.0);	
	srand (time(NULL));
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
	prog_p->addUniform("l1");
	prog_p->addUniform("ka");
	prog_p->addUniform("kd");
	prog_p->addUniform("ks");
	prog_p->addUniform("s");
	prog_p->setVerbose(false);

	Light l1("lightPos1","l1", glm::vec3(100.0f, 50.0f, -100.0f), glm::vec3(0.8f, 0.8f, 0.8f));
	lights.push_back(l1);
	
	camera = make_shared<Camera>();
	camera->setInitDistance(2.0f); // Camera's initial Z translation
	
	bunny = make_shared<Shape>();
	bunny->loadMesh(RESOURCE_DIR + "bunny.obj");
	bunny->init();
	bunny->fitToUnitBox();

	teapot = make_shared<Shape>();
	teapot->loadMesh(RESOURCE_DIR + "teapot.obj");
	teapot->init();
	teapot->fitToUnitBox();

	sphere = make_shared<Shape>();
	sphere->loadMesh(RESOURCE_DIR + "sphere.obj");
	sphere->init();
	sphere->fitToUnitBox();

	ground = make_shared<Shape>();
	ground->loadMesh(RESOURCE_DIR + "square.obj");
	ground->init();
	ground->fitToUnitBox();

	for (int i = 0; i < MATERIAL_COUNT; ++i){
		materials.push_back(Material(glm::vec3((rand()%100)/100.0f,(rand()%100)/100.0f,(rand()%100)/100.0f),glm::vec3((rand()%100)/100.0f,(rand()%100)/100.0f,(rand()%100)/100.0f),glm::vec3((rand()%100)/100.0f,(rand()%100)/100.0f,(rand()%100)/100.0f),(float)(rand()%1000)));
	}

	shared_ptr<Shape> t = teapot;
	shared_ptr<Shape> b = bunny;
	for (int i = 0; i < OBJECT_AMOUNT; ++i){
		objects.push_back(Object(materials.size(), X_BOUND,Z_BOUND,SCALE_BOUND,i%2==0?t:b));
	}
	
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
	glm::mat4 iMV;
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->rotate(3.14159/4.0, 0.0f,1.0f,0.0f);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);

	prog_p->bind();

	for (Object& obj : objects){
		MV->pushMatrix();

			
			MV->rotate(obj.rotation,0.0f,1.0f,0.0f);
			MV->scale(obj.scale,obj.scale,obj.scale);
			MV->translate(obj.x, obj.y, obj.z);

			iMV = glm::inverse(glm::transpose(glm::mat4(MV->topMatrix())));

			
			glUniformMatrix4fv(prog_p->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog_p->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			glUniformMatrix4fv(prog_p->getUniform("iMV"), 1, GL_FALSE, glm::value_ptr(iMV));
			glUniform3f(prog_p->getUniform("ka"), materials[obj.mat].ka.x, materials[obj.mat].ka.y, materials[obj.mat].ka.z);
			glUniform3f(prog_p->getUniform("kd"), materials[obj.mat].kd.x, materials[obj.mat].kd.y, materials[obj.mat].kd.z);
			glUniform3f(prog_p->getUniform("ks"), materials[obj.mat].ks.x, materials[obj.mat].ks.y, materials[obj.mat].ks.z);
			glUniform1f(prog_p->getUniform("s"), materials[obj.mat].s );

			obj.shape->draw(prog_p); 	
			
		MV->popMatrix();
		
	}
	

	for (Light l : lights) {
		MV->pushMatrix();
			MV->translate(l.position.x, l.position.y, l.position.z);
			MV->scale(10.0f, 10.0f, 10.0f);

			iMV = glm::inverse(glm::transpose(glm::mat4(MV->topMatrix())));

			
			glUniformMatrix4fv(prog_p->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog_p->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			glUniformMatrix4fv(prog_p->getUniform("iMV"), 1, GL_FALSE, glm::value_ptr(iMV));

			

			glUniform3f(prog_p->getUniform("ka"), 0.9f, 0.9f, 0.0f);
			glUniform3f(prog_p->getUniform("kd"), 0.9f, 0.9f, 0.0f);
			glUniform3f(prog_p->getUniform("ks"), 0.9f, 0.9f, 0.0f);
			glUniform1f(prog_p->getUniform("s"),0.0f );

			glUniform3f(prog_p->getUniform(l.pos_name), l.position.x, l.position.y, l.position.z);
			glUniform3f(prog_p->getUniform(l.color_name), l.color.r, l.color.g, l.color.b);

			sphere->draw(prog_p);
		MV->popMatrix();
	}

	MV->pushMatrix();
		
		MV->translate(50.0f,0.0f,-50.0f);
		MV->scale(100.0f, 100.0f, 100.0f);
		MV->rotate(3.14159/2.0f,-1.0f,0.0f,0.0f);

		iMV = glm::inverse(glm::transpose(glm::mat4(MV->topMatrix())));
		
		glUniformMatrix4fv(prog_p->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog_p->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
		glUniformMatrix4fv(prog_p->getUniform("iMV"), 1, GL_FALSE, glm::value_ptr(iMV));

		glUniform3f(prog_p->getUniform("ka"), 0.3f, 0.9f, 0.3f);
		glUniform3f(prog_p->getUniform("kd"), 0.3f, 0.9f, 0.3f);
		glUniform3f(prog_p->getUniform("ks"), 0.1f, 0.3f, 0.1f);
		glUniform1f(prog_p->getUniform("s"),0.0f );
		
		ground->draw(prog_p);
	MV->popMatrix();

	prog_p->unbind();
	MV->popMatrix();
	P->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Usage: A3 RESOURCE_DIR" << endl;
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
