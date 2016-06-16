cmake_minimum_required( VERSION 2.8.12 )

project( load_library_prj )

include( ExternalProject )

find_package( Boost 1.59.0 COMPONENTS system filesystem regex unit_test_framework REQUIRED )

IF( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_compile_options( -D_WIN32_WINNT=0x0601 ) 
ELSE( )
	find_package( Threads REQUIRED )
	if( THREADS_HAVE_PTHREAD_ARG )
		set( COMPILER_SPECIFIC_LIBS, "-lpthread" )
	endif()

	IF( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
		add_compile_options(-std=c++14 -stdlib=libc++ -Weverything -Wno-c++98-compat -Wfatal-errors -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion)		
	ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options(-std=c++14 -Wall -Wno-deprecated-declarations)
	ENDIF()
ENDIF()

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
target_link_libraries( load_library ${Boost_LIBRARIES} )

install( TARGETS load_library DESTINATION lib )
install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw/parse_template )

add_executable( example_load_library example_load_library.cpp )
add_test( example_load_library_test, example_load_library )
add_dependencies( example_load_library header_libraries_prj )
target_link_libraries( example_load_library load_library ${CMAKE_DL_LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${COMPILER_SPECIFIC_LIBS} )

