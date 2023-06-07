#include <benchmark/benchmark.h>
#include <stan/math/mix.hpp>
#include <Eigen/Dense>
#include <utility>

// Just to fill up the stack allocator
static void toss_me(benchmark::State& state) {
  using stan::math::var;
  using stan::math::var_value;
  using stan::math::sum;
  Eigen::Matrix<double, -1, -1> x_vals = Eigen::MatrixXd::Random(1300, 1300);
  Eigen::Matrix<double, -1, -1> y_vals = Eigen::MatrixXd::Random(1300, 1300);
  Eigen::Matrix<var, -1, -1> x = x_vals;
  Eigen::Matrix<var, -1, -1> y = y_vals;
  var lp = 0;
  lp -= sum(x * y + x);
  benchmark::DoNotOptimize(lp.vi_);
  for (auto _ : state) {
    auto x = 1 + 1;
    benchmark::ClobberMemory();
  }
  stan::math::recover_memory();
}
