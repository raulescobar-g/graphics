#pragma once
#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include <memory>
#include "Camera.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Sphere.h"
#include "Object.h"
#include "Ray.h"
#include "Ellipsoid.h"
#include "Plane.h"
#include "Mesh.h"
#include "SafeQueue.h"

#include <sys/wait.h>
#include <thread>
#include <mutex>
#include <queue>

#define EPS 1e-3
#define FAR 100000000.0f
class SceneManager {
    public:
        SceneManager(int _scene, std::shared_ptr<Image> _image, std::shared_ptr<Camera> _camera, std::string output) : scene(_scene), image(_image), camera(_camera), outputFile(output) {};

        void ray_job( SafeQueue<std::pair<std::shared_ptr<Ray>, glm::vec2> >& q ) {
            std::pair<std::shared_ptr<Ray>, glm::vec2> ray;
            while (q.pop(ray)) {
                glm::vec3 color = glm::clamp(compute_ray_color(ray.first, EPS, FAR), 0.0f,1.0f);
                image->setPixel(ray.second.x, ray.second.y, 255.0f*color.r,255.0f*color.g,255.0f*color.b);
            }
        }

        void generate_scene(int consumers){
            std::vector<std::thread> jobs;

            for (int i =0; i < consumers; ++i) {
                std::thread t = std::thread(&SceneManager::ray_job, this, ref(rays) );
                jobs.push_back(move(t));
            }
            for (int i = 0; i < consumers; ++i){
                jobs.at(i).join();
            }

            image->writeToFile(outputFile);
        }
        

        void generate_rays(){
            int height = camera->get_height();
            int width = camera->get_width();
            float fov = camera->get_fov();
            glm::vec3 pos = camera->get_pos();

            float pitch = camera->get_pitch();
            float yaw = camera->get_yaw();

            float k = 2.0f*tan(fov/2.0f) / (float)height;
            glm::vec4 p;
            glm::mat4 M = glm::lookAt(pos, pos + glm::normalize(glm::vec3(sin(yaw)*cos(pitch), sin(pitch), cos(yaw)*cos(pitch))), glm::vec3(0.0f,1.0f,0.0f) );
            
            for (int _y = 0; _y < height; ++_y){
                float shift_y = k* ((float)_y - ((float)height/2.0f) + 0.5f);

                for (int _x = 0; _x < width; ++_x){
                    float shift_x = k*((float)_x - ((float)width/2.0f) + 0.5f);
                    p = glm::vec4(shift_x, shift_y, -1.0f, 0.0f);
                    rays.push( std::pair< std::shared_ptr<Ray>, glm::vec2 >(std::make_shared<Ray>(glm::vec3(M*p), pos), glm::vec2(_x, _y)));
                }
            }
        };

        void generate_environment() {
            //srand (time(NULL));
            switch (scene) {
                case 1:
                    objects.push_back(
                            std::make_shared<Sphere>(
                                                        glm::vec3(-0.5f, -1.0f, 1.0f), 
                                                        glm::vec3(1.0f,1.0f,1.0f), 
                                                        0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,1.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                        )
                                                            )
                                    );

                    lights.push_back(std::make_shared<Light>(glm::vec3(-2.0f, 1.0f, 1.0f),1.0f));
                    break;
                case 2:
                    objects.push_back(
                            std::make_shared<Sphere>(
                                                        glm::vec3(-0.5f, -1.0f, 1.0f), 
                                                        glm::vec3(1.0f,1.0f,1.0f), 
                                                        0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,1.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                        )
                                                            )
                                    );

                    lights.push_back(std::make_shared<Light>(glm::vec3(-2.0f, 1.0f, 1.0f),1.0f));
                    break;
                 case 3:
                    objects.push_back(
                            std::make_shared<Ellipsoid>(
                                                        glm::vec3(0.5f, 0.0f, 0.5f), 
                                                        glm::vec3(0.5f, 0.6f, 0.2f), 
                                                        0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                        0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 0.0f, "reflective"
                                                                                        )
                                                            )
                                    );
                    objects.push_back( // reflective 2
                                    std::make_shared<Sphere>(
                                                                glm::vec3(1.5f, 0.0f, -1.5f), 
                                                                glm::vec3(1.0f,1.0f,1.0f), 
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 0.0f, "reflective"
                                                                                        )
                                                            )
                                    );
                    objects.push_back( // floor
                            std::make_shared<Plane>(
                                                        glm::vec3(0.0f, -1.0f, 0.0f), 
                                                        glm::vec3(0.0f,1.0f,0.0f), 
                                                        0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                case 5:
                    objects.push_back( // 1
                            std::make_shared<Sphere>(
                                                        glm::vec3(0.5f, -0.7f, 0.5f), 
                                                        glm::vec3(0.3f,0.3f,0.3f), 
                                                        0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 0.0f, "reflective"
                                                                                        )
                                                            )
                                    );
                    objects.push_back( // reflective 2
                                    std::make_shared<Sphere>(
                                                                glm::vec3(1.5f, 0.0f, -1.5f), 
                                                                glm::vec3(1.0f,1.0f,1.0f), 
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 
                                                                                            glm::vec3(0.0f,0.0f,0.0f), 0.0f, "reflective"
                                                                                        )
                                                            )
                                    );
                    objects.push_back( // floor
                                    std::make_shared<Plane>(
                                                            glm::vec3(0.0f, -1.0f, 0.0f), 
                                                            glm::vec3(0.0f,1.0f,0.0f), 
                                                            0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                            0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                case 6:
                    objects.push_back( // 1
                            std::make_shared<Mesh>(
                                                    glm::vec3(0.0f, 0.0f, 0.0f), 
                                                    glm::vec3(1.0f,1.0f,1.0f), 
                                                    0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                    std::make_shared<Material>(
                                                                                glm::vec3(0.0f,0.0f,1.0f), 
                                                                                glm::vec3(1.0f,1.0f,0.5f), 
                                                                                glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                            )
                                                )
                        );
                    lights.push_back(std::make_shared<Light>(glm::vec3(-1.0f, 1.0f, 1.0f),1.0f));
                    break; 
                case 7:
                    objects.push_back( // 1
                            std::make_shared<Mesh>(
                                                    glm::vec3(0.3f, -1.5f, 0.0f), 
                                                    glm::vec3(1.5f,1.5f,1.5f), 
                                                    (20.0f*glm::pi<float>())/180.0f, glm::vec3(1.0f,0.0f,0.0f),
                                                    std::make_shared<Material>(
                                                                                glm::vec3(0.0f,0.0f,1.0f), 
                                                                                glm::vec3(1.0f,1.0f,0.5f), 
                                                                                glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                            )
                                                )
                        );
                    lights.push_back(std::make_shared<Light>(glm::vec3(1.0f, 1.0f, 2.0f),1.0f));
                    break;
                case 8:
                    camera->set_pos(glm::vec3(-3.0f, 0.0f, 0.0f));
                    camera->rotate(0.0f, 90.0f); //pitch, yaw
                    camera->set_fov(60.0f);
                    objects.push_back(
                            std::make_shared<Sphere>(
                                                        glm::vec3(-0.5f, -1.0f, 1.0f), 
                                                        glm::vec3(1.0f,1.0f,1.0f), 
                                                        0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
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
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,1.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                        )
                                                            )
                                    );

                    lights.push_back(std::make_shared<Light>(glm::vec3(-2.0f, 1.0f, 1.0f),1.0f));
                    break;
                case 9:
                    objects.push_back(
                            std::make_shared<Sphere>(
                                                        glm::vec3(0.0f, 0.0f, 1.0f), 
                                                        glm::vec3(1.0f,1.0f,1.0f), 
                                                        0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                        std::make_shared<Material>(
                                                                                    glm::vec3(1.0f,0.0f,0.0f), 
                                                                                    glm::vec3(1.0f,1.0f,0.5f), 
                                                                                    glm::vec3(0.1f,0.1f,0.1f), 100.0f, "transparent", 1.04f
                                                                                )
                                                    )
                            );

                    objects.push_back(
                                    std::make_shared<Sphere>(
                                                                glm::vec3(1.0f, -0.6f, -2.5f), 
                                                                glm::vec3(1.0f,1.0f,1.0f), 
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,1.0f,0.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f,"reflective"
                                                                                        )
                                                            )
                                    );
                    objects.push_back(
                                    std::make_shared<Sphere>(
                                                                glm::vec3(-0.5f, 4.0f, 10.0f), 
                                                                glm::vec3(5.0f,5.0f,5.0f), 
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,1.0f,0.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f,"reflective"
                                                                                        )
                                                            )
                                    );

                    objects.push_back(
                                    std::make_shared<Sphere>(
                                                                glm::vec3(-1.0f, 0.6f, -2.5f), 
                                                                glm::vec3(1.0f,1.0f,1.0f), 
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(0.0f,0.0f,1.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                        )
                                                            )
                                    );

                    objects.push_back(
                                    std::make_shared<Sphere>(
                                                                glm::vec3(0.0f, -1.5f, -1.0f), 
                                                                glm::vec3(0.5f,0.5f,0.5f), 
                                                                0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                                std::make_shared<Material>(
                                                                                            glm::vec3(1.0f,0.0f,0.0f), 
                                                                                            glm::vec3(1.0f,1.0f,0.5f), 
                                                                                            glm::vec3(0.1f,0.1f,0.1f), 100.0f
                                                                                        )
                                                            )
                                    );

                    objects.push_back( // floor
                                    std::make_shared<Plane>(
                                                            glm::vec3(0.0f, -2.0f, 0.0f), 
                                                            glm::vec3(0.0f,1.0f,0.0f), 
                                                            0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                            std::make_shared<Material>(
                                                                                        glm::vec3(1.0f,1.0f,1.0f), 
                                                                                        glm::vec3(0.0f,0.0f,0.0f), 
                                                                                        glm::vec3(0.1f,0.1f,0.1f), 0.0f
                                                                                    )
                                                        )
                                );

                    objects.push_back( // wall
                                    std::make_shared<Plane>(
                                                            glm::vec3(0.0f, 0.0f, -10.0f), 
                                                            glm::vec3(0.0f,0.0f,1.0f), 
                                                            0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                            std::make_shared<Material>(
                                                                                        glm::vec3(1.0f,1.0f,1.0f), 
                                                                                        glm::vec3(0.0f,0.0f,0.0f), 
                                                                                        glm::vec3(0.1f,0.1f,0.1f), 0.0f
                                                                                    )
                                                        )
                                );

                    objects.push_back( // wall
                                    std::make_shared<Plane>(
                                                            glm::vec3(0.0f, 0.0f, 15.0f), 
                                                            glm::vec3(0.0f,0.0f,-1.0f), 
                                                            0.0f, glm::vec3(0.0f,1.0f,0.0f),
                                                            std::make_shared<Material>(
                                                                                        glm::vec3(1.0f,1.0f,1.0f), 
                                                                                        glm::vec3(0.0f,0.0f,0.0f), 
                                                                                        glm::vec3(0.1f,0.1f,0.1f), 0.0f
                                                                                    )
                                                        )
                                );

                    lights.push_back(std::make_shared<Light>(glm::vec3(0.0f, 5.0f, 0.0f),1.0f));
                    break;
                    
            }
        }

        glm::vec3 compute_ray_color(std::shared_ptr<Ray> ray, float t0, float t1,int recursions=0){
            Hit buff = Hit();
            buff.set_distance(t1);
            glm::vec3 color(0.0);
            glm::vec3 l,h,dir,x,v;

            if (hit(ray, t0, t1, buff)){ // remove ray saving if not needed for hit class
                x = ray->o() + (ray->xyz() * buff.get_distance());

                if (buff.get_mat().type == "reflective") {
                    glm::vec3 reflected_dir = glm::reflect(ray->xyz(), buff.n()); //HERE
                    std::shared_ptr<Ray> reflected_ray = std::make_shared<Ray>(reflected_dir , x);
                    return recursions < 100 ? 0.9f * compute_ray_color(reflected_ray ,t0,t1, recursions+1) : color;
                } else if (buff.get_mat().type == "transparent") {
                    glm::vec3 refracted_dir = glm::refract(ray->xyz(), buff.n(), glm::dot(ray->xyz(), buff.n()) > 0.0f ? buff.get_mat().util/1.0f : 1.0f/buff.get_mat().util); //HERE
                    std::shared_ptr<Ray> refracted_ray = std::make_shared<Ray>(refracted_dir , x);
                    return recursions < 100 ? compute_ray_color(refracted_ray ,t0,t1, recursions+1) : color;
                } 
                color += buff.get_mat().ka;

                
                float light_dist;

                for (auto light : lights) {
                    
                    dir = light->get_position() - x;

                    Hit light_buff = Hit();
                    std::shared_ptr<Ray> light_ray = std::make_shared<Ray>(dir,x);

                    light_dist = glm::length(dir);
                    
                    if (!hit(light_ray, t0, light_dist, light_buff, true)){ //get light ray , epsilon, dist to light    
                        l = glm::normalize(dir);
                        v = glm::normalize(ray->o() - x);
                        h = glm::normalize(l + v);
                        color.r += light->I() * ( ( buff.get_mat().kd.r * std::max(0.0f, glm::dot(l, buff.n())) ) + (buff.get_mat().ks.r * glm::pow(std::max(0.0f, glm::dot(h,buff.n())), buff.get_mat().s) ));
                        color.g += light->I() * ( ( buff.get_mat().kd.g * std::max(0.0f, glm::dot(l, buff.n())) ) + (buff.get_mat().ks.g * glm::pow(std::max(0.0f, glm::dot(h,buff.n())), buff.get_mat().s) ));
                        color.b += light->I() * ( ( buff.get_mat().kd.b * std::max(0.0f, glm::dot(l, buff.n())) ) + (buff.get_mat().ks.b * glm::pow(std::max(0.0f, glm::dot(h,buff.n())), buff.get_mat().s) ));

                    }
                }
            }
            return color;
            
        }

        bool hit(std::shared_ptr<Ray> ray, float start, float stop, Hit& buff, bool l=false){
            Hit temp = Hit();
            for (auto object : objects) { 
                
                temp = object->intersect(ray, start, stop); 

                if (!temp.empty()){
                    if (l) return true;
                    if (buff.get_distance() > temp.get_distance()) {
                        buff = temp;
                    }
                }  
            }
            
            return !buff.empty(); 
        }

    private:
        int scene;
        std::shared_ptr<Image> image;
        std::vector< std::shared_ptr<Object> > objects;
        std::vector< std::shared_ptr<Light> > lights;
        std::shared_ptr<Camera> camera;
        SafeQueue< std::pair<std::shared_ptr<Ray>,glm::vec2> > rays;
        std::string outputFile;
};

#endif