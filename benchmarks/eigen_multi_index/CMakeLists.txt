file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)


FetchContent_Declare(
  stanidx
  GIT_REPOSITORY https://github.com/stan-dev/stan
  GIT_TAG        feature/eigen-34-multi-index # replace with the version you want to use
)

FetchContent_GetProperties(stanidx)
if(NOT stanidx_POPULATED)
  FetchContent_Populate(stanidx)
endif()

# double impl
add_executable(eigen_dbl eigen_dbl.cpp)

target_include_directories(eigen_dbl PRIVATE
  ${stanbench_SOURCE_DIR}/benchmarks
  ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stan_SOURCE_DIR}/src)
target_link_libraries(eigen_dbl PRIVATE
  benchmark::benchmark
  TBB::tbb
  Eigen3::Eigen
  sundials_kinsol
  sundials_cvodes)


add_executable(nocheck_index_dbl nocheck_index_dbl.cpp)

target_include_directories(nocheck_index_dbl PRIVATE
  ${stanbench_SOURCE_DIR}/benchmarks
  ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stan_SOURCE_DIR}/src)
target_link_libraries(nocheck_index_dbl PRIVATE
  benchmark::benchmark
  TBB::tbb
  Eigen3::Eigen
  sundials_kinsol
  sundials_cvodes)


add_executable(orig_index_dbl index_dbl.cpp)

target_include_directories(orig_index_dbl PRIVATE
  ${stanbench_SOURCE_DIR}/benchmarks
  ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stan_SOURCE_DIR}/src)
target_link_libraries(orig_index_dbl PRIVATE
  benchmark::benchmark
  TBB::tbb
  Eigen3::Eigen
  sundials_kinsol
  sundials_cvodes)

add_executable(new_index_dbl index_dbl.cpp)

target_include_directories(new_index_dbl PRIVATE
  ${stanbench_SOURCE_DIR}/benchmarks
  ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stanidx_SOURCE_DIR}/src)

target_link_libraries(new_index_dbl PRIVATE
  benchmark::benchmark
  TBB::tbb
  Eigen3::Eigen
  sundials_kinsol
  sundials_cvodes)

add_executable(loop_index_dbl loop_dbl.cpp)

target_include_directories(loop_index_dbl PRIVATE
  ${stanbench_SOURCE_DIR}/benchmarks
  ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stan_SOURCE_DIR}/src)

target_link_libraries(loop_index_dbl PRIVATE
  benchmark::benchmark
  TBB::tbb
  Eigen3::Eigen
  sundials_kinsol
  sundials_cvodes)


# var impl
add_executable(orig_index_var index_var.cpp)

target_include_directories(orig_index_var PRIVATE
  ${stanbench_SOURCE_DIR}/benchmarks
  ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stan_SOURCE_DIR}/src)

target_link_libraries(orig_index_var PRIVATE
  benchmark::benchmark
  TBB::tbb
  Eigen3::Eigen
  sundials_kinsol
  sundials_cvodes)


add_executable(new_index_var index_var.cpp)

target_include_directories(new_index_var PRIVATE
    ${stanbench_SOURCE_DIR}/benchmarks
    ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stanidx_SOURCE_DIR}/src)

target_link_libraries(new_index_var PRIVATE
    benchmark::benchmark
    TBB::tbb
    Eigen3::Eigen
    sundials_kinsol
    sundials_cvodes)



add_executable(loop_index_var loop_var.cpp)

target_include_directories(loop_index_var PRIVATE
  ${stanbench_SOURCE_DIR}/benchmarks
  ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stan_SOURCE_DIR}/src)

target_link_libraries(loop_index_var PRIVATE
  benchmark::benchmark
  TBB::tbb
  Eigen3::Eigen
  sundials_kinsol
  sundials_cvodes)
