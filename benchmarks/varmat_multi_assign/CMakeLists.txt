# Now, you can use TBB::tbb as a target in your target_link_libraries:
add_executable(varmat_set varmat_set.cpp)
target_include_directories(varmat_set PRIVATE ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stan_SOURCE_DIR}/src)
target_link_libraries(varmat_set PRIVATE 
benchmark::benchmark 
TBB::tbb 
Eigen3::Eigen 
sundials_kinsol 
sundials_cvodes)

add_executable(varmat_map varmat_map.cpp)
target_include_directories(varmat_map PRIVATE ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stan_SOURCE_DIR}/src)
target_link_libraries(varmat_map PRIVATE 
benchmark::benchmark 
TBB::tbb 
Eigen3::Eigen 
sundials_kinsol 
sundials_cvodes)


add_executable(varmat_custom_map varmat_custom_map.cpp)
target_include_directories(varmat_custom_map PRIVATE ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stan_SOURCE_DIR}/src)
target_link_libraries(varmat_custom_map PRIVATE 
benchmark::benchmark 
TBB::tbb 
Eigen3::Eigen 
sundials_kinsol 
sundials_cvodes)


add_executable(matvar_multi matvar.cpp)
target_include_directories(matvar_multi PRIVATE ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR} ${stan_SOURCE_DIR}/src)
target_link_libraries(matvar_multi PRIVATE 
benchmark::benchmark 
TBB::tbb 
Eigen3::Eigen 
sundials_kinsol 
sundials_cvodes)

