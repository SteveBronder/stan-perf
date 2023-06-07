
```
make -j3 varmat_set varmat_map varmat_custom_map matvar_multi
taskset -c 11 ./benchmarks/varmat_multi_assign/varmat_set --benchmark_out_format=csv --benchmark_out=./benchmarks/varmat_multi_assign/varmat_set.csv --benchmark_report_aggregates_only=false --benchmark_repetitions=30
taskset -c 11 ./benchmarks/varmat_multi_assign/varmat_map --benchmark_out_format=csv --benchmark_out=./benchmarks/varmat_multi_assign/varmat_map.csv  --benchmark_report_aggregates_only=false --benchmark_repetitions=30
taskset -c 11 ./benchmarks/varmat_multi_assign/varmat_custom_map --benchmark_out_format=csv --benchmark_out=./benchmarks/varmat_multi_assign/varmat_custom_map.csv  --benchmark_report_aggregates_only=false --benchmark_repetitions=30
taskset -c 11 ./benchmarks/varmat_multi_assign/matvar_multi --benchmark_out_format=csv --benchmark_out=./benchmarks/varmat_multi_assign/matvar_multi.csv  --benchmark_report_aggregates_only=false --benchmark_repetitions=30

```