enable chromium_experimental_subgroup_matrix;

@group(0) @binding(0) var<storage, read_write> prevent_dce : array<u32, 1024>;

fn subgroupMatrixMultiplyAccumulate_052cba() -> subgroup_matrix_result<u8, 8, 8> {
  var arg_0 = subgroup_matrix_left<u8, 8, 8>();
  var arg_1 = subgroup_matrix_right<u8, 8, 8>();
  var arg_2 = subgroup_matrix_result<u8, 8, 8>();
  var res : subgroup_matrix_result<u8, 8, 8> = subgroupMatrixMultiplyAccumulate(arg_0, arg_1, arg_2);
  return res;
}

@compute @workgroup_size(1)
fn compute_main() {
  subgroupMatrixStore(&(prevent_dce), 0, subgroupMatrixMultiplyAccumulate_052cba(), false, 64);
}
