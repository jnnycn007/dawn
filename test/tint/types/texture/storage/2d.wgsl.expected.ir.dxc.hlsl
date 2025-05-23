
RWTexture2D<float4> t_rgba8unorm : register(u0);
RWTexture2D<float4> t_rgba8snorm : register(u1);
RWTexture2D<uint4> t_rgba8uint : register(u2);
RWTexture2D<int4> t_rgba8sint : register(u3);
RWTexture2D<uint4> t_rgba16uint : register(u4);
RWTexture2D<int4> t_rgba16sint : register(u5);
RWTexture2D<float4> t_rgba16float : register(u6);
RWTexture2D<uint4> t_r32uint : register(u7);
RWTexture2D<int4> t_r32sint : register(u8);
RWTexture2D<float4> t_r32float : register(u9);
RWTexture2D<uint4> t_rg32uint : register(u10);
RWTexture2D<int4> t_rg32sint : register(u11);
RWTexture2D<float4> t_rg32float : register(u12);
RWTexture2D<uint4> t_rgba32uint : register(u13);
RWTexture2D<int4> t_rgba32sint : register(u14);
RWTexture2D<float4> t_rgba32float : register(u15);
[numthreads(1, 1, 1)]
void main() {
  uint2 v = (0u).xx;
  t_rgba8unorm.GetDimensions(v.x, v.y);
  uint2 dim1 = v;
  uint2 v_1 = (0u).xx;
  t_rgba8snorm.GetDimensions(v_1.x, v_1.y);
  uint2 dim2 = v_1;
  uint2 v_2 = (0u).xx;
  t_rgba8uint.GetDimensions(v_2.x, v_2.y);
  uint2 dim3 = v_2;
  uint2 v_3 = (0u).xx;
  t_rgba8sint.GetDimensions(v_3.x, v_3.y);
  uint2 dim4 = v_3;
  uint2 v_4 = (0u).xx;
  t_rgba16uint.GetDimensions(v_4.x, v_4.y);
  uint2 dim5 = v_4;
  uint2 v_5 = (0u).xx;
  t_rgba16sint.GetDimensions(v_5.x, v_5.y);
  uint2 dim6 = v_5;
  uint2 v_6 = (0u).xx;
  t_rgba16float.GetDimensions(v_6.x, v_6.y);
  uint2 dim7 = v_6;
  uint2 v_7 = (0u).xx;
  t_r32uint.GetDimensions(v_7.x, v_7.y);
  uint2 dim8 = v_7;
  uint2 v_8 = (0u).xx;
  t_r32sint.GetDimensions(v_8.x, v_8.y);
  uint2 dim9 = v_8;
  uint2 v_9 = (0u).xx;
  t_r32float.GetDimensions(v_9.x, v_9.y);
  uint2 dim10 = v_9;
  uint2 v_10 = (0u).xx;
  t_rg32uint.GetDimensions(v_10.x, v_10.y);
  uint2 dim11 = v_10;
  uint2 v_11 = (0u).xx;
  t_rg32sint.GetDimensions(v_11.x, v_11.y);
  uint2 dim12 = v_11;
  uint2 v_12 = (0u).xx;
  t_rg32float.GetDimensions(v_12.x, v_12.y);
  uint2 dim13 = v_12;
  uint2 v_13 = (0u).xx;
  t_rgba32uint.GetDimensions(v_13.x, v_13.y);
  uint2 dim14 = v_13;
  uint2 v_14 = (0u).xx;
  t_rgba32sint.GetDimensions(v_14.x, v_14.y);
  uint2 dim15 = v_14;
  uint2 v_15 = (0u).xx;
  t_rgba32float.GetDimensions(v_15.x, v_15.y);
  uint2 dim16 = v_15;
}

