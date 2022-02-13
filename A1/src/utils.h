#ifndef UTILS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UTILS_H


#include "BoundingBox.h"
#include "Triangle.h"
#include "Image.h"
#include "Zbuffer.h"
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
    std::vector<float> box = {std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity()};

    for (int i = 0; i < posBuf.size(); i += 3){
        box[0] = std::min( posBuf[i] , box[0]);
        box[1] = std::min( posBuf[i+1], box[1]);
        box[2] = std::min( posBuf[i+2], box[2]);

        box[3] = std::max( posBuf[i] , box[3]);
        box[4] = std::max( posBuf[i+1], box[4]);
        box[5] = std::max( posBuf[i+2], box[5]);
    }
    return box;
}

float translation(int n, float s, float t0, float t1){
    return (n/2.0) - (s * ((t1 + t0) / 2.0));
}

float scale(int height, int width, std::vector<float>& box){
	return std::min((float) width / (float) (box[3] - box[0]), (float) height / (float) (box[4] - box[1]));
}

void extract_triangles(std::vector<Triangle>& triangles, std::vector<float>& posBuf, std::vector<float>& normBuf,  float s, float tx, float ty){
    for (int i = 0; i < posBuf.size(); i += 9){

		std::vector<int> v1 = {(int)( (s * posBuf[i]) + tx), (int) ((s * posBuf[i+1]) + ty),(int) (s * posBuf[i+2]) };
		std::vector<int> v2 = {(int)( (s * posBuf[i+3]) + tx),(int) ((s * posBuf[i+4]) + ty), (int) (s * posBuf[i+5]) };
		std::vector<int> v3 = {(int)( (s * posBuf[i+6]) + tx),(int) ((s * posBuf[i+7]) + ty), (int) (s * posBuf[i+8]) };
        Triangle tri(v1,v2,v3);

        std::vector<float> n1 = { normBuf[i]   , normBuf[i+1] , posBuf[i+2] };
		std::vector<float> n2 = { normBuf[i+3] , normBuf[i+4] , posBuf[i+5] };
		std::vector<float> n3 = { normBuf[i+6] , normBuf[i+7] , posBuf[i+8] };

        tri.v1.normal = n1;
        tri.v2.normal = n2;
        tri.v3.normal = n3;
		triangles.push_back(tri);
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

void task4(std::vector<Triangle>& triangles, float s, float tx, float ty, std::shared_ptr<Image> image, std::vector<float>& bb){
    float dy = (bb[4] - bb[1]) * s;
	for (Triangle& tri : triangles){

		for(int y= tri.bb.lower.y; y < tri.bb.upper.y; ++y){
			for(int x = tri.bb.lower.x; x < tri.bb.upper.x; ++x){

                std::vector<double> bary = bary_coords(x,y, tri);
                if (bary[0] >= 0.0 && bary[1] >= 0.0 && bary[2] >= 0.0){
                    
				    image->setPixel(x,y,255*((y - (bb[1]*s + ty)) / (float)dy) ,0 ,255*(1- ((y - (bb[1]*s + ty))/(float)dy)));
                }
			}
		}		
	}
}

std::vector<int> crossp(Vertex& v1, Vertex& v2, Vertex& v3) {
    int x1 = v2.position.x - v1.position.x;
    int y1 = v2.position.y - v1.position.y;
    int z1 = v2.position.z - v1.position.z;

    int x2 = v3.position.x - v1.position.x;
    int y2 = v3.position.y - v1.position.y;
    int z2 = v3.position.z - v1.position.z;

    std::vector<int> res = {0, 0, 0};
    res[0] = (y1*z2 - z1*y2);
    res[1] = -1 * (x1*z2 - x2*z1);
    res[2] = (x1*y2 - y1*x2);
    return res;
}

void task5(std::vector<Triangle>& triangles, float s, float tx, float ty, std::shared_ptr<Image> image, std::vector<float>& bb, Zbuff& z_buff){
    float dz = (bb[5] - bb[2]) * s;
	for (Triangle& tri : triangles){
		for(int y= tri.bb.lower.y; y < tri.bb.upper.y; ++y){
			for(int x = tri.bb.lower.x; x < tri.bb.upper.x; ++x){

                std::vector<double> bary = bary_coords(x,y, tri);

                if (bary[0] >= 0.0 && bary[1] >= 0.0 && bary[2] >= 0.0){

                    std::vector<int> cross = crossp(tri.v1, tri.v2, tri.v3);
                    
                    float z = (float) tri.v1.position.z - (float) ( ((cross[0] * (x - tri.v1.position.x)) + (cross[1] * (y - tri.v1.position.y))) / (float) cross[2] );

                    if (z > z_buff.get_ds(x,y)) {
                        z_buff.set_ds(x,y,z);
				        image->setPixel(x,y,255 * ((z - (float)bb[2] * s) / dz) ,0 ,0);

                    }
                }
			}
		}		
	}
}

void task6(std::vector<Triangle>& triangles, float s, float tx, float ty, std::shared_ptr<Image> image, Zbuff& z_buff){
    float xn, yn, zn;
	for (Triangle& tri : triangles){
		for(int y= tri.bb.lower.y; y < tri.bb.upper.y; ++y){
			for(int x = tri.bb.lower.x; x < tri.bb.upper.x; ++x){
                std::vector<double> bary = bary_coords(x,y, tri);
                if (bary[0] >= 0.0 && bary[1] >= 0.0 && bary[2] >= 0.0){

                    std::vector<int> cross = crossp(tri.v1, tri.v2, tri.v3);
                    float z = (float) tri.v1.position.z - (float) ( ((cross[0] * (x - tri.v1.position.x)) + (cross[1] * (y - tri.v1.position.y))) / (float) cross[2] );

                    if (z > z_buff.get_ds(x,y)) {
                        z_buff.set_ds(x,y,z);

                        xn = (tri.v1.normal[0] * bary[1] + tri.v2.normal[0] * bary[2] + tri.v3.normal[0]* bary[0]);
                        yn = (tri.v1.normal[1] * bary[1] + tri.v2.normal[1] * bary[2] + tri.v3.normal[1]* bary[0]);
                        zn = (tri.v1.normal[2] * bary[1] + tri.v2.normal[2] * bary[2] + tri.v3.normal[2]* bary[0]);

                        xn = (0.5 * xn + 0.5) * 255;
                        yn = (0.5 * yn + 0.5) * 255;
                        zn = (0.5 * zn + 0.5) * 255;
                        
                        image->setPixel(x,y, xn, yn, zn);
                    }
                }
			}
		}		
	}
}

void task7(std::vector<Triangle>& triangles, float s, float tx, float ty, std::shared_ptr<Image> image, Zbuff& z_buff){
    float xn, yn, zn;
	for (Triangle& tri : triangles){
		for(int y= tri.bb.lower.y; y < tri.bb.upper.y; ++y){
			for(int x = tri.bb.lower.x; x < tri.bb.upper.x; ++x){
                std::vector<double> bary = bary_coords(x,y, tri);
                if (bary[0] >= 0.0 && bary[1] >= 0.0 && bary[2] >= 0.0){

                    std::vector<int> cross = crossp(tri.v1, tri.v2, tri.v3);
                    float z = (float) tri.v1.position.z - (float) ( ((cross[0] * (x - tri.v1.position.x)) + (cross[1] * (y - tri.v1.position.y))) / (float) cross[2] );

                    if (z > z_buff.get_ds(x,y)) {
                        z_buff.set_ds(x,y,z);
                        
                        xn = tri.v1.normal[0] * bary[1] + tri.v2.normal[0]* bary[2] + tri.v3.normal[0] * bary[0];
                        yn = tri.v1.normal[1] * bary[1] + tri.v2.normal[1]* bary[2] + tri.v3.normal[1] * bary[0];
                        zn = tri.v1.normal[2] * bary[1] + tri.v2.normal[2]* bary[2] + tri.v3.normal[2] * bary[0];  

                        int c = std::max(0, (int)((xn + yn + zn) * (255.0 / std::sqrt(3))) );       
                        image->setPixel(x,y, c, c,  c);
                    }
                }
			}
		}		
	}
}

#endif