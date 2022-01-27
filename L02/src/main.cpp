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

double area_triangle2d(int x1,int y1, int x2,int y2, int x3,int y3) { return (x1 * (y2-y3) + x2 * (y3-y1) + x3 * (y1-y2)) / 2.0;}

Color inside_triangle(int x,int y, Triangle tri){
	double area = area_triangle2d(tri.v1.position.x, tri.v1.position.y, tri.v2.position.x, tri.v2.position.y,tri.v3.position.x, tri.v3.position.y); 

	double a = area_triangle2d(x,y, tri.v1.position.x, tri.v1.position.y, tri.v2.position.x, tri.v2.position.y) / area;
	if (a < 0.0){
		return Color();
	}
	double b = area_triangle2d(x,y, tri.v2.position.x, tri.v2.position.y, tri.v3.position.x, tri.v3.position.y) / area;
	if (b < 0.0){
		return Color();
	}

	double c = area_triangle2d(x,y, tri.v3.position.x, tri.v3.position.y, tri.v1.position.x, tri.v1.position.y) / area;
	if (c < 0.0){
		return Color();
	}
	return Color((int) (a * 255), (int) (b * 255), (int) (c * 255));
}

int main(int argc, char **argv)
{
	if(argc < 4) {
		cout << "Usage: L01 filename width height vertices..." << endl;
		return 0;
	}
	// Output filename
	string filename(argv[1]);
	// Width of image
	int width = atoi(argv[2]);
	// Height of image
	int height = atoi(argv[3]);

	vector<int> v1 = {atoi(argv[4]),atoi(argv[5])};
	vector<int> v2 = {atoi(argv[9]),atoi(argv[10])};
	vector<int> v3 = {atoi(argv[14]),atoi(argv[15])};
	vector<int> c1 = {atoi(argv[6]), atoi(argv[7]), atoi(argv[8])};
	vector<int> c2 = {atoi(argv[11]), atoi(argv[12]), atoi(argv[13])};
	vector<int> c3 = {atoi(argv[16]), atoi(argv[17]), atoi(argv[18])};

	Triangle tri(v1,v2,v3,c1,c2,c3);

	
	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);
	// Draw a rectangle
	// image->setPixel(x, y, r, g, b);

	for(int y= tri.bb.lower.y; y <= tri.bb.upper.y; ++y){
		for(int x = tri.bb.lower.x; x <= tri.bb.upper.x; ++x){
			Color blend = inside_triangle(x,y,tri);
			image->setPixel(x,y,blend.r,blend.g,blend.b);
		}
	}
	
	// Write image to file
	image->writeToFile(filename);
	return 0;
}
