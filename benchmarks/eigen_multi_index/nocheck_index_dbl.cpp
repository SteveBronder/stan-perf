#include <utils/setup.hpp>
#include <stan/model/indexing.hpp>
#include <algorithm>
#include <iterator>
#include <random>
#include <vector>

namespace bench {
template <typename EigVec, typename MultiIndex,
          stan::require_eigen_vector_t<EigVec>* = nullptr,
          stan::require_std_vector_t<MultiIndex>* = nullptr>
inline auto rvalue(EigVec&& v, const char* name, MultiIndex&& idx) {
  return stan::math::make_holder(
      [](auto&& v_ref, auto&& idx_inner) {
        Eigen::Map<const Eigen::Array<int, -1, 1>> idx2(idx_inner.data(),
                                                        idx_inner.size());
        return std::forward<decltype(v_ref)>(v_ref)(idx2 - 1);
      },
      std::forward<EigVec>(v),
      std::forward<MultiIndex>(idx));
}
}

static void index_test(benchmark::State& state) {
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
    stan::model::assign(mu,
      stan::math::add(mu,
        stan::math::add(
          stan::math::elt_multiply(
            bench::rvalue(r_1_1, "r_1_1", J_1), Z_1_1),
          stan::math::elt_multiply(
            bench::rvalue(r_1_2, "r_1_2", J_1), Z_1_2))),
      "assigning variable mu");
    benchmark::DoNotOptimize(mu.data());
    benchmark::ClobberMemory();
  }
}

const int min_N = std::pow(2, 11);
const int max_N = std::pow(2, 17);
constexpr int min_M = 4;
constexpr int max_M = 1024;
BENCHMARK(index_test)->ArgsProduct({
  benchmark::CreateRange(min_N, max_N, 2),
  benchmark::CreateRange(min_M, max_M, 2)
});
BENCHMARK_MAIN();
