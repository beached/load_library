# Copyright (c) Darrell Wright
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/beached/gumbo_pp
#

cmake_minimum_required(VERSION 3.20)

project("daw-load-library"
        VERSION "0.0.1"
        HOMEPAGE_URL "https://github.com/beached/load_library"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested.")

option(DAW_USE_PACKAGE_MANAGEMENT "Do not use FetchPackage for dependencies" OFF)
option(DAW_ENABLE_TESTING "Build tests and examples" OFF)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

if (NOT DAW_USE_PACKAGE_MANAGEMENT)
    add_subdirectory(extern)
else ()
    find_package(daw-header-libraries REQUIRED)
endif ()

find_package(fmt CONFIG REQUIRED)

include(GNUInstallDirs)

if (WIN32)
    set(DAW_LIBRARY_DEFINITION_FILE src/load_library_windows.cpp)
else ()
    set(DAW_LIBRARY_DEFINITION_FILE src/load_library_posix.cpp)
endif ()

add_library(${PROJECT_NAME}
        src/load_library.cpp
        ${DAW_LIBRARY_DEFINITION_FILE})
add_library(daw::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if (NOT WIN32)
    set(DAW_EXTRA_LIBS dl)
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE include/)
target_link_libraries(${PROJECT_NAME} PUBLIC daw::daw-header-libraries fmt::fmt ${DAW_EXTRA_LIBS})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME}
        INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        )

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

include(CMakePackageConfigHelpers)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE daw::
        DESTINATION lib/cmake/${PROJECT_NAME}
        )

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake "
        DESTINATION lib/cmake/${PROJECT_NAME}
        )

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include/)

if (DAW_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif ()

