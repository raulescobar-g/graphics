#pragma once
#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include <memory>
#include "Shape.h"
#include "Camera.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Sphere.h"
#include "Object.h"
#include "Ray.h"
#include "Ellipsoid.h"
#include "Plane.h"


class SceneManager {
    public:
        SceneManager(int _scene, std::shared_ptr<Image> _image, std::shared_ptr<Camera> _camera, std::string output) : scene(_scene), image(_image), camera(_camera), outputFile(output) {};

        void generate_scene(){
            switch (scene){
                case 1:
                    scene1();
                    break;
                case 2:
                    scene1();
                    break;
                case 3:
                    scene3();
                    break;
                case 4:
                    scene4();
                    break;
                case 5:
                    scene4();
                    break;
                default:
                    std::cout<<"did not write this scene"<<std::endl;
                    break;
            }
        }

        void scene1(){
            int i = 0;
            int height = camera->get_height();
            int width = camera->get_width();

            for (auto ray : rays) {
                
                std::vector<Hit> hits;
                ray->hits(objects, hits);
                float r = 0.0f;
                float g = 0.0f;
                float b = 0.0f;
                int y = i / height;
                int x = i - y * width;
                if (!hits.empty()){
                    r = hits[0].object->get_material()->ka.r;
                    g = hits[0].object->get_material()->ka.g;
                    b = hits[0].object->get_material()->ka.b;

                    for (auto light: lights){
                        glm::vec3 hitPos = ray->get_origin() + ray->get_ray()*hits[0].ds;
                        glm::vec3 l = glm::normalize(light->get_position() - hitPos);

                        Ray shadowRay(l.x, l.y, l.z, hitPos, glm::distance(light->get_position() , hitPos));

                        std::vector<Hit> lightHits;
                        shadowRay.light_hits(objects, lightHits);
                        if (lightHits.empty()) {
                        
                            glm::vec3 n = glm::normalize(hits[0].normal);
                            if (glm::dot(n,l) > 0.0f){
                                glm::vec3 h = glm::normalize(l - glm::normalize(ray->get_ray()));
                                                            
                                r += light->get_intensity() * ((hits[0].object->get_material()->kd.r * glm::dot(l, n)) + (hits[0].object->get_material()->ks.r * glm::pow(std::max(0.0f, glm::dot(h,n)), hits[0].object->get_material()->s) ));
                                g += light->get_intensity() * ((hits[0].object->get_material()->kd.g * glm::dot(l, n)) + (hits[0].object->get_material()->ks.g * glm::pow(std::max(0.0f, glm::dot(h,n)), hits[0].object->get_material()->s) ));
                                b += light->get_intensity() * ((hits[0].object->get_material()->kd.b * glm::dot(l, n)) + (hits[0].object->get_material()->ks.b * glm::pow(std::max(0.0f, glm::dot(h,n)), hits[0].object->get_material()->s) ));
                            } 
                        }
                    }
                    
                    
                }
                r = glm::clamp(r, 0.0f,1.0f);
                g = glm::clamp(g, 0.0f,1.0f);
                b = glm::clamp(b, 0.0f,1.0f);
                image->setPixel(x, y, 255.0f*r,255.0f*g,255.0f*b);
                
                ++i;
            }
            image->writeToFile(outputFile);

        }
        
        void scene3(){

            int i = 0;
            int height = camera->get_height();
            int width = camera->get_width();

            for (auto ray : rays) {
                
                std::vector<Hit> hits;
                ray->hits(objects, hits);
                float r = 0.0f;
                float g = 0.0f;
                float b = 0.0f;
                int y = i / height;
                int x = i - y * width;
                if (!hits.empty()){
                    r = hits[0].object->get_material()->ka.r;
                    g = hits[0].object->get_material()->ka.g;
                    b = hits[0].object->get_material()->ka.b;

                    for (auto light: lights){
                        glm::vec3 hitPos = ray->get_origin() + ray->get_ray()*hits[0].ds;
                        glm::vec3 l = glm::normalize(light->get_position() - hitPos);

                        Ray shadowRay(l.x, l.y, l.z, hitPos, glm::distance(light->get_position() , hitPos));

                        std::vector<Hit> lightHits;
                        shadowRay.light_hits(objects, lightHits);
                        if (lightHits.empty()) {
                        
                            glm::vec3 n = glm::normalize(hits[0].normal);
                            if (glm::dot(n,l) >= 0.0f){
                                glm::vec3 h = glm::normalize(l - glm::normalize(ray->get_ray()));
                                                            
                                r += light->get_intensity() * ((hits[0].object->get_material()->kd.r * glm::dot(l, n)) + (hits[0].object->get_material()->ks.r * glm::pow(std::max(0.0f, glm::dot(h,n)), hits[0].object->get_material()->s) ));
                                g += light->get_intensity() * ((hits[0].object->get_material()->kd.g * glm::dot(l, n)) + (hits[0].object->get_material()->ks.g * glm::pow(std::max(0.0f, glm::dot(h,n)), hits[0].object->get_material()->s) ));
                                b += light->get_intensity() * ((hits[0].object->get_material()->kd.b * glm::dot(l, n)) + (hits[0].object->get_material()->ks.b * glm::pow(std::max(0.0f, glm::dot(h,n)), hits[0].object->get_material()->s) ));
                            } 
                        }
                    }
                    
                    
                }
                r = glm::clamp(r, 0.0f,1.0f);
                g = glm::clamp(g, 0.0f,1.0f);
                b = glm::clamp(b, 0.0f,1.0f);
                image->setPixel(x, y, 255.0f*r,255.0f*g,255.0f*b);
                
                ++i;
            }
            image->writeToFile(outputFile);
        }
        
        void scene4(){

            int i = 0;
            int height = camera->get_height();
            int width = camera->get_width();

            // for (auto ray : camera->get_rays()) {
                
            //     std::vector<Hit> hits;
            //     ray->hits(objects, hits);
            //     float r = 0.0f;
            //     float g = 0.0f;
            //     float b = 0.0f;
            //     int y = i / height;
            //     int x = i - y * width;
            //     if (!hits.empty()){
            //         if (hits[0].object->get_type() == "reflective"){
            //             std::stack< std::shared_ptr<Hit> > recursive;
            //             recursive.push(std::make_shared<Hit>(hits[0].normal, hits[0].ds, hits[0].object));
            //             std::shared_ptr<Ray> curr = ray;

            //             while (recursive.size() < 100 && recursive.top()->object->get_type() == "reflective") {
            //                 glm::vec3 ref = glm::reflect(curr->get_ray(), recursive.top()->normal);
            //                 std::shared_ptr<Ray> reflection = std::make_shared<Ray>(ref.x, ref.y, ref.z , recursive.top()->ds * curr->get_ray() + curr->get_origin());
            //                 std::vector<Hit> res;
            //                 reflection->hits(objects, res);
            //                 std::shared_ptr<Hit> new_hit = std::make_shared<Hit>(res[0].normal, res[0].ds, res[0].object);
            //                 curr = reflection;
            //                 recursive.push(new_hit);
            //             }
            //             if (recursive.top()->object->get_type() != "reflective") {
            //                 image->setPixel(x,y,0.0f,0.0f,0.0f);
            //                 continue;
            //             } else {
            //                 image->setPixel(x,y,0.0f,0.0f,0.0f);
            //                 continue;
            //             }

            //         }
            //         r = hits[0].object->get_material()->ka.r;
            //         g = hits[0].object->get_material()->ka.g;
            //         b = hits[0].object->get_material()->ka.b;

            //         for (auto light: lights){
            //             glm::vec3 hitPos = ray->get_origin() + ray->get_ray()*hits[0].ds;
            //             glm::vec3 l = glm::normalize(light->get_position() - hitPos);

            //             Ray shadowRay(l.x, l.y, l.z, hitPos, glm::distance(light->get_position() , hitPos));

            //             std::vector<Hit> lightHits;
            //             shadowRay.light_hits(objects, lightHits);
            //             if (lightHits.empty()) {
                        
            //                 glm::vec3 n = glm::normalize(hits[0].normal);
            //                 if (glm::dot(n,l) >= 0.0f){
            //                     glm::vec3 h = glm::normalize(l - glm::normalize(ray->get_ray()));
                                                            
            //                     r += light->get_intensity() * ((hits[0].object->get_material()->kd.r * glm::dot(l, n)) + (hits[0].object->get_material()->ks.r * glm::pow(std::max(0.0f, glm::dot(h,n)), hits[0].object->get_material()->s) ));
            //                     g += light->get_intensity() * ((hits[0].object->get_material()->kd.g * glm::dot(l, n)) + (hits[0].object->get_material()->ks.g * glm::pow(std::max(0.0f, glm::dot(h,n)), hits[0].object->get_material()->s) ));
            //                     b += light->get_intensity() * ((hits[0].object->get_material()->kd.b * glm::dot(l, n)) + (hits[0].object->get_material()->ks.b * glm::pow(std::max(0.0f, glm::dot(h,n)), hits[0].object->get_material()->s) ));
            //                 } 
            //             }
            //         }
                    
                    
            //     }
            //     r = glm::clamp(r, 0.0f,1.0f);
            //     g = glm::clamp(g, 0.0f,1.0f);
            //     b = glm::clamp(b, 0.0f,1.0f);
            //     image->setPixel(x, y, 255.0f*r,255.0f*g,255.0f*b);
                
            //     ++i;
            // }
            image->writeToFile(outputFile);
        }


        void generate_rays(){
            int height = camera->get_height();
            int width = camera->get_width();
            int fov = camera->get_fov();
            glm::vec3 pos = camera->get_pos();

            float k = 2.0f*tan(fov/2.0f) / (float)height;

            for (int _y = 0; _y < height; ++_y){
                float shift_y = k* ((float)_y - ((float)height/2.0f) + 0.5f);

                for (int _x = 0; _x < width; ++_x){
                    float shift_x = k*((float)_x - ((float)width/2.0f) + 0.5f);
                    rays.push_back(std::make_shared<Ray>(shift_x, shift_y, -1.0f, pos));
                }
            }
        };

        void generate_environment() {
            switch (scene) {
                case 1:
                    objects.push_back(
                            std::make_shared<Sphere>(
                                                        glm::vec3(-0.5f, -1.0f, 1.0f), 
                                                        glm::vec3(1.0f,1.0f,1.0f), 
                                                        std::make_shared<Material>(
                                                                                    glm::vec3(1.0f,0.0f,0.0f), 
                                                                                    glm::vec3(1.0f,1.0f,0.5f), 
                                                                                    glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                )
                                                    )
                            );

                    objects.push_back(
                                    std::make_shared<Sphere>(
                                                                glm::vec3(0.5f, -1.0f, -1.0f), 
                                                                glm::vec3(1.0f,1.0f,1.0f), 
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,1.0f,0.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                        )
                                                            )
                                    );

                    objects.push_back(
                                    std::make_shared<Sphere>(
                                                                glm::vec3(0.0f, 1.0f, 0.0f), 
                                                                glm::vec3(1.0f,1.0f,1.0f), 
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,1.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                        )
                                                            )
                                    );

                    auto light = std::make_shared<Light>(glm::vec3(-2.0f, 1.0f, 1.0f),1.0f);
                    std::vector< std::shared_ptr<Light> > lights;
                    lights.push_back(light);
                    break;
                case 2:
                    objects.push_back(
                            std::make_shared<Sphere>(
                                                        glm::vec3(-0.5f, -1.0f, 1.0f), 
                                                        glm::vec3(1.0f,1.0f,1.0f), 
                                                        std::make_shared<Material>(
                                                                                    glm::vec3(1.0f,0.0f,0.0f), 
                                                                                    glm::vec3(1.0f,1.0f,0.5f), 
                                                                                    glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                )
                                                    )
                            );

                    objects.push_back(
                                    std::make_shared<Sphere>(
                                                                glm::vec3(0.5f, -1.0f, -1.0f), 
                                                                glm::vec3(1.0f,1.0f,1.0f), 
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,1.0f,0.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                        )
                                                            )
                                    );

                    objects.push_back(
                                    std::make_shared<Sphere>(
                                                                glm::vec3(0.0f, 1.0f, 0.0f), 
                                                                glm::vec3(1.0f,1.0f,1.0f), 
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,1.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                        )
                                                            )
                                    );

                    lights.push_back(std::make_shared<Light>(glm::vec3(-2.0f, 1.0f, 1.0f),1.0f););
                    break;
                case 3:
                    objects.push_back(
                            std::make_shared<Ellipsoid>(
                                                        glm::vec3(0.5f, 0.0f, 0.5f), 
                                                        glm::vec3(0.5f, 0.6f, 0.2f), 
                                                        std::make_shared<Material>(
                                                                                    glm::vec3(1.0f,0.0f,0.0f), 
                                                                                    glm::vec3(1.0f,1.0f,0.5f), 
                                                                                    glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                )
                                                    )
                            );

                    objects.push_back(
                                    std::make_shared<Sphere>(
                                                                glm::vec3(-0.5f, 0.0f, -0.5f), 
                                                                glm::vec3(1.0f,1.0f,1.0f), 
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,1.0f,0.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                        )
                                                            )
                                    );

                    objects.push_back(
                                    std::make_shared<Plane>(
                                                                glm::vec3(0.0f, -1.0f, 0.0f), 
                                                                glm::vec3(0.0f,1.0f,0.0f), 
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(1.0f,1.0f,1.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 0.0f
                                                                                        )
                                                            )
                                    );

                    lights.push_back(std::make_shared<Light>(glm::vec3(1.0f, 2.0f, 2.0f),0.5f));
                    lights.push_back(std::make_shared<Light>(glm::vec3(-1.0f, 2.0f, -1.0f), 0.5f));
                    break;
                case 4:
                    objects.push_back( // 1
                            std::make_shared<Sphere>(
                                                        glm::vec3(0.5f, -0.7f, 0.5f), 
                                                        glm::vec3(0.3f,0.3f,0.3f), 
                                                        std::make_shared<Material>(
                                                                                    glm::vec3(1.0f,0.0f,0.0f), 
                                                                                    glm::vec3(1.0f,1.0f,0.5f), 
                                                                                    glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                )
                                                    )
                            );
                    objects.push_back( // 2
                                    std::make_shared<Sphere>(
                                                                glm::vec3(1.0f, -0.7f, 0.0f), 
                                                                glm::vec3(0.3f,0.3f,0.3f), 
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,1.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                        )
                                                            )
                                    );
                    objects.push_back( // reflective 1
                                    std::make_shared<Sphere>(
                                                                glm::vec3(-0.5f, 0.0f, -0.5f), 
                                                                glm::vec3(1.0f,1.0f,1.0f), 
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 0.0f
                                                                                        ), 
                                                                "reflective"
                                                            )
                                    );
                    objects.push_back( // reflective 2
                                    std::make_shared<Sphere>(
                                                                glm::vec3(1.5f, 0.0f, -1.5f), 
                                                                glm::vec3(1.0f,1.0f,1.0f), 
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 0.0f
                                                                                        ),
                                                                "reflective"
                                                            )
                                    );
                    objects.push_back( // floor
                            std::make_shared<Plane>(
                                                        glm::vec3(0.0f, -1.0f, 0.0f), 
                                                        glm::vec3(0.0f,1.0f,0.0f), 
                                                        std::make_shared<Material>(
                                                                                    glm::vec3(1.0f,1.0f,1.0f), 
                                                                                    glm::vec3(0.0f,0.0f,0.0f), 
                                                                                    glm::vec3(0.1f,0.1f,0.1f), 0.0f
                                                                                )
                                                    )
                            );
                    objects.push_back( // wall
                                    std::make_shared<Plane>(
                                                                glm::vec3(0.0f, 0.0f, -3.0f), 
                                                                glm::vec3(0.0f,0.0f,1.0f), 
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(1.0f,1.0f,1.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 0.0f
                                                                                        )
                                                            )
                                    );

                    lights.push_back(std::make_shared<Light>(glm::vec3(-1.0f, 2.0f, 1.0f),0.5f));
                    lights.push_back(std::make_shared<Light>(glm::vec3(0.5f, -0.5f, 0.0f), 0.5f));
                    break;
            }
        }

    private:
        int scene;
        std::shared_ptr<Image> image;
        std::vector< std::shared_ptr<Shape> > objects;
        std::vector< std::shared_ptr<Light> > lights;
        std::shared_ptr<Camera> camera;
        std::vector< std::shared_ptr<Ray> > rays;
        std::string outputFile;
};

#endif