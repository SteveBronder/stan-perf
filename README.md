# stan-perf, An Example Repo For Reproducible Benchmarks
Performance Testing Suite for the Stan C++ libraries

## Steps to Run matmul for Struct of Arrays and Array of Structs Examples

1. Build the project in a build directory

```bash
cmake -S . -B "build" -DCMAKE_BUILD_TYPE=Release
cd ./build
# After you are in the build file you can call `cmake ..` to re-run cmake
```

2. Compile the google bench binaries

```bash
make matmul_soa matmul_aos
```

3. Use the script in the folders `readme.md` to run each test saving their results to local folder. Here we use `taskset -c 0` to pin the benchmark to the first cpu.

```bash
taskset -c 11 ./benchmarks/matmul_aos_soa/matmul_soa --benchmark_out_format=csv --benchmark_out=./benchmarks/matmul_aos_soa/matmul_soa.csv --benchmark_repetitions=30 --benchmark_report_aggregates_only=false
taskset -c 11 ./benchmarks/matmul_aos_soa/matmul_aos --benchmark_out_format=csv --benchmark_out=./benchmarks/matmul_aos_soa/matmul_aos.csv --benchmark_repetitions=30 --benchmark_report_aggregates_only=false
```

4. Run the script to plot the results.

```bash
Rscript ./scripts/matmul_aos_soa/plot.R
```

![plot](./scripts/matmul_aos_soa/raw_compare_plot.png)
![plot](./scripts/matmul_aos_soa/log_compare_plot.png)


## Steps to Add a New Example

1. Make a new folder in `benchmarks`

```bash
mkdir ./benchmarks/my_new_bench
```

2. Write your test using [google benchmark](https://github.com/google/benchmark#usage)

```C++
// Example benchmark
#include <benchmark/benchmark.h>

static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state)
    std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
  std::string x = "hello";
  for (auto _ : state)
    std::string copy(x);
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();
```

3. Make a `CMakeLists.txt` file in your benchmark folder to compile your test

```cmake
add_executable(matmul_aos aos.cpp)
```

4. In the `benchmarks` folder, add your subdirectory to the `CMakeLists.txt`

```cmake
add_subdirectory(my_new_bench)
```

5. From the top level directory, either call `cmake -S . -B "build" -DCMAKE_BUILD_TYPE=Release` or `mkdir build; cd build; cmake .. -DCMAKE_BUILD_TYPE=Release`

6. `cd` into build and call `make my_bench_name`

7. Run your benchmark, storing any scripts to generate plots in a `scripts/my_new_bench` folder

### Cmake Particulars

This repo comes with `tbb`, `Eigen`, `sundials`, `boost` headers, `stan`, and `stan math` which you can find in the top level `CMakeLists.txt`.

