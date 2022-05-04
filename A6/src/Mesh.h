#pragma once

#ifndef MESH_H
#define MESH_H


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Object.h"
#include "Hit.h"

#define EPSILON 0.000001

class Mesh : public Object {
    public:
        Mesh(glm::vec3 pos, glm::vec3 scale,float theta, glm::vec3 axis, std::shared_ptr<Material> material) : Object(pos,scale,theta,axis,material) {
            std::string mesh_file = "bunny.obj";
            std::string RESOURCE_DIR = "../resources/";
            loadMesh(RESOURCE_DIR + mesh_file);

            glm::vec3 vmin(posBuf[0], posBuf[1], posBuf[2]);
            glm::vec3 vmax(posBuf[0], posBuf[1], posBuf[2]);
            for(int i = 0; i < (int)posBuf.size(); i += 3) {
                glm::vec3 v(posBuf[i], posBuf[i+1], posBuf[i+2]);
                vmin.x = std::min(vmin.x, v.x);
                vmin.y = std::min(vmin.y, v.y);
                vmin.z = std::min(vmin.z, v.z);
                vmax.x = std::max(vmax.x, v.x);
                vmax.y = std::max(vmax.y, v.y);
                vmax.z = std::max(vmax.z, v.z);
            }
            
            bound_pos = (0.5f*(vmin + vmax));
            bound_scale = (glm::length(vmax - vmin)/2.0f);

            M = glm::translate(glm::mat4(1.0f), pos) * glm::rotate(glm::mat4(1.0f), theta,axis) * glm::scale(glm::mat4(1.0f), scale);
            Mt = glm::transpose(M);
            Mi = glm::inverse(M);
            Mit = glm::transpose(Mi);
            Mti = glm::inverse(glm::transpose(M));

        }

        Hit intersect(std::shared_ptr<Ray> ray, float start, float stop){ 
            Hit hit = Hit();
            
            glm::vec3 xyz = glm::normalize(Mi * glm::vec4(ray->xyz(),0.0f));
            glm::vec3 o = glm::vec3(Mi * glm::vec4((ray->o()+(ray->xyz()*start)),1.0f));

            std::shared_ptr<Ray> ray_p = std::make_shared<Ray>(xyz, o); // 

            float a = glm::dot( xyz , xyz ); 
            float b = 2.0f * (glm::dot((o - bound_pos), xyz ));
            float c = glm::dot(o - bound_pos,o - bound_pos) - bound_scale * bound_scale;
                        
            float d = b*b - 4 * a * c;
            if (d <= 0.0f) return hit;

            // float t1 = (-b + sqrt(d)) / (2 * a);
            // float t2 = (-b - sqrt(d)) / (2 * a);
            
            // if ((t1 < 0.0f && t2 < 0.0f) || (t1 > stop && t2 > stop)) return hit;
            
            float t = 0.0f;
            float u = 0.0f;
            float v = 0.0f;
            float w = 0.0f;
            glm::vec3 n,x;
            
            for(int i = 0; i < (int)posBuf.size(); i += 9) {
                
                if ( triangle_hit(ray_p, glm::vec3(posBuf[i+0], posBuf[i+1], posBuf[i+2]), glm::vec3(posBuf[i+3], posBuf[i+4], posBuf[i+5]), glm::vec3(posBuf[i+6], posBuf[i+7], posBuf[i+8]), t, u, v, start,stop) ){
                    w = 1.0f - (u + v);
                    n.x = w * norBuf[i] + u * norBuf[i+3] + v * norBuf[i+6];
                    n.y = w * norBuf[i+1] + u * norBuf[i+4] + v * norBuf[i+7];
                    n.z = w * norBuf[i+2] + u * norBuf[i+5] + v * norBuf[i+8];

                    x = glm::vec3(M * glm::vec4(o+(xyz*t), 1.0f));
                    t = glm::dot(ray->xyz(), x - ray->o()) < 0.0f ? -glm::length(x - ray->o()) : glm::length(x - ray->o());
                    n = glm::normalize(Mit * glm::vec4(glm::normalize(n), 0.0f));

                    if (t > 0.0f) hit = hit.empty() || hit.get_distance() > t ? Hit(n , t, ray, *material.get()) : hit ; // 
                }

            }
            return hit;
        }

        void loadMesh(const std::string &meshName){
            // Load geometry
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string errStr;
            bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
            if(!rc) {
                std::cerr << errStr << std::endl;
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
        };

        bool triangle_hit(std::shared_ptr<Ray> ray, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, float& t, float& u, float& v, float start, float stop) {
            glm::vec3 orig = ray->o() + (start * ray->xyz());
            glm::vec3 dir = ray->xyz();

            glm::vec3 edge1, edge2, tvec, pvec, qvec;
            double det,inv_det;

            /* find vectors for two edges sharing vert0 */
            edge1 = vert1 - vert0;
            edge2 = vert2 - vert0;

            /* begin calculating determinant - also used to calculate U parameter */
            pvec = glm::cross(dir, edge2);

            /* if determinant is near zero, ray lies in plane of triangle */
            det = glm::dot(edge1, pvec);

            if (det > -EPSILON && det < EPSILON) return false;
            inv_det = 1.0f / det;

            /* calculate distance from vert0 to ray origin */
            tvec = orig - vert0;

            /* calculate U parameter and test bounds */
            u = glm::dot(tvec, pvec) * inv_det;
            if (u < 0.0f || u > 1.0f) return false;

            /* prepare to test V parameter */
            qvec = glm::cross(tvec, edge1);

            /* calculate V parameter and test bounds */
            v = glm::dot(dir, qvec) * inv_det;
            if (v < 0.0 || u + v > 1.0f) return false;

            /* calculate t, ray intersects triangle */
            t = glm::dot(edge2, qvec) * inv_det;

            return t < stop;
        }

    private:
        std::vector<float> posBuf;
	    std::vector<float> norBuf;
        std::vector<float> texBuf;

        glm::mat4 M;
        glm::mat4 Mi;
        glm::mat4 Mit;
        glm::mat4 Mti;
        glm::mat4 Mt;

        glm::vec3 bound_pos;
        float bound_scale;
};


#endif