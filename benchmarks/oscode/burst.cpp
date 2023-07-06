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
inline constexpr std::complex<double> g(double t) noexcept {
    return 0.0;
};

/** Defines the frequency term in the ODE to solve
 * @param[in] t Value of the independent variable in the ODE
 * @returns The value of the frequency term at \a t
 */
inline std::complex<double> w(double t, double n) noexcept {
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

struct WFunctor : public BaseInterpolator<double *, std::complex<double> *, double *> {
  double n;
  bool sign_{false};
  WFunctor(double n_) : n(n_) {} 

  inline std::complex<double> operator()(double t) const noexcept {
    return 100.0*std::pow(1.0 + t*t,
    0.5)/n*std::complex<double>(std::cos(n*std::atan(t)),std::sin(n*std::atan(t))); 
  }
};

struct GFunctor : public BaseInterpolator<double *, std::complex<double> *, double *> {
  double n;
  bool sign_{false};
  GFunctor(double n_) : n(n_) {} 
  inline std::complex<double> operator()(double t) const noexcept {
    return 100.0/std::pow(1.0 + t*t,
    0.5)/n*(std::complex<double>(t,n)*std::cos(n*std::atan(t)) +
    std::complex<double>(-n,t)*std::sin(n*std::atan(t))); 
  }
};


static void burst(benchmark::State& state) {
  double n = state.range(0);
  /** Define integration range */
  const double ti = -2;
  const double tf = 2;

  /** Define initial conditions */
  std::complex<double> x0 = xburst(ti, n); 
  std::complex<double> dx0 = dxburst(ti, n); 

  /** Create differential equation "system" */
  /** Method 1: Give frequency and damping term as functions */
  for (auto _ : state) {
    de_system<WFunctor, GFunctor> sys(WFunctor{n}, GFunctor{n});
    Solution<de_system<WFunctor, GFunctor>> solution(sys, x0, dx0, ti, tf);
    benchmark::DoNotOptimize(&solution);
    solution.solve(1e-4, 0.0f);
  }
}

constexpr int start_val = 100;
constexpr int end_val = 40024;
BENCHMARK(burst)->RangeMultiplier(2)->Range(start_val, end_val);
BENCHMARK_MAIN();