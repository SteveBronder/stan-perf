# Run Benchmark

```bash
cd ./benchmarks/build/eigen_multi_index
make -j24 orig_index_dbl new_index_dbl loop_index_dbl \
 orig_index_var new_index_var loop_index_var eigen_dbl

# dbl
./eigen_dbl --benchmark_repetitions=30 \
 --benchmark_display_aggregates_only=true \
 --benchmark_out_format=csv \
 --benchmark_counters_tabular=true \
 --benchmark_out=./output/eigen_index_dbl.csv

 ./nocheck_index_dbl --benchmark_repetitions=30 \
 --benchmark_display_aggregates_only=true \
 --benchmark_out_format=csv \
 --benchmark_counters_tabular=true \
 --benchmark_out=./output/nocheck_index_dbl.csv

./orig_index_dbl --benchmark_repetitions=30 \
 --benchmark_display_aggregates_only=true \
 --benchmark_out_format=csv \
 --benchmark_counters_tabular=true \
 --benchmark_out=./output/orig_index_dbl.csv

 ./new_index_dbl --benchmark_repetitions=30 \
 --benchmark_display_aggregates_only=true \
 --benchmark_out_format=csv \
 --benchmark_counters_tabular=true \
 --benchmark_out=./output/new_index_dbl.csv

 ./loop_index_dbl --benchmark_repetitions=30 \
 --benchmark_display_aggregates_only=true \
 --benchmark_out_format=csv \
 --benchmark_counters_tabular=true \
 --benchmark_out=./output/loop_index_dbl.csv



# var
./orig_index_var --benchmark_repetitions=30 \
 --benchmark_display_aggregates_only=true \
 --benchmark_out_format=csv \
 --benchmark_counters_tabular=true \
 --benchmark_out=./output/orig_index_var.csv

 ./new_index_var --benchmark_repetitions=30 \
 --benchmark_display_aggregates_only=true \
 --benchmark_out_format=csv \
 --benchmark_counters_tabular=true \
 --benchmark_out=./output/new_index_var.csv

 ./loop_index_var --benchmark_repetitions=30 \
 --benchmark_display_aggregates_only=true \
 --benchmark_out_format=csv \
 --benchmark_counters_tabular=true \
 --benchmark_out=./output/loop_index_var.csv



```
