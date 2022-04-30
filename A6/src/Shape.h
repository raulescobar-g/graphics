#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <vector>
#include <memory>

/**
 * A shape defined by a list of triangles
 * - posBuf should be of length 3*ntris
 * - norBuf should be of length 3*ntris (if normals are available)
 * - texBuf should be of length 2*ntris (if texture coords are available)
 * posBufID, norBufID, and texBufID are OpenGL buffer identifiers.
 */
class Shape
{
public:
	Shape(std::string _id) :
	posBufID(0),
	norBufID(0),
	texBufID(0),
	//indBufID(0),
	id(_id) {};
	virtual ~Shape(){};
	void loadMesh(const std::string &meshName);
	void createMesh(int parameter);
	void fitToUnitBox();
	void draw() const;
	
	std::string get_id() {return id;};
	
private:
	std::vector<float> posBuf;
	std::vector<float> norBuf;
	std::vector<float> texBuf;
	//std::vector<unsigned int> indBuf;
	unsigned posBufID;
	unsigned norBufID;
	unsigned texBufID;
	//unsigned indBufID;
	std::string id;
};

#endif
