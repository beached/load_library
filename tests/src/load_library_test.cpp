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

#include <filesystem>
#include <iostream>
#include <optional>
#include <string_view>

#ifndef _WIN32
std::optional<std::filesystem::path>
find_library_file( std::string const &base_name,
              std::string const &root_path = "." ) {
	using namespace std::string_view_literals;
	std::string const lib_name = "lib" + base_name;
	static constexpr auto extensions = std::array{ ".so"sv, ".dylib"sv };
	for( std::filesystem::path const &file :
	     std::filesystem::directory_iterator( root_path ) ) {
		if( std::find( std::begin( extensions ),
		               std::end( extensions ),
		               file.extension( ) ) != std::end( extensions ) ) {
			if( base_name == file.stem( ) or lib_name == file.stem( ) ) {
				return file;
			}
		}
	}
	return { };
}
#endif

int main( ) {

#ifdef _WIN32
	char *strA = "this is a test";
	char *strB = "is";

	auto result = daw::system::call_dll_function<int>( "User32.dll",
	                                                   "MessageBoxA",
	                                                   (HWND)NULL,
	                                                   (LPCSTR)strA,
	                                                   (LPCSTR)strB,
	                                                   (UINT)0 );
#else
	auto lib_name = find_library_file( "test_library" );
	if( not lib_name ) {
		std::cerr << "could not find library\n";
		return 1;
	}
	auto result =
	  daw::system::call_library_function<int>( *lib_name, "add", 5, 6 );
#endif
	std::cout << result << '\n';
}
