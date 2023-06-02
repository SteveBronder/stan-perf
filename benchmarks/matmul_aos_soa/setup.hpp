#include <benchmark/benchmark.h>
#include <stan/math/mix.hpp>
#include <Eigen/Dense>
#include <utility>

// Just to fill up the stack allocator
static void toss_me(benchmark::State& state) {
  using stan::math::var;
  Eigen::Matrix<double, -1, -1> x_vals = Eigen::MatrixXd::Random(1300, 1300);
  Eigen::Matrix<double, -1, -1> y_vals = Eigen::MatrixXd::Random(1300, 1300);
  using stan::math::var_value;
  using stan::math::var;
  using stan::math::sum;
  Eigen::Matrix<var, -1, -1> x = x_vals;
  Eigen::Matrix<var, -1, -1> y = y_vals;
  var lp = 0;
  lp -= sum(x * y + x);
  benchmark::DoNotOptimize(lp.vi_);
  for (auto _ : state) {
    lp.grad();
    benchmark::ClobberMemory();
    stan::math::set_zero_all_adjoints();
  }
  stan::math::recover_memory();
}
