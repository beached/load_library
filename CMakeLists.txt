cmake_minimum_required( VERSION 2.8.0 )

project( load_library_prj )

include( ExternalProject )

find_package( Boost 1.59.0 COMPONENTS system filesystem regex unit_test_framework REQUIRED )
#set( Boost_LIBS -lboost_system -lboost_filesystem -lboost_regex -lboost_unit_test_framework )
set( Boost_LIBS ${Boost_LIBRARIES} )

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0601" )
	set( Boost_LIBS )
elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
	find_package( Threads REQUIRED )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++ -g -O3 -Weverything -Wno-c++98-compat  -Wfatal-errors -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion -Wno-global-constructors" )
elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
	find_package( Threads REQUIRED )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -O3 -Wall -Wno-deprecated-declarations" )
endif( )

ExternalProject_Add(
	header_libraries_prj
	GIT_REPOSITORY "https://github.com/beached/header_libraries.git"
	SOURCE_DIR "${CMAKE_BINARY_DIR}/3rdparty/header_libraries"
	GIT_TAG "master"
	PATCH_COMMAND ""
	INSTALL_DIR "${CMAKE_BINARY_DIR}/install"
	TEST_COMMAND ""
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
 )

include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
link_directories( "${CMAKE_BINARY_DIR}/install/lib" )

set( HEADER_FILES
	load_library.h
	load_library_posix.h
	load_library_windows.h
	load_plugin.h
	plugin_base.h
	ref_counted_value.h
)

set( SOURCE_FILES
	load_library.cpp
	load_library_posix.cpp
	load_library_test.cpp
	load_library_windows.cpp
	load_plugin.cpp
)

include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )

add_library ( load_library STATIC ${HEADER_FILES} ${SOURCE_FILES} )
add_dependencies( load_library header_libraries_prj )
target_link_libraries( load_library ${Boost_Libraries} )

install( TARGETS load_library DESTINATION lib )
install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw/parse_template )

add_executable( example_load_library example_load_library.cpp )
add_test( example_load_library_test, example_load_library )
add_dependencies( example_load_library header_libraries_prj )
target_link_libraries( example_load_library load_library ${CMAKE_DL_LIBS} ${Boost_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${COMPILER_SPECIFIC_LIBS} )

