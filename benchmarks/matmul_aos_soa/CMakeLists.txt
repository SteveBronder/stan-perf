# Now, you can use TBB::tbb as a target in your target_link_libraries:
add_executable(matmul_soa soa.cpp)

target_include_directories(matmul_soa PRIVATE
  ${stanbench_SOURCE_DIR}/benchmarks
  ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR})
target_link_libraries(matmul_soa PRIVATE
benchmark::benchmark
TBB::tbb
Eigen3::Eigen
sundials_kinsol
sundials_cvodes)

add_executable(matmul_aos aos.cpp)

target_include_directories(matmul_aos PRIVATE
  ${stanbench_SOURCE_DIR}/benchmarks
  ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR})
target_link_libraries(matmul_aos PRIVATE
benchmark::benchmark
TBB::tbb
Eigen3::Eigen
sundials_kinsol
sundials_cvodes)

