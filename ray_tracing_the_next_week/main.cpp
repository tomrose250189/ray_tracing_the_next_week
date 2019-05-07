#include <fstream>
#include "sphere.h"
#include "box.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

hitable *two_spheres() 
{
	texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
	int n = 50;
	hitable **list = new hitable*[n+1];
	list[0] = new sphere(vec3(0.0,-10.0,0.0), 10.0, new lambertian(checker));
	list[1] = new sphere(vec3(0.0, 10.0, 0.0), 10.0, new lambertian(checker));
	return new hitable_list(list, 2);
}

hitable *two_perlin_spheres()
{
	texture *pertext = new noise_texture(2.0);
	hitable **list = new hitable*[2];
	list[0] = new sphere(vec3(0.0, -1000.0, 0.0), 1000.0, new lambertian(pertext));
	list[1] = new sphere(vec3(0.0, 2.0, 0.0), 2.0, new lambertian(pertext));
	return new hitable_list(list,2);
}

hitable *image_texture_test_scene()
{
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("simpsons.jpg", &nx, &ny, &nn, 0);
	material *mat = new lambertian(new image_texture(tex_data, nx, ny));
	hitable **list = new hitable*[1];
	list[0] = new sphere(vec3(0.0, 0.0, 0.0), 5.0, mat);
	return new hitable_list(list, 1);
}

hitable *simple_light()
{
	texture *pertext = new noise_texture(4.0);
	hitable **list = new hitable*[4];
	list[0] = new sphere(vec3(0.0, -1000.0, 0.0), 1000.0, new lambertian(pertext));
	list[1] = new sphere(vec3(0.0, 2.0, 0.0), 2.0, new lambertian(pertext));
	list[2] = new sphere(vec3(0.0, 7.0, 0.0), 2.0, new diffuse_light(new constant_texture(vec3(4.0, 4.0, 4.0))));
	list[3] = new xy_rect(3.0, 5.0, 1.0, 3.0, -2.0, new diffuse_light(new constant_texture(vec3(4.0, 4.0, 4.0))));
	return new hitable_list(list, 4);
}

hitable *cornell_box()
{
	hitable **list = new hitable*[8];
	int i = 0;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	return new hitable_list(list, i);
}

hitable *rotated_box()
{
	hitable **list = new hitable*[1];
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	list[0] = new rotate_y(new translate(new box(vec3(-1, -1, -1), vec3(1, 1, 1), red), vec3(5, 0, 0)), 45);
	return new hitable_list(list, 1);
}

vec3 color(const ray& r, hitable *world, int depth) {
   hit_record rec;
   if(world->hit(r, 0.001, MAXFLOAT, rec)){
      ray scattered;
      vec3 attenuation;
	  vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
      if((depth < 50) && (rec.mat_ptr->scatter(r, rec, attenuation, scattered))) {
         return emitted + attenuation*color(scattered, world, depth+1);
	  }
      else{
         return emitted;
      }
   }
   else{
	   return vec3(0, 0, 0);
   }
}

int main(){
   std::ofstream fo("img016.ppm");
   int nx = 800;
   int ny = 400;
   int ns = 500;
   fo << "P3\n" << nx << " " << ny << "\n255\n";
   //hitable *world = random_scene();
   //hitable *world = two_spheres();
   //hitable *world = two_perlin_spheres();
   //hitable *world = image_texture_test_scene();
   //hitable *world = simple_light();
   hitable *world = cornell_box();
   //hitable *world = rotated_box();
   vec3 lookfrom(278.0, 278.0, -800.0);
   vec3 lookat(278.0, 278.0, 0.0);
   float dist_to_focus = 10.0;
   float aperture = 0.0;
   float vfov = 40.0;
   camera cam(lookfrom, lookat, vec3(0.0, 1.0, 0.0), vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);
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
