@group(1) @binding(0) var arg_0 : texture_storage_1d<rgb10a2uint, read_write>;

fn textureStore_343f2a() {
  textureStore(arg_0, 1u, vec4<u32>(1u));
}

@fragment
fn fragment_main() {
  textureStore_343f2a();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureStore_343f2a();
}
