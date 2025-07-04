#version 310 es
#extension GL_AMD_gpu_shader_half_float: require

float16_t t = 0.0hf;
f16vec3 m() {
  t = 1.0hf;
  return f16vec3(t);
}
uvec3 tint_v3f16_to_v3u32(f16vec3 value) {
  return uvec3(clamp(value, f16vec3(0.0hf), f16vec3(65504.0hf)));
}
void f() {
  uvec3 v = tint_v3f16_to_v3u32(m());
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
}
