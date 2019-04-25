#include <fstream>
#include <iostream>
#include <functional>
#include <random>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

#define MAXFLOAT 1.99999988079071044921875e127

//Chapter 1
hitable *random_scene() {
   int n = 50000;
   hitable **list = new hitable*[n+1];
   texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
   list[0] = new sphere(vec3(0.0, -1000.0, 0.0), 1000.0, new lambertian(checker));
   int i = 1;
   for(int a = -10; a < 10; a++){
      for(int b = -10; b < 10; b++){
         float choose_mat = rn();
         vec3 center(a + 0.9*rn(), 0.2, b+0.9*rn());
         if((center-vec3(4.0, 0.2, 0.0)).length() > 0.9){
            if(choose_mat < 0.8) { // diffuse
               list[i++] = new moving_sphere(center, center+vec3(0.0,0.5*rn(),0.0), 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec3(rn()*rn(), rn()*rn(), rn()*rn()))));
            }
            else if(choose_mat < 0.95) { // metal
               list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1.0 + rn()), 0.5*(1.0 + rn()), 0.5*(1.0 + rn())), 0.5*rn()));
            }
            else{
               list[i++] = new sphere(center, 0.2, new dielectric(1.5));
            }
         }
      }
   }
   list[i++] = new sphere(vec3(0.0, 1.0, 0.0), 1.0, new dielectric(1.5));
   list[i++] = new sphere(vec3(-4.0, 1.0, 0.0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
   list[i++] = new sphere(vec3(4.0, 1.0, 0.0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
   return new hitable_list(list, i);
}

vec3 color(const ray& r, hitable *world, int depth) {
   hit_record rec;
   if(world->hit(r, 0.001, MAXFLOAT, rec)){
      ray scattered;
      vec3 attenuation;
      if((depth < 50) && (rec.mat_ptr->scatter(r, rec, attenuation, scattered))) {
         return attenuation*color(scattered, world, depth+1);
      }
      else{
         return vec3(0, 0, 0);
      }
   }
   else{
      vec3 unit_direction = unit_vector(r.direction());
      float t = 0.5*(unit_direction.y() + 1.0);
      return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
   }
}

int main(){
   std::ofstream fo("img001.ppm");
   int nx = 200;
   int ny = 100;
   int ns = 20;
   fo << "P3\n" << nx << " " << ny << "\n255\n";
   hitable *world = random_scene();
   vec3 lookfrom(13.0, 2.0, 3.0);
   vec3 lookat(0.0, 0.0, 0.0);
   float dist_to_focus = 10.0;
   float aperture = 0.0;
   camera cam(lookfrom, lookat, vec3(0.0, 1.0, 0.0), 20, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);
   for(int j = ny-1; j >= 0; j--){
      for(int i = 0; i < nx; ++i){
         vec3 col(0, 0, 0);
         for(int s = 0; s < ns; ++s){
            float u = float(i + rn())/float(nx);
            float v = float(j + rn())/float(ny);
            ray r = cam.get_ray(u, v);
            col += color(r, world, 0);
         }
         col /= float(ns);
         col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
         int ir = int(255.99*col[0]);
         int ig = int(255.99*col[1]);
         int ib = int(255.99*col[2]);

         fo << ir << " " << ig << " " << ib << "\n";
      }
      std::cout << (float(ny-j)*100.0)/float(ny) << "%\n";
   }

   delete world;
   fo.close();
}
