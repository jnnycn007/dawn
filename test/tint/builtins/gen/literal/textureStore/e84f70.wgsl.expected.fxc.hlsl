//
// fragment_main
//

RWTexture2DArray<int4> arg_0 : register(u0, space1);
void textureStore_e84f70() {
  arg_0[int3((int(1)).xx, int(1))] = (int(1)).xxxx;
}

void fragment_main() {
  textureStore_e84f70();
}

//
// compute_main
//

RWTexture2DArray<int4> arg_0 : register(u0, space1);
void textureStore_e84f70() {
  arg_0[int3((int(1)).xx, int(1))] = (int(1)).xxxx;
}

[numthreads(1, 1, 1)]
void compute_main() {
  textureStore_e84f70();
}

