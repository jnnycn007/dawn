#version 310 es


struct Inner_std140 {
  float scalar_f32;
  int scalar_i32;
  uint scalar_u32;
  uint tint_pad_0;
  vec2 vec2_f32;
  ivec2 vec2_i32;
  uvec2 vec2_u32;
  uint tint_pad_1;
  uint tint_pad_2;
  vec3 vec3_f32;
  uint tint_pad_3;
  ivec3 vec3_i32;
  uint tint_pad_4;
  uvec3 vec3_u32;
  uint tint_pad_5;
  vec4 vec4_f32;
  ivec4 vec4_i32;
  uvec4 vec4_u32;
  vec2 mat2x2_f32_col0;
  vec2 mat2x2_f32_col1;
  vec3 mat2x3_f32_col0;
  uint tint_pad_6;
  vec3 mat2x3_f32_col1;
  uint tint_pad_7;
  mat2x4 mat2x4_f32;
  vec2 mat3x2_f32_col0;
  vec2 mat3x2_f32_col1;
  vec2 mat3x2_f32_col2;
  uint tint_pad_8;
  uint tint_pad_9;
  vec3 mat3x3_f32_col0;
  uint tint_pad_10;
  vec3 mat3x3_f32_col1;
  uint tint_pad_11;
  vec3 mat3x3_f32_col2;
  uint tint_pad_12;
  mat3x4 mat3x4_f32;
  vec2 mat4x2_f32_col0;
  vec2 mat4x2_f32_col1;
  vec2 mat4x2_f32_col2;
  vec2 mat4x2_f32_col3;
  vec3 mat4x3_f32_col0;
  uint tint_pad_13;
  vec3 mat4x3_f32_col1;
  uint tint_pad_14;
  vec3 mat4x3_f32_col2;
  uint tint_pad_15;
  vec3 mat4x3_f32_col3;
  uint tint_pad_16;
  mat4 mat4x4_f32;
  vec3 arr2_vec3_f32[2];
};

struct S_std140 {
  Inner_std140 arr[8];
};

layout(binding = 0, std140)
uniform ub_block_std140_1_ubo {
  S_std140 inner;
} v;
layout(binding = 1, std430)
buffer s_block_1_ssbo {
  int inner;
} v_1;
int tint_f32_to_i32(float value) {
  return int(clamp(value, -2147483648.0f, 2147483520.0f));
}
void main_inner(uint idx) {
  float scalar_f32 = v.inner.arr[min(idx, 7u)].scalar_f32;
  int scalar_i32 = v.inner.arr[min(idx, 7u)].scalar_i32;
  uint scalar_u32 = v.inner.arr[min(idx, 7u)].scalar_u32;
  vec2 vec2_f32 = v.inner.arr[min(idx, 7u)].vec2_f32;
  ivec2 vec2_i32 = v.inner.arr[min(idx, 7u)].vec2_i32;
  uvec2 vec2_u32 = v.inner.arr[min(idx, 7u)].vec2_u32;
  vec3 vec3_f32 = v.inner.arr[min(idx, 7u)].vec3_f32;
  ivec3 vec3_i32 = v.inner.arr[min(idx, 7u)].vec3_i32;
  uvec3 vec3_u32 = v.inner.arr[min(idx, 7u)].vec3_u32;
  vec4 vec4_f32 = v.inner.arr[min(idx, 7u)].vec4_f32;
  ivec4 vec4_i32 = v.inner.arr[min(idx, 7u)].vec4_i32;
  uvec4 vec4_u32 = v.inner.arr[min(idx, 7u)].vec4_u32;
  mat2 mat2x2_f32 = mat2(v.inner.arr[min(idx, 7u)].mat2x2_f32_col0, v.inner.arr[min(idx, 7u)].mat2x2_f32_col1);
  mat2x3 mat2x3_f32 = mat2x3(v.inner.arr[min(idx, 7u)].mat2x3_f32_col0, v.inner.arr[min(idx, 7u)].mat2x3_f32_col1);
  mat2x4 mat2x4_f32 = v.inner.arr[min(idx, 7u)].mat2x4_f32;
  mat3x2 mat3x2_f32 = mat3x2(v.inner.arr[min(idx, 7u)].mat3x2_f32_col0, v.inner.arr[min(idx, 7u)].mat3x2_f32_col1, v.inner.arr[min(idx, 7u)].mat3x2_f32_col2);
  mat3 mat3x3_f32 = mat3(v.inner.arr[min(idx, 7u)].mat3x3_f32_col0, v.inner.arr[min(idx, 7u)].mat3x3_f32_col1, v.inner.arr[min(idx, 7u)].mat3x3_f32_col2);
  mat3x4 mat3x4_f32 = v.inner.arr[min(idx, 7u)].mat3x4_f32;
  mat4x2 mat4x2_f32 = mat4x2(v.inner.arr[min(idx, 7u)].mat4x2_f32_col0, v.inner.arr[min(idx, 7u)].mat4x2_f32_col1, v.inner.arr[min(idx, 7u)].mat4x2_f32_col2, v.inner.arr[min(idx, 7u)].mat4x2_f32_col3);
  mat4x3 mat4x3_f32 = mat4x3(v.inner.arr[min(idx, 7u)].mat4x3_f32_col0, v.inner.arr[min(idx, 7u)].mat4x3_f32_col1, v.inner.arr[min(idx, 7u)].mat4x3_f32_col2, v.inner.arr[min(idx, 7u)].mat4x3_f32_col3);
  mat4 mat4x4_f32 = v.inner.arr[min(idx, 7u)].mat4x4_f32;
  vec3 arr2_vec3_f32[2] = v.inner.arr[min(idx, 7u)].arr2_vec3_f32;
  int v_2 = (tint_f32_to_i32(scalar_f32) + scalar_i32);
  int v_3 = (v_2 + int(scalar_u32));
  int v_4 = ((v_3 + tint_f32_to_i32(vec2_f32.x)) + vec2_i32.x);
  int v_5 = (v_4 + int(vec2_u32.x));
  int v_6 = ((v_5 + tint_f32_to_i32(vec3_f32.y)) + vec3_i32.y);
  int v_7 = (v_6 + int(vec3_u32.y));
  int v_8 = ((v_7 + tint_f32_to_i32(vec4_f32.z)) + vec4_i32.z);
  int v_9 = (v_8 + int(vec4_u32.z));
  int v_10 = (v_9 + tint_f32_to_i32(mat2x2_f32[0u].x));
  int v_11 = (v_10 + tint_f32_to_i32(mat2x3_f32[0u].x));
  int v_12 = (v_11 + tint_f32_to_i32(mat2x4_f32[0u].x));
  int v_13 = (v_12 + tint_f32_to_i32(mat3x2_f32[0u].x));
  int v_14 = (v_13 + tint_f32_to_i32(mat3x3_f32[0u].x));
  int v_15 = (v_14 + tint_f32_to_i32(mat3x4_f32[0u].x));
  int v_16 = (v_15 + tint_f32_to_i32(mat4x2_f32[0u].x));
  int v_17 = (v_16 + tint_f32_to_i32(mat4x3_f32[0u].x));
  int v_18 = (v_17 + tint_f32_to_i32(mat4x4_f32[0u].x));
  v_1.inner = (v_18 + tint_f32_to_i32(arr2_vec3_f32[0u].x));
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  main_inner(gl_LocalInvocationIndex);
}
