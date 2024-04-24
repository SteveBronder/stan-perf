#include <utils/setup.hpp>
#include <stan/model/indexing.hpp>
#include <algorithm>
#include <iterator>
#include <random>
#include <vector>

static void loop_test(benchmark::State& state) {
  using stan::math::sum;
  Eigen::Matrix<double, -1, 1> r_1_1 =
    Eigen::VectorXd::Random(state.range(1));
  Eigen::Matrix<double, -1, 1> r_1_2 =
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
    Eigen::Matrix<double, -1, 1> mu = Eigen::VectorXd::Zero(state.range(0));
    const auto N_iter = state.range(0);
    for (int n = 1; n <= N_iter; ++n) {
      stan::model::assign(mu,
        (stan::model::rvalue(mu, "mu", stan::model::index_uni(n)) +
        ((stan::model::rvalue(r_1_1, "r_1_1",
            stan::model::index_uni(
              stan::model::rvalue(J_1, "J_1", stan::model::index_uni(n))))
        *
        stan::model::rvalue(Z_1_1, "Z_1_1", stan::model::index_uni(n)))
        +
        (stan::model::rvalue(r_1_2, "r_1_2",
            stan::model::index_uni(
              stan::model::rvalue(J_1, "J_1", stan::model::index_uni(n))))
        *
        stan::model::rvalue(Z_1_2, "Z_1_2", stan::model::index_uni(n))))),
        "assigning variable mu", stan::model::index_uni(n));
    }
    benchmark::DoNotOptimize(mu.data());
    benchmark::ClobberMemory();
  }
}

const int min_N = std::pow(2, 11);
const int max_N = std::pow(2, 17);
constexpr int min_M = 4;
constexpr int max_M = 1024;
BENCHMARK(loop_test)->ArgsProduct({
  benchmark::CreateRange(min_N, max_N, 2),
  benchmark::CreateRange(min_M, max_M, 2)
});
BENCHMARK_MAIN();
