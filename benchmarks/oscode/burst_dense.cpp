#include <iostream>
#include <oscode/solver.hpp>
#include <benchmark/benchmark.h>
#include <cmath>
#include <fstream>
#include <string>
#include <stdlib.h>


/** A variable to control the number of oscillations in the solution of the
 * burst equation  */
static constexpr double n = 100.0;

/** Defines the friction term in the ODE to solve
 * @param[in] t Value of the independent variable in the ODE
 * @returns The value of the friction term at \a t
 */
std::complex<double> g(double t) { return 0.0; };

/** Defines the frequency term in the ODE to solve
 * @param[in] t Value of the independent variable in the ODE
 * @returns The value of the frequency term at \a t
 */
std::complex<double> w(double t) {
  return std::pow(n * n - 1.0, 0.5) / (1.0 + t * t);
};

struct WFunc : public BaseInterpolator<double *, std::complex<double> *>{

    static constexpr bool is_interpolated = false;
    double n;
    WFunc(double n) : n(n) {}
    inline std::complex<double> operator()(double t) const {
        return std::pow(n * n - 1.0, 0.5) / (1.0 + t * t);
    }
    bool sign_ = false;
};
struct GFunc : public BaseInterpolator<double *, std::complex<double> *> {
    bool sign_ = false; 
    static constexpr bool is_interpolated = false;
    double n;
    GFunc(double n) : n(n) {}
    inline constexpr std::complex<double> operator()(double /* t */) const {
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

template <typename T>
std::vector<T> linspace(T start, T end, std::size_t points) {
  std::vector<T> res(points);
  float step = (end - start) / (points - 1);
  size_t i = 0;
  for (auto &e : res) {
    e = start + step * i++;
  }
  return res;
}



static void burst(benchmark::State& state) {
  const double n = state.range(0);
  /** Define integration range */
  const double ti = -2 * state.range(0);
  const double tf = 2 * state.range(0);

  /** Define initial conditions */
  std::complex<double> x0 = xburst(ti, n); 
  std::complex<double> dx0 = dxburst(ti, n); 
  std::vector<double> times = linspace(ti, tf, 1000);
  /** Create differential equation "system" */
  /** Method 1: Give frequency and damping term as functions */
  for (auto _ : state) {
    using Sys = de_system<WFunc, GFunc>;
    Sys sys(WFunc{n}, GFunc{n});
    Solution<Sys> solution(sys, x0, dx0, ti, tf, times);
    benchmark::DoNotOptimize(&solution);
    solution.solve();
  }
}

constexpr int start_val = 100;
constexpr int end_val = 40024;
BENCHMARK(burst)->RangeMultiplier(2)->Range(start_val, end_val);
BENCHMARK_MAIN();