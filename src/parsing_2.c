#include "main.h"
#include "parsing.h"

t_optional_material parse_ambient(const t_optional_array elems)
{
  t_optional_double ambient_ratio;
  t_optional_rgb ambient_color;
  static bool already_parsed = false;

  if (elems.error || already_parsed)
	  return (t_optional_material) {.error = true};
  ambient_ratio = get_ratio(elems.value[1]);
  ambient_color = get_rgb(elems.value[2]);
  if (ambient_ratio.error || ambient_color.error)
    return (t_optional_material) {.error = true};
  already_parsed = true;
  return (t_optional_material) {
    .value = {
      .ambient_ratio = ambient_ratio.value,
      .ambient_color = ambient_color.value,
    },
	.error = false,
  };
}

t_optional_transform get_camera_view(char *from, char *to)
{
  const t_optional_transform error = {.error = true};
  t_optional_point p_from;
  t_optional_point p_to;

  p_from = get_position(from);
  p_to = get_position(to);
  if (p_from.error || p_to.error)
    return error;
  // print_tupil("from", p_from.value);
  // print_tupil("to", p_to.value);
  return (t_optional_transform) {
    .value = view_transform(p_from.value, p_to.value, vector(0, 1, 0)),
    .error = false,
  };
}

t_optional_camera parse_camera(const t_optional_array elems, t_res res)
{
  const t_optional_camera error = {.error = true};
  t_optional_transform camera_view;
  t_camera cam;
  t_optional_double fov;
  static bool already_parsed = false;

  if (elems.error || already_parsed)
	  return error;
  camera_view = get_camera_view(elems.value[1], elems.value[2]);
  fov = get_fov(elems.value[3]);
  if (camera_view.error || fov.error)
	return error;
  cam = camera(res.width, res.height, fov.value);
  cam.transform = camera_view.value;
  already_parsed = true;
  return (t_optional_camera) {
	.value = cam,
	.error = false,
  };
}

t_optional_light parse_light(const t_optional_array elems)
{
  const t_optional_light error = {.error = true};
  t_optional_point pos;
  t_optional_double brightness;
  t_optional_rgb color;
  static bool already_parsed = false;

  if (elems.error || already_parsed)
	  return error;
  pos = get_position(elems.value[1]);
  brightness = get_ratio(elems.value[2]);
  color = get_rgb(elems.value[3]);
  if (pos.error || brightness.error || color.error)
    return error;
  already_parsed = true;
  return (t_optional_light) {
	.value = point_light(pos.value, rgb_scalar(color.value, brightness.value)),
	.error = false,
  };
}

t_optional_plane parse_plane(const t_optional_array elems)
{
  const t_optional_plane error = {.error = true};
  t_optional_point p;
  t_optional_point  normal;
  t_optional_rgb  color;

  if (elems.error)
	  return error;
  p = get_position(elems.value[1]);
  normal = get_position(elems.value[2]);
  color = get_rgb(elems.value[3]); 
  if (p.error || normal.error || color.error)
    return error;
  t_plane pl = plane();
  normal.value.w = 0;
  pl = make_plane(p.value, normal.value);
  pl.material = material();
  pl.material.color = color.value;
  pl.transform = identity();
  // print_tupil("point", p.value);
  // print_tupil("normal", normal.value);
  return (t_optional_plane){
    .value = pl,
    .error = false,
  };
}

t_optional_sphere parse_sphere(const t_optional_array elems)
{
  const t_optional_sphere error = {.error = true};
  t_optional_point center;
  t_optional_double  diameter;
  t_optional_rgb  color;

  if (elems.error)
	  return error;
  center = get_position(elems.value[1]);
  diameter = get_double(elems.value[2]);
  color = get_rgb(elems.value[3]);
  if (center.error || diameter.error || color.error)
	return error;
  double radius = diameter.value / 2.0;
  t_sphere s = make_sphere(center.value, radius, color.value);
  // t_sphere s = sphere();
  // s.material.color = color.value;
  // s.t = transform(
		// 		  translation(center.value.x, center.value.y, center.value.z),
		// 		  scaling(radius, radius, radius),
		// 		  identity()
		// 		  );
  return (t_optional_sphere){
	.value = s,
	.error = false,
  };
}


t_optional_shape parse_shape(const t_optional_array elems, t_line_type type)
{
  t_optional_sphere sp;
  t_optional_plane pl;

  if (!elems.error && type == e_sphere) {
    sp = parse_sphere(elems);
    if (sp.error)
      return (t_optional_shape) {.error = true};
    return (t_optional_shape){
      .value = (t_shape)sp.value,
    } ;
  }
  if (!elems.error && type == e_plane) {
    pl = parse_plane(elems);
    if (pl.error)
      return (t_optional_shape) {.error = true};
    return (t_optional_shape){
      .value = (t_shape)pl.value,
    } ;
  }
  return (t_optional_shape){.value = true};
}
