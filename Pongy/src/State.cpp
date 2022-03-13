#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "State.h"



State::State(){
    //Initialize time
    
    glfwSetTime(0.0);
}