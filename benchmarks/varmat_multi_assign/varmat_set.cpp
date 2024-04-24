#include <utils/setup.hpp>
#include <stan/math/mix.hpp>
#include <stan/model/indexing/assign_varmat.hpp>
#include <benchmark/benchmark.h>

BENCHMARK_DEFINE_F(ArenaAlloc, multi_assign)(benchmark::State& state) {
  using stan::math::var;
  using stan::math::var_value;
  using stan::math::sum;
  Eigen::Matrix<double, -1, 1> x_vals = Eigen::Matrix<double, -1, 1>::Random(state.range(0));
  Eigen::Matrix<double, -1, 1> y_vals = Eigen::Matrix<double, -1, 1>::Random(state.range(0) / 2);
  std::vector<int> idx_vec(state.range(0) / 2, 0);
  std::size_t sentinal = 0;
  for (int i = state.range(0) / 2; i >= 0; --i) {
    idx_vec[sentinal] = i;
    ++sentinal;
  }
  std::random_shuffle(idx_vec.begin(), idx_vec.end());
  const stan::model::index_multi idxs(idx_vec);
  for (auto _ : state) {
    var_value<Eigen::Matrix<double, -1, 1>> x = x_vals;
    var_value<Eigen::Matrix<double, -1, 1>> z = x_vals;
    var_value<Eigen::Matrix<double, -1, 1>> y = y_vals;
    auto start = std::chrono::high_resolution_clock::now();
    stan::model::assign(x, y, "", idxs);
    sum(stan::math::add(stan::math::dot_product(x, z), z)).grad();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    state.SetIterationTime(elapsed_seconds.count());
    benchmark::ClobberMemory();
    stan::math::recover_memory();
  }
}

int start_val = 2;
int end_val = 1024;
BENCHMARK_REGISTER_F(ArenaAlloc, multi_assign)->RangeMultiplier(2)->Range(start_val, end_val)->UseManualTime();
BENCHMARK_MAIN();
