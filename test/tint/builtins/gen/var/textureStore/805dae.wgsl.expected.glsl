//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, rgba8_snorm) uniform highp writeonly image2D f_arg_0;
void textureStore_805dae() {
  ivec2 arg_1 = ivec2(1);
  vec4 arg_2 = vec4(1.0f);
  imageStore(f_arg_0, arg_1, arg_2);
}
void main() {
  textureStore_805dae();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, rgba8_snorm) uniform highp writeonly image2D arg_0;
void textureStore_805dae() {
  ivec2 arg_1 = ivec2(1);
  vec4 arg_2 = vec4(1.0f);
  imageStore(arg_0, arg_1, arg_2);
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  textureStore_805dae();
}
