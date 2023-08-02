

```
make -j4 move_ex move_ex_orig
taskset -c 11 ./benchmarks/move_stuff/move_ex --benchmark_out_format=csv --benchmark_out=./benchmarks/move_stuff/move_ex.csv --benchmark_report_aggregates_only=false --benchmark_repetitions=30 --benchmark_report_aggregates_only=false --benchmark_display_aggregates_only=true --benchmark_repetitions=30 --benchmark_enable_random_interleaving=true
taskset -c 11 ./benchmarks/move_stuff/move_ex_orig --benchmark_out_format=csv --benchmark_out=./benchmarks/move_stuff/move_ex_orig.csv  --benchmark_report_aggregates_only=false --benchmark_display_aggregates_only=true --benchmark_repetitions=30 --benchmark_enable_random_interleaving=true


```