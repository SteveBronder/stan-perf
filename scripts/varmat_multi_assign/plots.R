library(data.table)
library(ggplot2)
set_dt = fread("./benchmarks/varmat_multi_assign/varmat_set2.csv", skip = "name,iterations")
map_dt = fread("./benchmarks/varmat_multi_assign/varmat_map2.csv", skip = "name,iterations")
custom_map_dt = fread("./benchmarks/varmat_multi_assign/varmat_custom_map2.csv", skip = "name,iterations")
matvar_dt = fread("./benchmarks/varmat_multi_assign/matvar_multi2.csv", skip = "name,iterations")

toss_unused = \(x) {
    x = x[!grepl("toss", name)]
    x = x[!grepl("time_cv", name)]
    x = x[!grepl("time_mean", name)]
    x = x[!grepl("time_median", name)]
    x = x[!grepl("time_cv", name)]
    x = x[!grepl("time_stddev", name)]
    return(x);
}
set_dt = toss_unused(set_dt)
set_dt[, type := "set"]

map_dt = toss_unused(map_dt)
map_dt[, type := "map"]

custom_map_dt = toss_unused(custom_map_dt)
custom_map_dt[, type := "custom_map"]

matvar_dt = toss_unused(matvar_dt)
matvar_dt[, type := "matvar"]

full_dt = rbind(set_dt, map_dt, custom_map_dt, matvar_dt)
full_dt[, size := as.double(gsub("\\D", "", name))]
full_dt[, idx := seq(1, .N), .(size, type, name)]
quantile_cuts = c(0.25, 0.5, 0.75)
full_sum_dt = full_dt[, .(value = c(quantile(cpu_time, quantile_cuts))), .(size, type)]
q_names = rep(c(paste0("q", quantile_cuts)), full_sum_dt[, .N] / length(quantile_cuts))
full_sum_dt[, qq := q_names]
setkey(full_sum_dt, size, type)
full_sum_cast_dt = dcast(full_sum_dt, size + type ~ qq, value.var = "value")
raw_compare_plot = ggplot(full_sum_cast_dt, aes(x = size, y = q0.5, color = type)) + 
  geom_point() + geom_line() + 
  geom_ribbon(aes(ymin = q0.25, ymax = q0.75, fill = type), alpha = 0.2) +
  ylab("") + xlab("Matrix Size") +
  ggtitle("Vector Multi Index Assignment + fma Time Vs. Size") +
  theme_bw()
raw_compare_plot

ggsave("./benchmarks/varmat_multi_assign/raw_compare_plot2.png", raw_compare_plot, width = 6, height = 4, units = "in")

log_compare_plot = ggplot(full_sum_cast_dt, aes(x = size, y = q0.5, color = type)) + 
  geom_point() + geom_line() + 
  geom_ribbon(aes(ymin = q0.25, ymax = q0.75, fill = type), alpha = 0.2) +
  ylab("") + xlab("Matrix Size") +
  ggtitle("Vector Multi Index Assignment + fma Time Vs. Size") +
  scale_y_log10() +
  theme_bw()

log_compare_plot

ggsave("./benchmarks/varmat_multi_assign/log_compare_plot2.png", log_compare_plot, width = 6, height = 4, units = "in")

full_cast_dt = dcast(full_dt[, .(name, cpu_time, type, size, idx)], name + size + idx ~ type, value.var = "cpu_time")
full_cast_sum_dt = full_cast_dt[, 
.(custom_map_r = mean(custom_map / matvar),
  custom_map_sd = sd(custom_map / matvar),
  map_r = mean(map / matvar),
  map_sd = sd(map / matvar),
  set_r = mean(set / matvar),
  matvar_r = 1), .(size, name)]

full_cast_dt

mat_to_matvar = ggplot(full_cast_sum_dt, aes(x = size)) +
    geom_ribbon(aes(ymin = custom_map_r - custom_map_sd * 2, ymax = custom_map_r + custom_map_sd * 2), alpha = 0.2) +
    geom_line(aes(y = custom_map_r), color = "red") +
    geom_line(aes(y = matvar_r), color = "blue") +
    geom_ribbon(aes(ymin = map_r - map_sd * 2, ymax = map_r + map_sd * 2), alpha = 0.2) +
    geom_line(aes(y = map_r), color = "black") +
    ggtitle("Ratio of Custom Map (red), Map (black), and Matvar (blue)") +
    ylab("") + xlab("Vector Size") + scale_y_continuous(breaks = seq(0, 1.8, 0.1)) +
    theme_bw(base_size = 16)

mat_to_matvar

ggsave("./benchmarks/varmat_multi_assign/map_to_matvar2.png", mat_to_matvar, width = 6, height = 4, units = "in")
