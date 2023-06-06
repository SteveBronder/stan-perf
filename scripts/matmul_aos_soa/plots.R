library(data.table)
library(ggplot2)
aos = fread("./benchmarks/matmul_aos_soa/matmul_aos.csv", skip = "name,iterations")
soa = fread("./benchmarks/matmul_aos_soa/matmul_soa.csv", skip = "name,iterations")
aos = aos[!grepl("toss", name)]
aos[, type := "AOS"]
soa = soa[!grepl("toss", name)]
soa[, type := "SOA"]
aos_soa = rbind(aos, soa)
aos_soa[, size := as.double(gsub("\\D", "", name))]
quantile_cuts = c(0.25, 0.5, 0.75)

aos_soa_sum = aos_soa[, .(value = c(quantile(cpu_time, quantile_cuts), mean(cpu_time))), .(size, type)]
aos_soa_sum[, qq := rep(c(paste0("q", quantile_cuts), "mu"), aos_soa_sum[, .N] / 6)]
aos_soa_sum_cast = dcast(aos_soa_sum, size + type ~ qq, value.var = "value")
raw_compare_plot = ggplot(aos_soa_sum_cast, aes(x = size, y = q0.5, color = type)) + 
  geom_point() + geom_line() + 
  geom_ribbon(aes(ymin = q0.25, ymax = q0.75, fill = type), alpha = 0.2) +
  theme_bw()
raw_compare_plot
ggsave("./benchmarks/matmul_aos_soa/plots/raw_compare_plot.png", raw_compare_plot, width = 6, height = 4, units = "in")

log_compare_plot = ggplot(aos_soa_sum_cast, aes(x = size, y = q0.5, color = type)) + 
  geom_point() + geom_line() + 
  geom_ribbon(aes(ymin = q0.25, ymax = q0.75, fill = type), alpha = 0.2) +
  scale_y_log10() +
  theme_bw()

log_compare_plot

ggsave("./benchmarks/matmul_aos_soa/plots/log_compare_plot.png", log_compare_plot, width = 6, height = 4, units = "in")

