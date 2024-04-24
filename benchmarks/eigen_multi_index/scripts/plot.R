library(data.table)
library(ggplot2)
orig_dt  = fread("../../../open_source/stan/stevebronder/stan-perf/build/benchmarks/eigen_multi_index/orig_index.csv", skip = 10)
new_dt  = fread("../../../open_source/stan/stevebronder/stan-perf/build/benchmarks/eigen_multi_index/new_index.csv", skip = 10)
loop_dt  = fread("../../../open_source/stan/stevebronder/stan-perf/build/benchmarks/eigen_multi_index/loop_index.csv", skip = 10)

orig_dt[, bench := "orig"]
new_dt[, bench := "new"]
loop_dt[, bench := "loop"]

extract_number <- function(input_string, num) {
  # Use a regular expression to find numbers in the string
  numbers <- regmatches(input_string, gregexpr("\\d+", input_string))
  numbers = lapply(numbers, as.numeric)
  # Check if we have any numbers and return the first one if available
  return(unlist(lapply(numbers, \(x) x[num])))
}

full_dt = rbindlist(list(orig_dt, new_dt, loop_dt))
full_dt[, `:=`(N = extract_number(name, 1), M = extract_number(name, 2))]
full_dt[grepl("mean", name), stat_type := "mean"]
full_dt[grepl("stddev", name), stat_type := "stddev"]
mean_var_dt = full_dt[!is.na(stat_type),]
mean_var_dt[bench == "loop" & M == 4 & N == 1024]
mean_var_dt = dcast(mean_var_dt[, .(real_time, M, N, bench, stat_type)],
  bench+M+N~stat_type, value.var = "real_time")
var_bench = ggplot(mean_var_dt[M == 256], aes(x = N, y = mean, group = bench, color = bench)) +
  geom_ribbon(aes(ymin = mean - 2 * stddev, ymax = mean + 2 * stddev)) +
  geom_line() +
  scale_x_log10() +
  scale_y_log10() +
  xlab("") +
  ylab("") +
  ggtitle("var Average Time for multi index elt_multiply") +
  theme_bw()
var_bench

ggsave(filename = "../../stan/stevebronder/stan-perf/benchmarks/eigen_multi_index/output/varplot.png", plot = var_bench)

