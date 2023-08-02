```
make -j4 burst burst_orig
taskset -c 11 ./benchmarks/oscode/burst --benchmark_out_format=csv --benchmark_out=./benchmarks/oscode/burst.csv  --benchmark_report_aggregates_only=false --benchmark_repetitions=30
taskset -c 11 ./benchmarks/oscode/burst_orig --benchmark_out_format=csv --benchmark_out=./benchmarks/oscode/burst_orig.csv --benchmark_report_aggregates_only=false --benchmark_repetitions=30
taskset -c 11 ./benchmarks/oscode/burst && taskset -c 11 ./benchmarks/oscode/burst_orig
```