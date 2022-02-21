#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"
#include "Tree.h"
#include "Node.h"
#include <math.h> 

using namespace std;

GLFWwindow *window; // Main application window
string RES_DIR = ""; // Where data files live
shared_ptr<Program> prog;
shared_ptr<Program> progIM; // immediate mode

shared_ptr<Shape> cube;
shared_ptr<Shape> sphere;

Tree * robot;
Node * current;


static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

static void character_callback(GLFWwindow* window, unsigned int codepoint) {

	switch (codepoint) {
		case 'X':
			current->rotate(-0.1,0.0,0.0);
			break;
		case 'Y':
			current->rotate(0.0,-0.1,0.0);
			break;
		case 'Z':
			current->rotate(0.0,0.0,-0.1);
			break;
		case 'x':
			current->rotate(0.1,0.0,0.0);
			break;
		case 'y':
			current->rotate(0.0,0.1,0.0);
			break;
		case 'z':
			current->rotate(0.0,0.0,0.1);
			break;
		case '.':
		current = robot->next();
			break;
		case ',':
		current = robot->prev();
			break;		
	}
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);	
}

static void init()
{
	GLSL::checkVersion();

	// Check how many texture units are supported in the vertex shader
	int tmp;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &tmp);
	cout << "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = " << tmp << endl;
	// Check how many uniforms are supported in the vertex shader
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &tmp);
	cout << "GL_MAX_VERTEX_UNIFORM_COMPONENTS = " << tmp << endl;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &tmp);
	cout << "GL_MAX_VERTEX_ATTRIBS = " << tmp << endl;

	// Set background color.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	// Initialize meshes.
	cube = make_shared<Shape>();
	sphere = make_shared<Shape>();

	cube->loadMesh(RES_DIR + "cube.obj");
	sphere->loadMesh(RES_DIR + "sphere.obj");

	cube->init();
	sphere->init();
	
	// Initialize the GLSL programs.
	prog = make_shared<Program>();
	prog->setVerbose(true);
	prog->setShaderNames(RES_DIR + "nor_vert.glsl", RES_DIR + "nor_frag.glsl");
	prog->init();
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->addAttribute("aPos");
	prog->addAttribute("aNor");
	prog->setVerbose(false);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = width/(float)height;
	glViewport(0, 0, width, height);

	// _CONSTANTS FOR EASIER EDITING______________________________________________________
	float distance_camera_to_body = 15.0;												
																						
	float torso_length = 2.5;
	float torso_width = 1.0; 												

	float head_length = 0.6; 												

	float arm_length = torso_length * 0.6;
	float arm_width = torso_length * 0.2;

	float leg_length = torso_length * 0.6;
	float leg_width = torso_width * 0.4;

	float forearm_length = arm_length*0.7;
	float forearm_width = arm_width *0.7;

	float lower_leg_length = leg_length * 0.5;
	float lower_leg_width = leg_width * 0.8;

	//___________________________________________________________________________________|

	//TORSO INITIAL VALUES
	glm::vec3 torso_joint_t(0.0,0.0,-1 * distance_camera_to_body);
	glm::vec3 torso_joint_a(0.0,0.0,0.0);
	glm::vec3 torso_mesh_t(0.0,0.0, 0.0);
	glm::vec3 torso_scale(torso_width, torso_length , torso_width);

	robot = new Tree(torso_joint_t, torso_joint_a, torso_mesh_t, torso_scale, cube); //THIS IS THE TORSO, THUS ALSO THE ROOT WHICH WAS DECLARED GLOBALLY AT THE TOP OF THE FILE
	current = robot->get_root(); //this is for traversing the tree also declared at the top of the file
		// indentation for readability

		//HEAD INITIAL VALUES
		glm::vec3 head_mesh_t(0.0, head_length / 2.0,0.0);
		glm::vec3 head_joint_a(0.0,0.0,0.0);
		glm::vec3 head_joint_t(0.0, torso_length/2.0 ,0.0);
		glm::vec3 head_scale(head_length,head_length,head_length);
		Node * head = new Node(head_joint_t, head_joint_a, head_mesh_t, head_scale, cube);
		robot->push_child(head, robot->get_root());//Add the head as child of torso

		//LEFT ARM INITIAL VALUES
		glm::vec3 left_arm_mesh_t(-arm_length/2.0, 0.0, 0.0);
		glm::vec3 left_arm_joint_a(0.0,0.0,0.0);
		glm::vec3 left_arm_joint_t(-torso_width/2.0, torso_length/2.0 - arm_width/2.0, 0.0);
		glm::vec3 left_arm_scale(arm_length, arm_width, arm_width);
		Node * left_arm = new Node(left_arm_joint_t, left_arm_joint_a, left_arm_mesh_t, left_arm_scale, cube);

			//LEFT FOREARM INITIAL VALUES
			glm::vec3 left_forearm_mesh_t(-forearm_length/2.0, 0.0, 0.0);
			glm::vec3 left_forearm_joint_a(0.0,0.0,0.0);
			glm::vec3 left_forearm_joint_t(-arm_length,0.0,0.0);
			glm::vec3 left_forearm_scale(forearm_length, forearm_width, forearm_width);
			Node * left_forearm = new Node(left_forearm_joint_t, left_forearm_joint_a, left_forearm_mesh_t, left_forearm_scale, cube);
			
		robot->push_child(left_arm, robot->get_root());//Add the arm as child of torso
		robot->push_child(left_forearm, left_arm);//Add the forearm as child of arm

		//RIGHT ARM INITIAL VALUES
		glm::vec3 right_arm_mesh_t(arm_length/2, 0.0, 0.0);
		glm::vec3 right_arm_joint_a(0.0,0.0,0.0);
		glm::vec3 right_arm_joint_t(torso_width/2.0, torso_length/2.0 - arm_width/2.0, 0.0);
		glm::vec3 right_arm_scale(arm_length, arm_width, arm_width);
		Node * right_arm = new Node(right_arm_joint_t, right_arm_joint_a, right_arm_mesh_t, right_arm_scale, cube);

			//RIGHT FOREARM INITIAL VALUES
			glm::vec3 right_forearm_mesh_t(forearm_length/2.0, 0.0, 0.0);
			glm::vec3 right_forearm_joint_a(0.0,0.0,0.0);
			glm::vec3 right_forearm_joint_t(arm_length,0.0,0.0);
			glm::vec3 right_forearm_scale(forearm_length, forearm_width, forearm_width);
			Node * right_forearm = new Node(right_forearm_joint_t, right_forearm_joint_a, right_forearm_mesh_t, right_forearm_scale, cube);
			
		robot->push_child(right_arm, robot->get_root());//Add the arm as child of torso
		robot->push_child(right_forearm, right_arm);//Add the forearm as child of arm

		//LEFT LEG INITIAL VALUES
		glm::vec3 left_leg_mesh_t(0.0, -leg_length/2.0, 0.0);
		glm::vec3 left_leg_joint_a(0.0,0.0,0.0);
		glm::vec3 left_leg_joint_t(-torso_width/2.0 + leg_width/2.0, -torso_length/2.0, 0.0);
		glm::vec3 left_leg_scale(leg_width, leg_length, leg_width);
		Node * left_leg = new Node(left_leg_joint_t, left_leg_joint_a, left_leg_mesh_t, left_leg_scale, cube);

			//LEFT LOWER LEG INITIAL VALUES
			glm::vec3 left_lower_leg_mesh_t(0.0, -lower_leg_length/2.0, 0.0);
			glm::vec3 left_lower_leg_joint_a(0.0,0.0,0.0);
			glm::vec3 left_lower_leg_joint_t(0.0,-leg_length,0.0);
			glm::vec3 left_lower_leg_scale(lower_leg_width, lower_leg_length, lower_leg_width);
			Node * left_lower_leg = new Node(left_lower_leg_joint_t, left_lower_leg_joint_a, left_lower_leg_mesh_t, left_lower_leg_scale, cube);
			
		robot->push_child(left_leg, robot->get_root());//Add the leg as child of torso
		robot->push_child(left_lower_leg, left_leg);//Add the forearm as child of arm

		//RIGHT LEG INITIAL VALUES
		glm::vec3 right_leg_mesh_t(0.0, -leg_length/2.0, 0.0);
		glm::vec3 right_leg_joint_a(0.0,0.0,0.0);
		glm::vec3 right_leg_joint_t(torso_width/2.0 - leg_width/2.0, -torso_length/2.0, 0.0);
		glm::vec3 right_leg_scale(leg_width, leg_length, leg_width);
		Node * right_leg = new Node(right_leg_joint_t, right_leg_joint_a, right_leg_mesh_t, right_leg_scale, cube);

			//RIGHT LOWER LEG INITIAL VALUES
			glm::vec3 right_lower_leg_mesh_t(0.0, -lower_leg_length/2.0, 0.0);
			glm::vec3 right_lower_leg_joint_a(0.0,0.0,0.0);
			glm::vec3 right_lower_leg_joint_t(0.0,-leg_length,0.0);
			glm::vec3 right_lower_leg_scale(lower_leg_width, lower_leg_length, lower_leg_width);
			Node * right_lower_leg = new Node(right_lower_leg_joint_t, right_lower_leg_joint_a, right_lower_leg_mesh_t, right_lower_leg_scale, cube);
			

		robot->push_child(right_leg, robot->get_root());//Add the leg as child of torso
		robot->push_child(right_lower_leg, right_leg);//Add the forearm as child of arm
	
	robot->prepare_traversal(); //THIS LINE IS IMPORTANT FOR TRAVERSING THE TREE DEPTH FIRST, IT POPULATES THE STACK WE WILL USE TO TRAVERSE THE TREE

	progIM = make_shared<Program>();
	progIM->setVerbose(true);
	progIM->setShaderNames(RES_DIR + "simple_vert.glsl", RES_DIR + "simple_frag.glsl");
	progIM->init();
	progIM->addUniform("P");
	progIM->addUniform("MV");
	progIM->setVerbose(false);
	
	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);
}

static void render()
{
	double t = glfwGetTime(); //time
	double s = 1 + (0.15/2.0) + (0.15/2.0) * sin(5 * 3.14159 * t);

	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = width/(float)height;
	glViewport(0, 0, width, height);

	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//init our P stack, load perspective because its constant, could make it global to not have to do the multiplications everytime
	//our ModelView stack
	auto MV = make_shared<MatrixStack>();
	auto P = make_shared<MatrixStack>();
	P->pushMatrix();
	P->multMatrix(glm::perspective((float)(45.0*M_PI/180.0), aspect, 0.01f, 100.0f));

	//bind program and go ham
	MV->pushMatrix();

	prog->bind();
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
	
	
	robot->draw(MV, prog, s, current);
	prog->unbind();

	P->popMatrix();
	MV->popMatrix();

	GLSL::checkError(GET_FILE_LINE);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RES_DIR = argv[1] + string("/");

	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	// https://en.wikipedia.org/wiki/OpenGL
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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
	// Set vsync.
	glfwSwapInterval(1);

	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);
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
