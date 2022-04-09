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

using namespace std;

#define OBJECT_AMOUNT 100
#define MATERIAL_COUNT 100
#define MOVEMENT_SPEED 5.0f
#define GRAVITY 10.0
#define JUMP_SPEED 5.0
#define SENSITIVITY 0.005
#define SUN_SIZE 0.02
#define SPIN_SPEED 1.0

#define MINIMAP_SIZE 0.5

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "./"; // Where the resources are loaded from

shared_ptr<Camera> camera;

shared_ptr<Program> prog_p;
shared_ptr<Program> prog_vase;
shared_ptr<Program> curr_prog;

shared_ptr<Shape> bunny;
shared_ptr<Shape> teapot;
shared_ptr<Shape> sphere;
shared_ptr<Shape> gen_sphere;
shared_ptr<Shape> vase;
shared_ptr<Shape> ground;

bool keyToggles[256] = {false}; // only for English keyboards!
bool inputs[256] = {false}; // only for English keyboards!

vector< shared_ptr<Material> > materials;
vector< shared_ptr<Material> > light_materials;
shared_ptr<Light> lights;
vector<Object> objects;

// could not think of a better way to initialize these values probably bad practice
double o_x = 0.0;			
double o_y = 0.0;
float t, jump_t, dt;

static void error_callback(int error, const char *description) { cerr << description << endl; }

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, GL_TRUE);} 	// tried to keep these in one line per key to not litter the file
	if (key == GLFW_KEY_W) inputs[(unsigned)'w'] = action != GLFW_RELEASE ? true : false;				//value has a default of false. gets set to true if action is not release and it will only be set to false if action is release
	if (key == GLFW_KEY_S) inputs[(unsigned)'s'] = action != GLFW_RELEASE ? true : false;
	if (key == GLFW_KEY_D) inputs[(unsigned)'d'] = action != GLFW_RELEASE ? true : false;
	if (key == GLFW_KEY_A) inputs[(unsigned)'a'] = action != GLFW_RELEASE ? true : false;
	
	if (key == GLFW_KEY_Z && (mods == GLFW_MOD_SHIFT || inputs[(unsigned) 'Z'])) inputs[(unsigned)'Z'] = action != GLFW_RELEASE ? true : false;
	if (key == GLFW_KEY_Z && mods != GLFW_MOD_SHIFT && !inputs[(unsigned) 'Z']) inputs[(unsigned)'z'] = action != GLFW_RELEASE ? true : false;

	if (key == GLFW_KEY_SPACE && action != GLFW_RELEASE && !inputs[(unsigned)' ']) { // allows you to hold down spacebars
		inputs[(unsigned)' '] = true;
		jump_t = glfwGetTime();
	}
	
}

// This function is called when the mouse moves
static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse){

	float xdiff = (xmouse - o_x) * SENSITIVITY;
	float ydiff = (ymouse - o_y) * SENSITIVITY;

	camera->yaw -= xdiff;
	camera->pitch -= ydiff;

	if (camera->pitch > glm::pi<float>()/3.0f) { camera->pitch = glm::pi<float>()/3.0f;}
	if (camera->pitch < -glm::pi<float>()/3.0f) { camera->pitch = -glm::pi<float>()/3.0f;}
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

	prog_vase = make_shared<Program>();
	prog_vase->setShaderNames(RESOURCE_DIR + "vase_vert.glsl", RESOURCE_DIR + "phong_frag.glsl");
	prog_vase->setVerbose(true);
	prog_vase->init();
	prog_vase->addAttribute("aPos");
	prog_vase->addUniform("t");
	prog_vase->addUniform("MV");
	prog_vase->addUniform("iMV");
	prog_vase->addUniform("P");
	prog_vase->addUniform("lightPos");
	prog_vase->addUniform("lightCol");
	prog_vase->addUniform("ke");
	prog_vase->addUniform("kd");
	prog_vase->addUniform("ks");
	prog_vase->addUniform("s");
	prog_vase->setVerbose(false);

	lights = make_shared<Light>("lightPos", "lightCol");
	
	camera = make_shared<Camera>();
	
	bunny = make_shared<Shape>();
	bunny->loadMesh(RESOURCE_DIR + "bunny.obj");
	bunny->fitToUnitBox();
	bunny->init();
	bunny->set_id("bunny");
	

	teapot = make_shared<Shape>();
	teapot->loadMesh(RESOURCE_DIR + "teapot.obj");
	teapot->fitToUnitBox();
	teapot->init();
	teapot->set_id("teapot");
	

	gen_sphere = make_shared<Shape>();
	gen_sphere->createMesh("gen_sphere", 20);
	gen_sphere->fitToUnitBox();
	gen_sphere->init();
	gen_sphere->set_id("gen_sphere");

	vase = make_shared<Shape>();
	vase->createMesh("vase", 20);
	//vase->fitToUnitBox(); breaks the shape for some reason
	vase->init();
	vase->set_id("vase");

	sphere = make_shared<Shape>();
	sphere->loadMesh(RESOURCE_DIR + "sphere.obj");
	sphere->fitToUnitBox();
	sphere->init();
	sphere->set_id("sphere");
	

	ground = make_shared<Shape>();
	ground->loadMesh(RESOURCE_DIR + "square.obj");
	ground->fitToUnitBox();
	ground->init();
	ground->set_id("ground");


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

	
	for (int i = 0; i < OBJECT_AMOUNT; ++i){
		if (i % 4 == 0) {
			objects.push_back(Object(materials[i],teapot));
		} else if (i % 4 == 1) { 
			objects.push_back(Object(materials[i], bunny));
		} else if (i % 4 == 2){
			objects.push_back(Object(materials[i], gen_sphere));
		} else {
			objects.push_back(Object(materials[i], vase));
		}
	}

	objects.push_back( Object(make_shared<Material>(glm::vec3(0.0f,  0.0f,  0.0f), glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.5f,  0.5f,  0.5f), 0.0f), ground, glm::vec3(0.0f,0.0f,0.0f), 10.0f, glm::vec3(-1.0f, 0.0f, 0.0f)));

	for (int i= 0; i < LIGHT_AMOUNT; ++i){
		Object temp(light_materials[i],sphere);
		temp.x = lights->position[i].x;
		temp.y = lights->position[i].y;
		temp.z = lights->position[i].z;
		temp.scale = SUN_SIZE;
		objects.push_back(temp);
	}

	
	GLSL::checkError(GET_FILE_LINE);
}

static void input_handling() {
	dt = glfwGetTime() - t;
	t = glfwGetTime();
	glm::vec3 buff(0.0f,0.0f,0.0f);

	if (inputs[(unsigned)'w']) {buff += glm::normalize(glm::vec3( sin(camera->yaw)*cos(camera->pitch) , 0.0f, cos(camera->yaw)*cos(camera->pitch))); }
	if (inputs[(unsigned)'s']) {buff -= glm::normalize(glm::vec3( sin(camera->yaw)*cos(camera->pitch) , 0.0f, cos(camera->yaw)*cos(camera->pitch))); }
	if (inputs[(unsigned)'a']) {buff -= glm::normalize(glm::cross(glm::vec3( sin(camera->yaw)*cos(camera->pitch) , 0.0f, cos(camera->yaw)*cos(camera->pitch)), glm::vec3(0.0f, 1.0f, 0.0f)));}
	if (inputs[(unsigned)'d']) {buff += glm::normalize(glm::cross(glm::vec3( sin(camera->yaw)*cos(camera->pitch) , 0.0f, cos(camera->yaw)*cos(camera->pitch)), glm::vec3(0.0f, 1.0f, 0.0f)));}

	if (glm::length(buff) > 0.00001f || glm::length(buff) < -0.00001f) camera->pos += glm::normalize(buff) * MOVEMENT_SPEED * dt; // keeps the movement the same speed even if moving diagonally by summing direction of movement vectors and normalizing

	// jumping capabilities :p
	if (inputs[(unsigned)' ']) {
		float _t = t - jump_t; // make the time start at the moment when the jump began
		float dy = (JUMP_SPEED  -  GRAVITY * _t) * dt;
		if (camera->pos.y + dy >= 0.1f) {
			camera->pos.y += dy; // adding the position differential derived from the kinematic equation of an object in free fall
		} else {
			inputs[(unsigned)' '] = false;
			camera->pos.y = 0.1f;
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

		for (Object& obj : objects){ //loops over objects except lights and ground
			MV->pushMatrix();
				
				float shear_q = 1.0f;
				glm::mat4 S(1.0f);
				S[1][0] = shear_q*cos(t*2.0);

				if (obj.shape->get_id() == "bunny")  obj.spin(SPIN_SPEED * dt);

				MV->translate(obj.x, obj.y, obj.z);
				MV->scale(obj.scale,obj.scale,obj.scale);
				MV->rotate(obj.rotation,obj.axis);

				if (obj.shape->get_id() == "vase") {MV->scale(0.1f,0.1f,0.1f);}

				if (obj.shape->get_id() == "gen_sphere") {
					float Ay = 1.3f;
					float As = 0.5f;
					float p = 1.7f;
					float t0 = 0.9f;
					float y = Ay * (0.5f * sin((2.0f * glm::pi<float>() / p) * (t + t0)) + 0.5f);
					float s = -As * (0.5f * cos((4.0f * glm::pi<float>() / p) * (t + t0)) + 0.5f) + 1.0f;

					
					MV->translate(0.0f, y, 0.0f);
					MV->scale(s,1.0f,s);
				}

				if (obj.shape->get_id() == "teapot") MV->translate((shear_q / 4.0f)*cos(t*2.0), 0.0f,0.0f);
				if (obj.shape->get_id() == "teapot") MV->multMatrix(S);
				
				
				
				iMV = glm::transpose(glm::inverse(glm::mat4(MV->topMatrix())));
				
				curr_prog = obj.shape->get_id() == "vase" ? prog_vase : prog_p;

				curr_prog->bind();
				glUniform3fv(curr_prog->getUniform(lights->pos_name), LIGHT_AMOUNT, value_ptr(lights->world_positions[0]));
				if (obj.shape->get_id() == "vase"){ glUniform1f(curr_prog->getUniform("t"), t); }
				glUniformMatrix4fv(curr_prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
				glUniformMatrix4fv(curr_prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
				glUniformMatrix4fv(curr_prog->getUniform("iMV"), 1, GL_FALSE, glm::value_ptr(iMV));
				glUniform3f(curr_prog->getUniform("ke"), obj.material->ke.x, obj.material->ke.y, obj.material->ke.z);
				glUniform3f(curr_prog->getUniform("kd"), obj.material->kd.x, obj.material->kd.y, obj.material->kd.z);
				glUniform3f(curr_prog->getUniform("ks"), obj.material->ks.x, obj.material->ks.y, obj.material->ks.z);
				glUniform1f(curr_prog->getUniform("s"), obj.material->s );
				glUniform3fv(curr_prog->getUniform(lights->pos_name), LIGHT_AMOUNT, value_ptr(lights->world_positions[0]));
				glUniform3fv(curr_prog->getUniform(lights->color_name), LIGHT_AMOUNT, value_ptr(lights->color[0]));
				obj.shape->draw(curr_prog);
				curr_prog->unbind(); 	
				
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
	window = glfwCreateWindow(640, 480, "Raul Escobar", NULL, NULL);

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
