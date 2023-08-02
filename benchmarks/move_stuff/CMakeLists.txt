# Now, you can use TBB::tbb as a target in your target_link_libraries:
add_executable(move_ex move_ex.cpp)

FetchContent_Declare(
  stanmathnew
  GIT_REPOSITORY https://github.com/stan-dev/math
  GIT_TAG        feature/reverse-mode-move-semantics # replace with the version you want to use
)

FetchContent_GetProperties(stanmathnew)
if(NOT stanmathnew_POPULATED)
  FetchContent_Populate(stanmathnew)
endif()

target_include_directories(move_ex PRIVATE ${boost_INCLUDE_DIR} ${stanmathnew_SOURCE_DIR})
target_link_libraries(move_ex PRIVATE 
benchmark::benchmark 
TBB::tbb 
Eigen3::Eigen 
sundials_kinsol 
sundials_cvodes)

add_executable(move_ex_orig move_ex.cpp)

target_include_directories(move_ex_orig PRIVATE ${boost_INCLUDE_DIR} ${stanmath_SOURCE_DIR})
target_link_libraries(move_ex_orig PRIVATE 
benchmark::benchmark 
TBB::tbb 
Eigen3::Eigen 
sundials_kinsol 
sundials_cvodes)

