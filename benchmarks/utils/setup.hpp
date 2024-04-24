#ifndef STANPERF_BENCHMARKS_UTILS_SETUP_HPP
#define STANPERF_BENCHMARKS_UTILS_SETUP_HPP

#include <benchmark/benchmark.h>
#include <stan/math/mix.hpp>
#include <Eigen/Dense>
#include <utility>


static bool done_once{false};

/**
 * Fixture for setting up the arena allocator for the benchmarks.
 * This will only set up the allocator once for the entire benchmark run.
 */
class ArenaAlloc : public benchmark::Fixture {
public:
  void SetUp(::benchmark::State& state) {
    if (done_once) {
      return;
    } else {
      done_once = true;
    }
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
    lp.grad();
    benchmark::ClobberMemory();
    stan::math::recover_memory();
  }

  void TearDown(::benchmark::State& state) {
  }
};

#endif
