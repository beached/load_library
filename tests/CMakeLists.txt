cmake_policy(SET CMP0065 NEW)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt CONFIG REQUIRED)

add_library(load-library-test-deps INTERFACE)
target_link_libraries(load-library-test-deps INTERFACE daw::daw-header-libraries daw::daw-load-library fmt::fmt ${COMPILER_SPECIFIC_LIBS})

target_include_directories(load-library-test-deps INTERFACE include/)
target_compile_features(load-library-test-deps INTERFACE cxx_std_17)

add_custom_target(${PROJECT_NAME}_full)

add_library(test_library SHARED src/libtest.cpp)

add_executable(load_library_test_bin src/load_library_test.cpp)
target_link_libraries(load_library_test_bin load-library-test-deps)
add_test(NAME load_library_test COMMAND load_library_test_bin )
add_dependencies(${PROJECT_NAME}_full load_library_test_bin)
add_dependencies(load_library_test_bin test_library)


