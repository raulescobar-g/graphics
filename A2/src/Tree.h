#pragma  once
#ifndef Tree_H
#define Tree_H
#include <glm/glm.hpp>
#include "Node.h"

class Tree {
    private:
        Node * root;
        int loc;
        std::vector<Node*> node_order;

        void make_empty(Node * & t){
            if (t != nullptr){
                for (auto child : t->children){
                    make_empty(child);
                }
                delete t;
            }
            t = nullptr;
        };

        void draw_self(std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog, Node * t, double s, Node * cur){ //entire procedure for self_drawing taken from assignment document given to us TASK 2
            MV->translate(t->joint_t);
            MV->rotate(t->joint_a[2], glm::vec3(0.0,0.0,1.0));
            MV->rotate(t->joint_a[1], glm::vec3(0.0,1.0,0.0));
            MV->rotate(t->joint_a[0], glm::vec3(1.0,0.0,0.0)); 
            MV->pushMatrix();
                MV->translate(t->mesh_t);
                t == cur ? MV->scale(t->scale[0] * s,t->scale[1] * s,t->scale[2] * s ) : MV->scale(t->scale);
                glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]); 
                t->shape->draw(prog);
            MV->popMatrix();
        };

        void draw_helper(std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog, Node * iter, double s, Node * cur) { 

            draw_self(MV, prog, iter,s,cur);
            for (auto child : iter->children){
                MV->pushMatrix();
                    draw_helper(MV, prog, child,s,cur);
                MV->popMatrix();
            }
        };

        void gather_children(Node * t){
            node_order.push_back(t);

            for (auto child : t->children){
                gather_children(child);
            }
        }

    public:
       
       Node * get_root() {
           return this->root;
       }

       Tree(glm::vec3& jt, glm::vec3& ja, glm::vec3& mt, glm::vec3& s, std::shared_ptr<Shape> _shape): loc(0) {
           this->root = new Node(jt, ja, mt, s, _shape);
       }

        ~Tree() {
            make_empty(this->root);
        }

        void draw(std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog, double s, Node * cur) { 
            Node * iter = this->root;

            draw_self(MV, prog, iter,s,cur);
            for (auto child : iter->children){
                MV->pushMatrix();
                    draw_helper(MV, prog, child,s,cur);
                MV->popMatrix();
            }
        };

        void push_child(Node * child, Node * parent){
            parent->children.push_back(child);
        }

        Node * next() {
            loc+=1;
            return node_order[loc % node_order.size()];
        }

        Node * prev() {
            loc -=1;
            return node_order[loc % node_order.size()];
        }

        void prepare_traversal(){
            Node * iter = this->root;

            node_order.push_back(iter);
            for (auto child: iter->children){
                gather_children(child);
            }
        }
};

#endif
