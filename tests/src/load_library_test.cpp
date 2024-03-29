// The MIT License (MIT)
//
// Copyright (c) Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <daw/system/load_library.h>

#include <cassert>
#include <filesystem>
#include <fmt/format.h>
#include <optional>
#include <string_view>

using string_t = typename std::filesystem::path::string_type;

std::optional<string_t>
find_library_file( string_t const &base_name,
                   std::filesystem::path const &root_path ) {
	using namespace std::string_view_literals;
	fmt::print( stderr, "Searching for library in {}\n", root_path.string( ) );
#ifndef _WIN32
	string_t const lib_name = "lib" + base_name;
	static constexpr auto extensions = std::array{ ".so"sv, ".dylib"sv };
#else
	string_t const lib_name = L"lib" + base_name;
	static constexpr auto extensions = std::array{ std::wstring_view( L".dll" ) };
#endif

	for( std::filesystem::path const &file :
	     std::filesystem::recursive_directory_iterator( root_path ) ) {
		if( std::find( std::begin( extensions ),
		               std::end( extensions ),
		               file.extension( ) ) != std::end( extensions ) ) {
			if( base_name == file.stem( ) or lib_name == file.stem( ) ) {
				return file.native( );
			}
		}
	}
	return { };
}

int main( ) {
#ifdef _WIN32
	auto lib_name =
	  find_library_file( L"test_library", std::filesystem::current_path( ) );
#else
	auto lib_name =
	  find_library_file( "test_library", std::filesystem::current_path( ) );
#endif
	if( not lib_name ) {
		fmt::print( stderr, "could not load library\n" );
		return EXIT_FAILURE;
	}
	auto result =
	  daw::system::call_library_function<int>( *lib_name, "add", 5, 6 );
	assert( result == 11 );
	fmt::print( "result: {}\n", result );
}
