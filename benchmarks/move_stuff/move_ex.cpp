#include <matmul_aos_soa/setup.hpp>

static bool did_setup = false;

static void DoSetup(const benchmark::State& state) {
  using stan::math::var;
  using stan::math::var_value;
  using stan::math::sum;
  if (did_setup) {
    return;
  }
  did_setup = true;
  // about 10GB
  double* xx = stan::math::ChainableStack::instance_->memalloc_.alloc_array<double>(std::pow(2, 34)/8);
  benchmark::DoNotOptimize(xx);
  benchmark::ClobberMemory();
  stan::math::recover_memory();
}

static void move_ex(benchmark::State& state) {
  using stan::math::var;
  using stan::math::sum;
  Eigen::Matrix<double, -1, -1> x_vals = Eigen::MatrixXd::Random(state.range(0), state.range(0));
  Eigen::Matrix<double, -1, -1> y_vals = Eigen::MatrixXd::Random(state.range(0), state.range(0));
  for (auto _ : state) {
    Eigen::Matrix<var, -1, -1> x = x_vals;
    Eigen::Matrix<var, -1, -1> y = y_vals;
    auto start = std::chrono::high_resolution_clock::now();
    var lp = sum(stan::math::add(stan::math::multiply(stan::math::multiply(x, y), std::move(y)), std::move(x)));
    auto end = std::chrono::high_resolution_clock::now();
    //lp.grad();
    benchmark::DoNotOptimize(lp.vi_);
    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    state.SetIterationTime(elapsed_seconds.count());
    stan::math::recover_memory();
    benchmark::ClobberMemory();
  }

}

int start_val = 2;
int end_val = 4096;
BENCHMARK(move_ex)->RangeMultiplier(2)->Range(start_val, end_val)->UseManualTime()->Setup(DoSetup);
BENCHMARK_MAIN();