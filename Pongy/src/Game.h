#pragma  once
#ifndef GAME_H
#define GAME_H

#include "State.h"
#include "Program.h"
#include "Shape.h"
#include "Camera.h"
#include <memory>

class Game {
    public:
        Game(GLFWwindow *window);
        void init();
        void render();
        void camera_mouseClicked(float xmouse, float ymouse, bool shift, bool ctrl, bool alt);
        void camera_mouseMoved(float xmouse, float ymouse);
    private:
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Shape> cube;
        std::shared_ptr<Shape> sphere;
        std::shared_ptr<State> gamestate;
        std::shared_ptr<Program> prog_n;
        std::string RESOURCE_DIR;
        GLFWwindow *window;
};

#endif