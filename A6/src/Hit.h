#pragma  once
#ifndef HIT_H
#define HIT_H


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Ray;
class Material;
class Hit {
    public:
        Hit() : e(true), mat(){};
        Hit(glm::vec3 n, float s, std::shared_ptr<Ray> _ray, Material _mat) : normal(n), distance(s), e(false), ray(_ray), mat(_mat){}; 

        bool empty() const {
            return e;
        };
        float get_distance() const {
            return distance;
        }
        void set_distance(float d) {
            distance = d;
        }

        glm::vec3 n() {
            return normal;
        }

        Material get_mat() {
            return mat;
        }
        void set_mat(Material m){
            mat = m;
        }

        private:
            glm::vec3 normal;
            float distance;
            bool e;
            std::shared_ptr<Ray> ray;
            Material mat;        
};

#endif