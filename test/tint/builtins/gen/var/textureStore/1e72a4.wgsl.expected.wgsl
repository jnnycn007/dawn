@group(1) @binding(0) var arg_0 : texture_storage_2d<rg16sint, write>;

fn textureStore_1e72a4() {
  var arg_1 = vec2<i32>(1i);
  var arg_2 = vec4<i32>(1i);
  textureStore(arg_0, arg_1, arg_2);
}

@fragment
fn fragment_main() {
  textureStore_1e72a4();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureStore_1e72a4();
}
