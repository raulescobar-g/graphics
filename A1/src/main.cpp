#include <iostream>
#include <string>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Image.h"
#include "Triangle.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

double RANDOM_COLORS[7][3] = {
	{0.0000,    0.4470,    0.7410},
	{0.8500,    0.3250,    0.0980},
	{0.9290,    0.6940,    0.1250},
	{0.4940,    0.1840,    0.5560},
	{0.4660,    0.6740,    0.1880},
	{0.3010,    0.7450,    0.9330},
	{0.6350,    0.0780,    0.1840},
};


int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Usage: A1 meshfile" << endl;
		return 0;
	}
	string meshName(argv[1]);
	string filename(argv[2]);
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	int task = atoi(argv[5]);

	// Load geometry
	vector<float> posBuf; // list of vertex positions
	vector<float> norBuf; // list of vertex normals
	vector<float> texBuf; // list of vertex texture coords
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());

	if(!rc) {
		cerr << errStr << endl;
	} else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for(size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
					if(!attrib.normals.empty()) {
						norBuf.push_back(attrib.normals[3*idx.normal_index+0]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+1]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+2]);
					}
					if(!attrib.texcoords.empty()) {
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+0]);
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	cout << "Number of vertices: " << posBuf.size()/3 << endl;
	auto image = make_shared<Image>(width, height);

	BoundingBox net_size;
	for (int i = 0; i < posBuf.size(); i += 3){
		net_size.lower.x = min( posBuf[i] , net_size.lower.x);
		net_size.lower.y = min( posBuf[i+1], net_size.lower.y);

		net_size.upper.x = max( posBuf[i] , net_size.upper.x);
		net_size.upper.y = max( posBuf[i+1], net_size.upper.y);
	}

	float dx = net_size.upper.x - net_size.lower.x;
	float dy = net_size.upper.y - net_size.lower.y;
	float s = min((float)width/(float) dx, (float)height/(float) dy);
	float tx = (width/2.0) - (s * ((net_size.upper.x + net_size.lower.x) / 2.0));
	float ty = (height/2.0) - (s * ((net_size.upper.y + net_size.lower.y) / 2.0));


	vector<Triangle> triangles;
	for (int i = 0; i < posBuf.size(); i += 9){

		vector<float> v1 = {posBuf[i], posBuf[i+1], posBuf[i+2]};
		vector<float> v2 = { posBuf[i+3],  posBuf[i+4],  posBuf[i+5]};
		vector<float> v3 = { posBuf[i+6],  posBuf[i+7],  posBuf[i+8]};

		triangles.push_back( Triangle(v1,v2,v3) );
	}


	int r = 0;
	for (Triangle& tri : triangles){

		tri.bb.lower.y = (s * tri.bb.lower.y) + ty;
		tri.bb.lower.x = (s * tri.bb.lower.x) + tx;
		tri.bb.upper.x = (s * tri.bb.upper.x) + tx;
		tri.bb.upper.y = (s * tri.bb.upper.y) + ty;

		for(int y= tri.bb.lower.y; y < tri.bb.upper.y; ++y){
			for(int x = tri.bb.lower.x; x < tri.bb.upper.x; ++x){
				image->setPixel(x,y,RANDOM_COLORS[r][0]*255 , RANDOM_COLORS[r][1]*255 , RANDOM_COLORS[r][2]*255);
			}
		}
		r = (r + 1) % 7;
	}
	
	image->writeToFile(filename);
	return 0;
}
