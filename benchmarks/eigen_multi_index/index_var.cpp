#include <utils/setup.hpp>
#include <stan/model/indexing.hpp>
#include <algorithm>
#include <iterator>
#include <random>
#include <vector>

BENCHMARK_DEFINE_F(ArenaAlloc, index_test)(benchmark::State& state) {
  using stan::math::var;
  using stan::math::sum;
  Eigen::Matrix<double, -1, 1> r_1_1_vals =
    Eigen::VectorXd::Random(state.range(1));
  Eigen::Matrix<double, -1, 1> r_1_2_vals =
    Eigen::VectorXd::Random(state.range(1));
  Eigen::Matrix<double, -1, 1> Z_1_1 = Eigen::VectorXd::Random(state.range(0));
  Eigen::Matrix<double, -1, 1> Z_1_2 = Eigen::VectorXd::Random(state.range(0));
  std::vector<int> J_1;
  int N_per_group = state.range(0) / state.range(1);
  J_1.reserve(state.range(0));
  for (int group_id = 0; group_id < state.range(1); ++group_id) {
    for (int n = 0; n < N_per_group; ++n) {
      J_1.push_back(group_id + 1);
    }
  }
  std::mt19937 g(1234);
  std::shuffle(J_1.begin(), J_1.end(), g);
  for (auto _ : state) {
    var lp = 0;
    Eigen::Matrix<var, -1, 1> mu = Eigen::VectorXd::Zero(state.range(0));
    Eigen::Matrix<var, -1, 1> r_1_1 = r_1_1_vals;
    Eigen::Matrix<var, -1, 1> r_1_2 = r_1_2_vals;
    auto start = std::chrono::high_resolution_clock::now();
    stan::model::assign(mu,
      stan::math::add(stan::model::deep_copy(mu),
        stan::math::add(
          stan::math::elt_multiply(
            stan::model::rvalue(r_1_1, "r_1_1",
              stan::model::index_multi(J_1)), Z_1_1),
          stan::math::elt_multiply(
            stan::model::rvalue(r_1_2, "r_1_2",
              stan::model::index_multi(J_1)), Z_1_2))),
      "assigning variable mu");
    lp += sum(mu);
    lp.grad();
    benchmark::DoNotOptimize(lp.vi_);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    state.SetIterationTime(elapsed_seconds.count());
    state.counters.insert({{"N", state.range(0)}, {"M", state.range(1)}});
    stan::math::recover_memory();
    benchmark::ClobberMemory();
  }
  stan::math::recover_memory();
}

const int min_N = std::pow(2, 11);
const int max_N = std::pow(2, 17);
constexpr int min_M = 4;
constexpr int max_M = 1024;
BENCHMARK_REGISTER_F(ArenaAlloc, index_test)->ArgsProduct({
  benchmark::CreateRange(min_N, max_N, 2),
  benchmark::CreateRange(min_M, max_M, 2)
})->UseManualTime();
BENCHMARK_MAIN();
