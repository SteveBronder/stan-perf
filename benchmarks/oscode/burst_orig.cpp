#include <oscode/solver.hpp>
#include <benchmark/benchmark.h>
#include <cmath>
#include <fstream>
#include <string>
#include <stdlib.h>


/** Defines the friction term in the ODE to solve
 * @param[in] t Value of the independent variable in the ODE
 * @returns The value of the friction term at \a t
 */
inline constexpr std::complex<double> g(double t, void* /* n */) noexcept {
    return 0.0;
};

/** Defines the frequency term in the ODE to solve
 * @param[in] t Value of the independent variable in the ODE
 * @returns The value of the frequency term at \a t
 */
inline std::complex<double> w(double t, void* n_) noexcept {
    int n = *reinterpret_cast<int*>(n_);
    return std::pow(n*n - 1.0,0.5)/(1.0 + t*t);
};

/** Defines the initial value of the solution of the ODE
 * @param[in] t Value of the independent variable in the ODE
 * @returns The value of the solution \a x at \a t
 */
inline std::complex<double> xburst(double t, double n){
    return 100*std::pow(1.0 + t*t,
    0.5)/n*std::complex<double>(std::cos(n*std::atan(t)),std::sin(n*std::atan(t))); 
};

/** Defines the initial value of the derivative of the solution of the ODE
 * @param[in] t Value of the independent variable in the ODE
 * @returns The value of the derivative of solution \a dx/dt at \a t
 */
inline std::complex<double> dxburst(double t, double n){
    return 100/std::pow(1.0 + t*t,
    0.5)/n*(std::complex<double>(t,n)*std::cos(n*std::atan(t)) +
    std::complex<double>(-n,t)*std::sin(n*std::atan(t))); 
};


static void burst(benchmark::State& state) {
  long int n = state.range(0);
  /** Define integration range */
  const double ti = -2 * state.range(0);
  const double tf = 2 * state.range(0);

  /** Define initial conditions */
  std::complex<double> x0 = xburst(ti, n); 
  std::complex<double> dx0 = dxburst(ti, n); 
  
  /** Create differential equation "system" */
  /** Method 1: Give frequency and damping term as functions */
  for (auto _ : state) {
    de_system sys(&w, &g, reinterpret_cast<void*>(&n));
    Solution solution(sys, x0, dx0, ti, tf);
    benchmark::DoNotOptimize(&solution);
    solution.solve();
  }
}

constexpr int start_val = 100;
constexpr int end_val = 40024;
BENCHMARK(burst)->RangeMultiplier(2)->Range(start_val, end_val);
BENCHMARK_MAIN();