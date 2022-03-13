#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLSL.h"
#include "Game.h"
#include "MatrixStack.h"


Game::Game(GLFWwindow *window){
    this->window = window;
    prog_n = std::make_shared<Program>();

    RESOURCE_DIR = "./../resources/";

    gamestate = std::make_shared<State>();
    camera = std::make_shared<Camera>();
    sphere = std::make_shared<Shape>();
    cube = std::make_shared<Shape>();

}

void Game::init() {

	// Set background color.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	prog_n->setShaderNames(RESOURCE_DIR + "normal_vert.glsl", RESOURCE_DIR + "normal_frag.glsl");
	prog_n->setVerbose(true);
	prog_n->init();
	prog_n->addAttribute("aPos");
	prog_n->addAttribute("aNor");
	prog_n->addUniform("MV");
	prog_n->addUniform("P");
	prog_n->setVerbose(false);

	
	camera->setInitDistance(5.0f); // Camera's initial Z translation
	
	cube->loadMesh(RESOURCE_DIR + "cube.obj");
	cube->init();

	sphere->loadMesh(RESOURCE_DIR + "sphere.obj");
	sphere->init();
	
	GLSL::checkError(GET_FILE_LINE);
}

void Game::render(){
	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	double t = glfwGetTime();

	// Matrix stacks
	auto P = std::make_shared<MatrixStack>();
	auto MV = std::make_shared<MatrixStack>();
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);
	
	MV->scale(0.2f,0.2f,0.2f);
	MV->translate(0.0f,0.0f,-5.0f);
	
	prog_n->bind();
	glUniformMatrix4fv(prog_n->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog_n->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	cube->draw(prog_n);
	prog_n->unbind();
			
		
	MV->popMatrix();
	P->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
	
}

void Game::camera_mouseClicked(float xmouse, float ymouse, bool shift, bool ctrl, bool alt){
    camera->mouseClicked(xmouse, ymouse, shift, ctrl, alt);
}

void Game::camera_mouseMoved(float xmouse, float ymouse) {
    camera->mouseMoved(xmouse, ymouse);
}