#include <matmul_aos_soa/setup.hpp>
// Just to kick off the stack allocation
static void static_matmul_add(benchmark::State& state) {
  using stan::math::var;
  Eigen::Matrix<double, -1, -1> x_vals = Eigen::MatrixXd::Random(state.range(0), state.range(0));
  Eigen::Matrix<double, -1, -1> y_vals = Eigen::MatrixXd::Random(state.range(0), state.range(0));
  using stan::math::var;
  using stan::math::sum;
  stan::math::var_value<Eigen::Matrix<double, -1, -1>> x = x_vals;
  stan::math::var_value<Eigen::Matrix<double, -1, -1>> y = y_vals;
  var lp = 0;
  lp -= sum(x * y + x);
  benchmark::DoNotOptimize(lp.vi_);
  for (auto _ : state) {
    auto start = std::chrono::high_resolution_clock::now();
    lp.grad();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    state.SetIterationTime(elapsed_seconds.count());
    stan::math::set_zero_all_adjoints();
    benchmark::ClobberMemory();
  }
  stan::math::recover_memory();
}

constexpr int start_val = 2;
constexpr int end_val = 1024;
BENCHMARK(toss_me);
BENCHMARK(static_matmul_add)->RangeMultiplier(2)->Range(start_val, end_val)->UseManualTime();
BENCHMARK_MAIN();