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
#include "Texture.h"
#include "Chunk.h"

using namespace std;

#define OBJECT_AMOUNT 1
#define MATERIAL_COUNT 1
#define GRAVITY 10.0
#define JUMP_SPEED 5.0
#define SENSITIVITY 0.005


#define MINIMAP_SIZE 0.5

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "./"; // Where the resources are loaded from

shared_ptr<Camera> camera;

shared_ptr<Program> prog_p;

shared_ptr<Shape> cube;
shared_ptr<Shape> sphere;

shared_ptr<Chunk> chunk;

bool keyToggles[256] = {false}; // only for English keyboards!
bool inputs[256] = {false}; // only for English keyboards!

vector< shared_ptr<Material> > materials;
vector< shared_ptr<Material> > light_materials;
shared_ptr<Light> lights;
shared_ptr<Object> obj;

// could not think of a better way to initialize these values probably bad practice
double o_x = 0.0;			
double o_y = 0.0;
float t, jump_t, dt;

static void error_callback(int error, const char *description) { cerr << description << endl; }

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, GL_TRUE);} 	// tried to keep these in one line per key to not litter the file
	if (key == GLFW_KEY_W) inputs[(unsigned)'w'] = action != GLFW_RELEASE;				//value has a default of false. gets set to true if action is not release and it will only be set to false if action is release
	if (key == GLFW_KEY_S) inputs[(unsigned)'s'] = action != GLFW_RELEASE;
	if (key == GLFW_KEY_D) inputs[(unsigned)'d'] = action != GLFW_RELEASE;
	if (key == GLFW_KEY_A) inputs[(unsigned)'a'] = action != GLFW_RELEASE;
	
	if (key == GLFW_KEY_Z && (mods == GLFW_MOD_SHIFT || inputs[(unsigned) 'Z'])) inputs[(unsigned)'Z'] = action != GLFW_RELEASE;
	if (key == GLFW_KEY_Z && mods != GLFW_MOD_SHIFT && !inputs[(unsigned) 'Z']) inputs[(unsigned)'z'] = action != GLFW_RELEASE;

	if (key == GLFW_KEY_SPACE && action != GLFW_RELEASE && !inputs[(unsigned)' ']) { // allows you to hold down spacebars
		inputs[(unsigned)' '] = true;
		jump_t = glfwGetTime();
	}
	
}

// This function is called when the mouse moves
static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse){

	float xdiff = (xmouse - o_x) * SENSITIVITY;
	float ydiff = (ymouse - o_y) * SENSITIVITY;

	camera->dyaw(-xdiff);
	camera->dpitch(-ydiff);

	if (camera->get_pitch() > glm::pi<float>()/3.0f) { camera->set_pitch(glm::pi<float>()/3.0f);}
	else if (camera->get_pitch() < -glm::pi<float>()/3.0f) { camera->set_pitch(-glm::pi<float>()/3.0f);}
	o_x = xmouse;
	o_y = ymouse;
}

static void char_callback(GLFWwindow *window, unsigned int key){keyToggles[key] = !keyToggles[key];}

// If the window is resized, capture the new size and reset the viewport
static void resize_callback(GLFWwindow *window, int width, int height){ glViewport(0, 0, width, height);}

// This function is called once to initialize the scene and OpenGL
static void init(){
	glfwSetTime(0.0);	
	srand (time(NULL));
	glClearColor(0.8f, 0.8f, 0.99f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	prog_p = make_shared<Program>();
	prog_p->setShaderNames(RESOURCE_DIR + "phong_vert.glsl", RESOURCE_DIR + "phong_frag.glsl");
	prog_p->setVerbose(true);
	prog_p->init();
	prog_p->addAttribute("aPos");
	prog_p->addAttribute("aNor");
	prog_p->addUniform("MV");
	prog_p->addUniform("iMV");
	prog_p->addUniform("P");
	prog_p->addUniform("lightPos");
	prog_p->addUniform("lightCol");
	prog_p->addUniform("ke");
	prog_p->addUniform("kd");
	prog_p->addUniform("ks");
	prog_p->addUniform("s");
	prog_p->setVerbose(false);

	lights = make_shared<Light>("lightPos", "lightCol");
	
	camera = make_shared<Camera>();

	cube = make_shared<Shape>();
	cube->loadMesh(RESOURCE_DIR + "cube.obj");
	cube->fitToUnitBox();
	cube->init();
	cube->set_id("cube");

	sphere = make_shared<Shape>();
	sphere->loadMesh(RESOURCE_DIR + "sphere.obj");
	sphere->fitToUnitBox();
	sphere->init();
	sphere->set_id("sphere");


	for (int i = 0; i < MATERIAL_COUNT; ++i){ 
		materials.push_back(
			make_shared<Material>( glm::vec3(0.0f,0.0f,0.0f), glm::vec3((rand()%101)/100.0f,  (rand()%101)/100.0f,  (rand()%101)/100.0f), glm::vec3(1.0f,  1.0f,  1.0f), 10.0f)
		);
	}
	for (int i = 0; i < LIGHT_AMOUNT; ++i){ 
		light_materials.push_back(
			make_shared<Material>( lights->color[i] , glm::vec3(0.0f,  0.0f,  0.0f), glm::vec3(0.0f,  0.0f,  0.0f), 1.0f)
		);
	}

	
	chunk = std::make_shared<Chunk>();
	obj = std::make_shared<Object>(materials[0], cube);

	
	GLSL::checkError(GET_FILE_LINE);
}

static void input_handling() {
	dt = glfwGetTime() - t;
	t = glfwGetTime();

	camera->move(inputs[(unsigned)'w'], inputs[(unsigned)'a'], inputs[(unsigned)'s'], inputs[(unsigned)'d'], dt);

	// jumping capabilities :p
	if (inputs[(unsigned)' ']) {
		float _t = t - jump_t; // make the time start at the moment when the jump began
		float dy = (JUMP_SPEED  -  GRAVITY * _t) * dt;
		if (camera->y() + dy >= 1.0f) {
			camera->dy(dy); // adding the position differential derived from the kinematic equation of an object in free fall
		} else {
			inputs[(unsigned)' '] = false;
			camera->y(1.0f);
		}
	}

	if (inputs[(unsigned) 'z']){camera->decrement_fovy();}
	if (inputs[(unsigned) 'Z']){camera->increment_fovy();}
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

	glViewport(0, 0, width, height);
	
	
	// Matrix stacks
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	glm::mat4 iMV;
	
	P->pushMatrix();	
	camera->applyProjectionMatrix(P);
	camera->applyViewMatrix(MV);	
	MV->pushMatrix();

		

		for (int i = 0; i < LIGHT_AMOUNT; ++i) { // calc world coords for all lights
			lights->world_positions[i] = MV->topMatrix() * glm::vec4(lights->position[i],1.0f);
		}
		int layer = CHUNK_WIDTH * CHUNK_WIDTH;
		for (int i = 0; i < CHUNK_VOLUME; ++i){ //loops over ALL chunky monkey
			if (chunk->map[i] == 'a') continue;
			MV->pushMatrix();
				glm::vec3 pos((i % layer) / CHUNK_WIDTH, i / layer, (i % layer) % CHUNK_WIDTH);
				MV->translate(pos);
				MV->scale(obj->scale);
								
				iMV = glm::transpose(glm::inverse(glm::mat4(MV->topMatrix())));
				

				prog_p->bind();
				glUniform3fv(prog_p->getUniform(lights->pos_name), LIGHT_AMOUNT, value_ptr(lights->world_positions[0]));
				glUniformMatrix4fv(prog_p->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
				glUniformMatrix4fv(prog_p->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
				glUniformMatrix4fv(prog_p->getUniform("iMV"), 1, GL_FALSE, glm::value_ptr(iMV));
				glUniform3f(prog_p->getUniform("ke"), obj->material->ke.x, obj->material->ke.y, obj->material->ke.z);
				glUniform3f(prog_p->getUniform("kd"), obj->material->kd.x, obj->material->kd.y, obj->material->kd.z);
				glUniform3f(prog_p->getUniform("ks"), obj->material->ks.x, obj->material->ks.y, obj->material->ks.z);
				glUniform1f(prog_p->getUniform("s"), obj->material->s );
				glUniform3fv(prog_p->getUniform(lights->pos_name), LIGHT_AMOUNT, value_ptr(lights->world_positions[0]));
				glUniform3fv(prog_p->getUniform(lights->color_name), LIGHT_AMOUNT, value_ptr(lights->color[0]));
				obj->shape->draw(prog_p);
				prog_p->unbind(); 	
				
			MV->popMatrix();
			
		}


	MV->popMatrix();	
	P->popMatrix();
	
	
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Usage: A5 RESOURCE_DIR" << endl;
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
	// GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	// const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	window = glfwCreateWindow(640,480, "Raul Escobar", NULL, NULL);

	//glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

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
	// glfwSetMouseButtonCallback(window, mouse_button_callback);
	// Set the window resize call back.
	glfwSetFramebufferSizeCallback(window, resize_callback);

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // THIS MAKES THE CURSOR DISAPPEAR

	// Initialize scene.
	init();
	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(window)) {
		// update position and camera
		input_handling();
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
