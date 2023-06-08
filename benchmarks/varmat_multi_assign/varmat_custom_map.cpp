#include <benchmark/benchmark.h>
#include <stan/math/mix.hpp>
#include <stan/model/indexing/assign_varmat.hpp>
#include <varmat_multi_assign/setup.hpp>
#include <varmat_multi_assign/hashmap.hpp>



struct IdentityHash {
    template <typename K>
    static inline size_t hash(const K &key) noexcept {
        return static_cast<size_t>(key);
    }
};

template <typename Vec1, typename Vec2>
inline void new_assign(Vec1&& x, const Vec2& y, const char* name,
                   const stan::model::index_multi& idx) {
  stan::math::check_size_match("vector[multi] assign", name, idx.ns_.size(),
                               "right hand side", y.size());
  const auto x_size = x.size();
  const auto assign_size = idx.ns_.size();
  stan::arena_t<Eigen::Matrix<double, -1, 1>> prev_vals(assign_size);
  Eigen::Matrix<double, -1, 1> y_idx_vals(assign_size);
  // Use boost unordered flat map when boost 1.82 goes into math
  rigtorp::HashMap<int, int, std::hash<int>, std::equal_to<>,
                       stan::math::arena_allocator<std::pair<int, int>>>
      x_map(idx.ns_.size(), std::numeric_limits<int>::max());
  x_map.reserve(assign_size);
  // Keep track of the last place we assigned to.
  for (int i = 0; i < assign_size; ++i) {
    x_map[idx.ns_[i] - 1] = i;
  }
  const auto& y_val = stan::math::value_of(y);
  // We have to use two loops to avoid aliasing issues.
  for (auto&& x_idx : x_map) {
    stan::math::check_range("vector[multi] assign", name, x_size,
                            x_idx.first + 1);
    prev_vals.coeffRef(x_idx.second) = x.vi_->val_.coeffRef(x_idx.first);
    y_idx_vals.coeffRef(x_idx.second) = y_val.coeff(x_idx.second);
  }
  for (auto&& x_idx : x_map) {
    x.vi_->val_.coeffRef(x_idx.first) = y_idx_vals.coeff(x_idx.second);
  }

  if (!stan::is_constant<Vec2>::value) {
    stan::math::reverse_pass_callback([x, y, x_map, prev_vals]() mutable {
      for (auto&& x_idx : x_map) {
        x.vi_->val_.coeffRef(x_idx.first) = prev_vals.coeffRef(x_idx.second);
        prev_vals.coeffRef(x_idx.second) = x.adj().coeffRef(x_idx.first);
        x.adj().coeffRef(x_idx.first) = 0.0;
      }
      for (auto&& x_idx : x_map) {
        stan::math::forward_as<stan::math::promote_scalar_t<stan::math::var, Vec2>>(y)
            .adj()
            .coeffRef(x_idx.second)
            += prev_vals.coeff(x_idx.second);
      }
    });
  } else {
    stan::math::reverse_pass_callback([x, x_map, prev_vals]() mutable {
      for (auto&& x_idx : x_map) {
        x.vi_->val_.coeffRef(x_idx.first) = prev_vals.coeff(x_idx.second);
        prev_vals.coeffRef(x_idx.second) = x.adj().coeff(x_idx.first);
        x.adj().coeffRef(x_idx.first) = 0.0;
      }
    });
  }
}


static void multi_assign(benchmark::State& state) {
  using stan::math::var;
  using stan::math::var_value;
  using stan::math::sum;
  Eigen::Matrix<double, -1, 1> x_vals = Eigen::Matrix<double, -1, 1>::Random(state.range(0), state.range(0));
  Eigen::Matrix<double, -1, 1> y_vals = Eigen::Matrix<double, -1, 1>::Random(state.range(0) / 2, state.range(0));
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
    new_assign(x, y, "", idxs);
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
BENCHMARK(toss_me);
BENCHMARK(multi_assign)->RangeMultiplier(2)->Range(start_val, end_val)->UseManualTime();
BENCHMARK_MAIN();