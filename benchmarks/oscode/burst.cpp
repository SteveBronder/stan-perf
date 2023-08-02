#include <iostream>
#include <oscode/solver.hpp>
#include <benchmark/benchmark.h>
#include <cmath>
#include <fstream>
#include <string>
#include <stdlib.h>


/** A variable to control the number of oscillations in the solution of the
 * burst equation  */



struct WFunc : public BaseInterpolator<double *, std::complex<double> *> {

    static constexpr bool is_interpolated = false;
    double n;
    WFunc(double naa) : n(naa) {}
    inline std::complex<double> operator()(double t) const noexcept {
        return std::pow(this->n * this->n - 1.0, 0.5) / (1.0 + t * t);
    }
    bool sign_ = false;
};
struct GFunc : public BaseInterpolator<double *, std::complex<double> *> {
    bool sign_ = false; 
    static constexpr bool is_interpolated = false;
    inline constexpr std::complex<double> operator()(double /* t */) const noexcept {
        return 0.0;
    }
    
};
/** Defines the initial value of the solution of the ODE
 * @param[in] t Value of the independent variable in the ODE
 * @returns The value of the solution \a x at \a t
 */
std::complex<double> xburst(double t, double n) {
  return 100 * std::pow(1.0 + t * t, 0.5) / n *
         std::complex<double>(std::cos(n * std::atan(t)),
                              std::sin(n * std::atan(t)));
};

/** Defines the initial value of the derivative of the solution of the ODE
 * @param[in] t Value of the independent variable in the ODE
 * @returns The value of the derivative of solution \a dx/dt at \a t
 */
std::complex<double> dxburst(double t, double n) {
  return 100 / std::pow(1.0 + t * t, 0.5) / n *
         (std::complex<double>(t, n) * std::cos(n * std::atan(t)) +
          std::complex<double>(-n, t) * std::sin(n * std::atan(t)));
};



static void burst(benchmark::State& state) {
  const double n = state.range(0);
  /** Define integration range */
  const double ti = -2 * state.range(0);
  const double tf = 2 * state.range(0);

  /** Define initial conditions */
  std::complex<double> x0 = xburst(ti, n); 
  std::complex<double> dx0 = dxburst(ti, n); 
  /** Create differential equation "system" */
  /** Method 1: Give frequency and damping term as functions */
  for (auto _ : state) {
    using Sys = de_system<WFunc, GFunc>;
    Sys sys(WFunc{n}, GFunc{});
    Solution<Sys> solution(sys, x0, dx0, ti, tf);
    benchmark::DoNotOptimize(&solution);
    solution.solve();
    std::cout << "t: " << solution.ssteps << " Wkb: " << solution.wkbsteps <<  " rk: " << 
      solution.ssteps - solution.wkbsteps << std::endl;
  }
}

constexpr int start_val = 100;
constexpr int end_val = 40024;
BENCHMARK(burst)->RangeMultiplier(2)->Range(start_val, end_val);
BENCHMARK_MAIN();