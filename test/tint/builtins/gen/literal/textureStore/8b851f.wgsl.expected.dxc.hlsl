//
// fragment_main
//

RWTexture3D<float4> arg_0 : register(u0, space1);
void textureStore_8b851f() {
  arg_0[(int(1)).xxx] = (1.0f).xxxx;
}

void fragment_main() {
  textureStore_8b851f();
}

//
// compute_main
//

RWTexture3D<float4> arg_0 : register(u0, space1);
void textureStore_8b851f() {
  arg_0[(int(1)).xxx] = (1.0f).xxxx;
}

[numthreads(1, 1, 1)]
void compute_main() {
  textureStore_8b851f();
}

