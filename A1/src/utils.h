#ifndef UTILS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UTILS_H

#include "BoundingBox.h"
#include "Triangle.h"
#include "Image.h"
#include <vector>

double RANDOM_COLORS[7][3] = {
	{0.0000,    0.4470,    0.7410},
	{0.8500,    0.3250,    0.0980},
	{0.9290,    0.6940,    0.1250},
	{0.4940,    0.1840,    0.5560},
	{0.4660,    0.6740,    0.1880},
	{0.3010,    0.7450,    0.9330},
	{0.6350,    0.0780,    0.1840},
};

std::vector<float> inplace_linear_bounding_box_search(const std::vector<float>& posBuf){
    std::vector<float> box;
    box.push_back(INFINITY);
    box.push_back(INFINITY);
    box.push_back(-1 * INFINITY);
    box.push_back(-1 * INFINITY);

    for (int i = 0; i < posBuf.size(); i += 3){
        box[0] = std::min( posBuf[i] , box[0]);
        box[1] = std::min( posBuf[i+1], box[1]);

        box[2] = std::max( posBuf[i] , box[2]);
        box[3] = std::max( posBuf[i+1], box[3]);
    }
    return box;
}

float translation(int n, float s, float t0, float t1){
    return (n/2.0) - (s * ((t1 + t0) / 2.0));
}

float scale(int height, int width, std::vector<float>& box){
	return std::min((float) width / (float) (box[2] - box[0]), (float) height / (float) (box[3] - box[1]));
}

void extract_triangles(std::vector<Triangle>& triangles, std::vector<float>& posBuf, float s, float tx, float ty){
    for (int i = 0; i < posBuf.size(); i += 9){

		std::vector<int> v1 = {(int)( (s * posBuf[i]) + tx), (int) ((s * posBuf[i+1]) + ty),(int) posBuf[i+2]};
		std::vector<int> v2 = {(int)( (s * posBuf[i+3]) + tx),(int) ((s * posBuf[i+4]) + ty), (int) posBuf[i+5]};
		std::vector<int> v3 = {(int)( (s * posBuf[i+6]) + tx),(int) ((s * posBuf[i+7]) + ty), (int) posBuf[i+8]};

		triangles.push_back( Triangle(v1,v2,v3) );
	}
}

void task1(std::vector<Triangle> triangles, float s, float tx, float ty, std::shared_ptr<Image> image){
    int r = 0;
	for (Triangle& tri : triangles){

		for(int y= tri.bb.lower.y; y < tri.bb.upper.y; ++y){
			for(int x = tri.bb.lower.x; x < tri.bb.upper.x; ++x){
				image->setPixel(x,y,RANDOM_COLORS[r][0]*255 , RANDOM_COLORS[r][1]*255 , RANDOM_COLORS[r][2]*255);
			}
		}
		r = (r + 1) % 7;
	}
}

double area_triangle2d(int x1,int y1, int x2,int y2, int x3,int y3) { return (x1 * (y2-y3) + x2 * (y3-y1) + x3 * (y1-y2)) / 2.0;}

std::vector<double> bary_coords(int x,int y, Triangle& tri){
	double area = area_triangle2d(tri.v1.position.x, tri.v1.position.y, tri.v2.position.x, tri.v2.position.y,tri.v3.position.x, tri.v3.position.y); 
    std::vector<double> areas;
    areas.push_back(area_triangle2d(x,y, tri.v1.position.x, tri.v1.position.y, tri.v2.position.x, tri.v2.position.y) / area);
    areas.push_back(area_triangle2d(x,y, tri.v2.position.x, tri.v2.position.y, tri.v3.position.x, tri.v3.position.y) / area);
    areas.push_back(area_triangle2d(x,y, tri.v3.position.x, tri.v3.position.y, tri.v1.position.x, tri.v1.position.y) / area);
	
	return areas;
}

void task2(std::vector<Triangle>& triangles, float s, float tx, float ty, std::shared_ptr<Image> image){
    int r = 0;
	for (Triangle& tri : triangles){

		for(int y= tri.bb.lower.y; y < tri.bb.upper.y; ++y){
			for(int x = tri.bb.lower.x; x < tri.bb.upper.x; ++x){
                std::vector<double> bary = bary_coords(x,y, tri);
                if (bary[0] >= 0.0 && bary[1] >= 0.0 && bary[2] >= 0.0){
                    
				    image->setPixel(x,y,RANDOM_COLORS[r][0]*255 , RANDOM_COLORS[r][1]*255 , RANDOM_COLORS[r][2]*255);
                }
			}
		}
		r = (r + 1) % 7;
	}
}

void task3(std::vector<Triangle>& triangles, float s, float tx, float ty, std::shared_ptr<Image> image){
    int k = 0;
    int r,g,b;
	for (Triangle& tri : triangles){

		for(int y= tri.bb.lower.y; y < tri.bb.upper.y; ++y){
			for(int x = tri.bb.lower.x; x < tri.bb.upper.x; ++x){
                std::vector<double> bary = bary_coords(x,y, tri);
                if (bary[0] >= 0.0 && bary[1] >= 0.0 && bary[2] >= 0.0){
                    r = 0;
                    g = 0;
                    b = 0;
                    for (int a = 0; a < 3; ++a){
                        r += bary[a] * RANDOM_COLORS[(k+a) % 7][0] * 255;
                        g += bary[a] * RANDOM_COLORS[(k+a) % 7][1] * 255;
                        b += bary[a] * RANDOM_COLORS[(k+a) % 7][2] * 255;
                    }
				    image->setPixel(x,y,r,g,b);
                }
			}
		}
        k+=1;
		
	}
}


#endif