#pragma  once
#ifndef Node_H
#define Node_H
#include <glm/glm.hpp>

class Node {
    public:
        std::vector< Node* > children;

        glm::vec3 joint_t;
        glm::vec3 joint_a;
        glm::vec3 mesh_t;
        glm::vec3 scale;

        std::shared_ptr<Shape> shape;
        std::shared_ptr<Shape> joint;

        bool spinning;
        float rad_count;

        Node(glm::vec3& jt, glm::vec3& ja, glm::vec3& mt, glm::vec3& s, std::shared_ptr<Shape> _shape, std::shared_ptr<Shape> _joint) : children(),
                                                                                                        joint_t(jt), 
                                                                                                        joint_a(ja), 
                                                                                                        mesh_t(mt), 
                                                                                                        scale(s), 
                                                                                                        shape(_shape),
                                                                                                        joint(_joint),
                                                                                                        spinning(false),
                                                                                                        rad_count(0.0){};
       

        void rotate(float x, float y, float z) {
            joint_a[0] += x;
            joint_a[1] += y;
            joint_a[2] += z;
        }          
        void spin_me() {
            spinning = true;
        }                                                                              

};

#endif
