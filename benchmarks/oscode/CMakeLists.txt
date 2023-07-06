FetchContent_Declare(
  oscode_new
  GIT_REPOSITORY https://github.com/SteveBronder/oscode
  GIT_TAG        fixes/cleanup # replace with the version you want to use
)

FetchContent_GetProperties(oscode_new)
if(NOT oscode_new_POPULATED)
  FetchContent_Populate(oscode_new)
endif()


add_executable(burst burst.cpp)
target_include_directories(burst PRIVATE ${oscode_new_SOURCE_DIR}/include)
target_link_libraries(burst PRIVATE Eigen3::Eigen benchmark::benchmark)

FetchContent_Declare(
  oscode_orig
  GIT_REPOSITORY https://github.com/fruzsinaagocs/oscode
  GIT_TAG        master # replace with the version you want to use
)

FetchContent_GetProperties(oscode_orig)
if(NOT oscode_orig_POPULATED)
  FetchContent_Populate(oscode_orig)
endif()


add_executable(burst_orig burst_orig.cpp)
target_include_directories(burst_orig PRIVATE ${oscode_orig_SOURCE_DIR}/include)
target_link_libraries(burst_orig PRIVATE Eigen3::Eigen benchmark::benchmark)
