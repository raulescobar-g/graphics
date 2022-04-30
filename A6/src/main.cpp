#include <iostream>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Image.h"
#include "SceneManager.h"
#include "Ray.h"
#include "Shape.h"
#include "Light.h"
#include "Camera.h"
#include <stdlib.h>
#include <chrono>
// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


int main(int argc, char **argv)
{	
	vector<float> times;
	int total_runs = 1;
	for (int i = 0; i < total_runs; ++i){
		auto t1 = high_resolution_clock::now();
		// TODO INPUT ARGS
		shared_ptr<Image> image;
		shared_ptr<Camera> camera;

		if(argc < 2) {
			cout << "Usage: A6 meshfile" << endl;
			return 0;
		}
		int scene = strtol(argv[1], NULL, 10);
		int width = strtol(argv[2], NULL, 10);
		int height = width;

		image = make_shared<Image>(width, height);
		camera = make_shared<Camera>(width,height);				// default position and rays are initialized
		
		
		SceneManager mux(scene, image, camera, "../output.png");
		mux.generate_environment();
		mux.generate_rays();
		mux.generate_scene();

		times.push_back( duration_cast<milliseconds>(high_resolution_clock::now() - t1).count() );
		i % 5 == 0 ? cout<<"["<<(float)i/(float)total_runs<<"%]"<<endl : cout<<"";;
	}

	int sum = 0;
	for (int runs: times) {
		sum += runs;
	}
	cout<<"Averaged: "<<(sum/(float)total_runs) / 1000.0f<<" s\n";
	
	return 0;
}
