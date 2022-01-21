#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Image.h"
#include "Triangle.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

int main(int argc, char **argv)
{
	if(argc < 4 && (argc - 4) % 9 == 0) {
		cout << "Usage: L01 filename width height vertices..." << endl;
		return 0;
	}
	// Output filename
	string filename(argv[1]);
	// Width of image
	int width = atoi(argv[2]);
	// Height of image
	int height = atoi(argv[3]);

	vector<int> v1{atoi(argv[4]),atoi(argv[5])};
	vector<int> v2{atoi(argv[6]),atoi(argv[7])};
	vector<int> v3{atoi(argv[8]),atoi(argv[9])};

	Triangle tri(v1,v2,v3);

	
	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);
	// Draw a rectangle
	for(int y = tri.bb->lower[1]; y < tri.bb->upper[1]; ++y) {
		for(int x = tri.bb->lower[0]; x < tri.bb->upper[0]; ++x) {
			unsigned char r = 255;
			unsigned char g = 0;
			unsigned char b = 0;
			image->setPixel(x, y, r, g, b);
		}
	}
	for (int x = -5; x < 5; ++x){
		for (int y = -5; y < 6; ++y){
			image->setPixel(tri.v1[0]+x,tri.v1[1]+y, 255,255,255);
			image->setPixel(tri.v2[0]+x,tri.v2[1]+y, 255,255,255);
			image->setPixel(tri.v3[0]+x,tri.v3[1]+y, 255,255,255);
		}
	}
	
	// Write image to file
	image->writeToFile(filename);
	return 0;
}
