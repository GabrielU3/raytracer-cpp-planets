//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================
#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
    hittable_list world;

    // --- O Ambiente: Piso e Fundo ---
    auto ground_material = make_shared<metal>(color(0.4, 0.4, 0.4), 0.1);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    // --- As "Peças de Arte": Os Três Orbes Principais ---
    auto material_glass = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material_glass));

    auto material_diffuse = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material_diffuse));

    auto material_metal = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material_metal));


    // --- BONECO DE VIDRO (LADO DIREITO) ---
    auto material_glass_doll = make_shared<dielectric>(1.5);
    point3 glass_doll_base(2.0, 0.0, -1.5);
    // Cabeça
    world.add(make_shared<sphere>(glass_doll_base + point3(0, 1.15, 0), 0.25, material_glass_doll));
    // Tronco
    world.add(make_shared<sphere>(glass_doll_base + point3(0, 0.7, 0), 0.2, material_glass_doll));
    world.add(make_shared<sphere>(glass_doll_base + point3(0, 0.3, 0), 0.2, material_glass_doll));
    // Braços
    world.add(make_shared<sphere>(glass_doll_base + point3(-0.35, 0.75, 0), 0.1, material_glass_doll));
    world.add(make_shared<sphere>(glass_doll_base + point3(-0.6, 0.5, 0), 0.1, material_glass_doll));
    world.add(make_shared<sphere>(glass_doll_base + point3(0.35, 0.75, 0), 0.1, material_glass_doll));
    world.add(make_shared<sphere>(glass_doll_base + point3(0.6, 0.5, 0), 0.1, material_glass_doll));
    // Pernas
    world.add(make_shared<sphere>(glass_doll_base + point3(-0.15, -0.2, 0), 0.1, material_glass_doll));
    world.add(make_shared<sphere>(glass_doll_base + point3(-0.25, -0.6, 0), 0.1, material_glass_doll));
    world.add(make_shared<sphere>(glass_doll_base + point3(0.15, -0.2, 0), 0.1, material_glass_doll));
    world.add(make_shared<sphere>(glass_doll_base + point3(0.25, -0.6, 0), 0.1, material_glass_doll));


    // --- BONECO DE METAL (LADO ESQUERDO) ---
    auto material_metal_doll = make_shared<metal>(color(0.8, 0.8, 0.8), 0.05);
    point3 metal_doll_base(-2.0, 0.0, -1.0);
    // Cabeça
    world.add(make_shared<sphere>(metal_doll_base + point3(0, 1.15, 0), 0.25, material_metal_doll));
    // Tronco
    world.add(make_shared<sphere>(metal_doll_base + point3(0, 0.7, 0), 0.2, material_metal_doll));
    world.add(make_shared<sphere>(metal_doll_base + point3(0, 0.3, 0), 0.2, material_metal_doll));
    // Braços (um braço levantado em uma pose diferente)
    world.add(make_shared<sphere>(metal_doll_base + point3(-0.35, 0.9, 0), 0.1, material_metal_doll));
    world.add(make_shared<sphere>(metal_doll_base + point3(-0.6, 1.1, 0), 0.1, material_metal_doll));
    world.add(make_shared<sphere>(metal_doll_base + point3(0.35, 0.75, 0), 0.1, material_metal_doll));
    world.add(make_shared<sphere>(metal_doll_base + point3(0.6, 0.5, 0), 0.1, material_metal_doll));
    // Pernas
    world.add(make_shared<sphere>(metal_doll_base + point3(-0.15, -0.2, 0), 0.1, material_metal_doll));
    world.add(make_shared<sphere>(metal_doll_base + point3(-0.25, -0.6, 0), 0.1, material_metal_doll));
    world.add(make_shared<sphere>(metal_doll_base + point3(0.15, -0.2, 0), 0.1, material_metal_doll));
    world.add(make_shared<sphere>(metal_doll_base + point3(0.25, -0.6, 0), 0.1, material_metal_doll));


    // --- Orbes Secundários para Enriquecer a Cena ---
    for (int i = 0; i < 15; ++i) {
        auto radius = random_double(0.1, 0.3);
        point3 center(random_double(-8, 8), radius, random_double(-3, 3));
        auto choose_mat = random_double();
        if ((center - point3(4, 1, 0)).length() > 1.5 && (center - point3(-4, 1, 0)).length() > 1.5 && (center - point3(0, 1, 0)).length() > 1.5) {
             if (choose_mat < 0.7) {
                auto albedo = color::random(0, 1) * color::random(0, 1);
                world.add(make_shared<sphere>(center, radius, make_shared<lambertian>(albedo)));
            } else if (choose_mat < 0.85) {
                auto albedo = color::random(0.5, 1);
                auto fuzz = random_double(0, 0.2);
                world.add(make_shared<sphere>(center, radius, make_shared<metal>(albedo, fuzz)));
            } else {
                world.add(make_shared<sphere>(center, radius, make_shared<dielectric>(1.5)));
            }
        }
    }


    // --- Configuração da Câmera ---
    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 50;

    cam.vfov     = 25;
    cam.lookfrom = point3(0, 6, 13); 
    cam.lookat   = point3(0, 1, 0);
    cam.vup      = vec3(0, 1, 0);

    cam.defocus_angle = 0.8;
    cam.focus_dist    = (cam.lookfrom - cam.lookat).length();

    // Renderiza a cena!
    cam.render(world);
}
