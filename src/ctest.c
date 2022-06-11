#include "main.h"

int main()
{
  init_mlx(pair(2000, 1000), pair(0, 0));

  t_sphere floor = sphere();
  floor.material = material();
  floor.t = scaling(10, 0.01, 10);
  floor.material.color = color(1, 0.9, 0.9);
  floor.material.specular = 0;
  
  t_sphere left_wall = sphere();
  left_wall.material = material();
  left_wall.t = transform(
                          scaling(10, 0.01, 10),
                          mat4_mult(rotation_y(-M_PI_4), rotation_x(M_PI_2)),
                          translation(0, 0, 5)
    );
  left_wall.material = floor.material;


  t_sphere right_wall = sphere();
  right_wall.material = material();
  right_wall.t = transform(
                          scaling(10, 0.01, 10),
                          mat4_mult(rotation_y(M_PI_4), rotation_x(M_PI_2)),
                          translation(0, 0, 5)
    );
  right_wall.material = floor.material;

  t_sphere middle = sphere();
  middle.t = translation(-0.5, 1, 0.5);
  middle.material = material();
  middle.material.color = color(0.1, 1, 0.5);
  middle.material.diffuse = 0.7;
  middle.material.specular = 0.3;

  t_sphere right = sphere();
  right.t = mat4_mult(translation(1.5, 0.5, -0.5),
                      scaling(0.5, 0.5, 0.5));
  right.material = material();
  right.material.color = color(0.5, 1, 0.1);
  right.material.diffuse = 0.7;
  right.material.specular = 0.3;

  t_sphere left = sphere();
  left.material = material();
  left.t = mat4_mult(translation(-1.5, 0.33, -0.75),
                      scaling(0.33, 0.33, 0.33));
  left.material.color = color(1, 0.8, 0.1);
  left.material.diffuse = 0.7;
  left.material.specular = 0.3;

  t_world w = world();
  t_light light = point_light(point(-10, 10, -10), color(1, 1, 1));
  t_camera c = camera(2000, 1000, M_PI / 3);
  c.transform = view_transform(point(0, 1.5, -5),
                               point(0, 1, 0),
                               vector(0, 1, 0));
  w = add_light(w, light);

  w = add_sphere(w, floor);
  w = add_sphere(w, left_wall);
  w = add_sphere(w, right_wall);

  w = add_sphere(w, middle);
  w = add_sphere(w, right);
  w = add_sphere(w, left);
  printf("num of spheres %d num of lights %d\n", w.amount_of_spheres, w.amount_of_lights);
  t_image canvas = render(c, w);
  put_image(pair(0, 0), canvas);
  start_mlx();
}