library(data.table)
library(ggplot2)
orig_dt = fread("./benchmarks/move_stuff/move_ex_orig.csv", skip = "name,iterations")
pr_dt = fread("./benchmarks/move_stuff/move_ex.csv", skip = "name,iterations")
toss_unused = \(x) {
    x = x[!grepl("toss", name)]
    x = x[!grepl("time_cv", name)]
    x = x[!grepl("time_mean", name)]
    x = x[!grepl("time_median", name)]
    x = x[!grepl("time_cv", name)]
    x = x[!grepl("time_stddev", name)]
    return(x);
}
orig_dt = toss_unused(orig_dt)
orig_dt[, type := "orig"]
pr_dt = toss_unused(pr_dt)
pr_dt[, type := "pr"]
orig_pr_dt = rbind(orig_dt[, .(name, iterations, real_time, cpu_time, time_unit, type)], 
  pr_dt[, .(name, iterations, real_time, cpu_time, time_unit, type)])
orig_pr_dt[, size := as.double(gsub("\\D", "", name))]
orig_pr_dt[, idx := seq(1, .N), .(size, type, name)]

quantile_cuts = c(0.25, 0.5, 0.75)
orig_pr_sum = orig_pr_dt[, .(value = c(quantile(cpu_time, quantile_cuts), mean(cpu_time))), .(size, type)]
orig_pr_sum[, qq := rep(c(paste0("q", quantile_cuts), "mu"), orig_pr_sum[, .N] / (length(quantile_cuts) + 1))]
orig_pr_sum_cast = dcast(orig_pr_sum, size + type ~ qq, value.var = "value")
raw_compare_plot = ggplot(orig_pr_sum_cast, aes(x = size, y = q0.5, color = type)) + 
  geom_point() + geom_line() + 
  geom_ribbon(aes(ymin = q0.25, ymax = q0.75, fill = type), alpha = 0.2) +
  ylab("") + xlab("Matrix Size") +
  ggtitle("Matrix Multiplication Time Vs. Size (orig vs pr)") +
  theme_bw()
raw_compare_plot

ggsave("./benchmarks/matmul_orig_pr/plots/raw_compare_plot.png", raw_compare_plot, width = 6, height = 4, units = "in")

log_compare_plot = ggplot(orig_pr_sum_cast, aes(x = size, y = q0.5, color = type)) + 
  geom_point() + geom_line() + 
  geom_ribbon(aes(ymin = q0.25, ymax = q0.75, fill = type), alpha = 0.2) +
  ylab("") + xlab("Matrix Size") +
  ggtitle("Matrix Multiplication Time Vs. Size on Log Scale (orig vs pr)") +
  scale_y_log10() +
  theme_bw()

log_compare_plot

ggsave("./benchmarks/matmul_orig_pr/plots/log_compare_plot.png", log_compare_plot, width = 6, height = 4, units = "in")

full_cast_dt = dcast(orig_pr_dt[, .(name, cpu_time, type, size, idx)], name + size + idx ~ type, value.var = "cpu_time")
full_cast_sum_dt = full_cast_dt[, 
.(move_r = median(orig / pr) - 1,
  move_sd = sd(orig / pr)), .(size, name)]
full_cast_dt
mat_to_orig = ggplot(full_cast_sum_dt, aes(x = size)) +
    geom_hline(yintercept = 0, color = "blue") +
    geom_ribbon(aes(ymin = move_r - move_sd * 2, ymax = move_r + move_sd * 2), alpha = 0.2) +
    geom_line(aes(y = move_r), color = "black") +
    ggtitle("% speedup of using move semantics to not using move semantics") +
    ylab("") + xlab("Matrix Size") + scale_x_log10(breaks=2^(1:12)) +
    scale_y_continuous(breaks=scales::extended_breaks(n=10)) +
    theme_bw(base_size = 12)

mat_to_orig

ggsave("./benchmarks/move_stuff/plots/compare_plot.png", mat_to_orig, width = 6, height = 4, units = "in")
