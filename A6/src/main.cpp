#include <iostream>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Image.h"
#include "SceneManager.h"
#include "Ray.h"
#include "Light.h"
#include "Camera.h"
#include <stdlib.h>
#include <chrono>
#include <sys/wait.h>
#include <thread>
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


int main(int argc, char **argv)
{	
	
	auto t1 = high_resolution_clock::now();
	shared_ptr<Image> image;
	shared_ptr<Camera> camera;

	if(argc < 2) {
		cout << "Usage: ./A6 scene resolution outputFile" << endl;
		return 0;
	}
	int scene = strtol(argv[1], NULL, 10);
	int width = strtol(argv[2], NULL, 10);
	int height = width;
	std::string filename = argv[3];

	image = make_shared<Image>(width, height);
	camera = make_shared<Camera>(width,height);				// default position and rays are initialized
	
	SceneManager mux(scene, image, camera, filename);
	mux.generate_environment();
	mux.generate_rays();

	mux.generate_scene(16); // argument is amount of threads
	

	int sum = duration_cast<milliseconds>(high_resolution_clock::now() - t1).count();
	
	cout<<"duration: "<<sum / 1000.0f<<" s\n";
	
	return 0;
}
