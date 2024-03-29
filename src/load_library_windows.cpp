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

#ifdef _WIN32

#include "daw/system/impl/load_library_windows.h"

#include <daw/daw_move.h>
#include <daw/daw_utility.h>

#include <cstdint>
#include <filesystem>
#include <fmt/format.h>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <windows.h>

namespace daw::system::impl {

	std::string GetLastErrorAsString( DWORD errorMessageID ) {
		// Get the error message, if any.
		if( errorMessageID == 0 ) {
			return "No error message has been recorded";
		}
		auto buffer = std::vector<char>( 64, 0 );
		auto const flags =
		  FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		auto const lang_id = MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT );
		auto size = FormatMessageA( flags,
		                            nullptr,
		                            errorMessageID,
		                            lang_id,
		                            (LPSTR)buffer.data( ),
		                            64,
		                            nullptr );

		if( 0 == size ) {
			return "No message";
		}
		std::string message{ buffer.data( ), size };

		return message;
	}

	std::string GetLastErrorStdStr( ) {
		return GetLastErrorAsString( GetLastError( ) );
	}

	std::pair<DWORD, std::string> GetLastErrorAsString( ) {
		unsigned long err_no = ::GetLastError( );
		return std::make_pair( err_no, GetLastErrorAsString( err_no ) );
	}

	HINSTANCE load_library( std::filesystem::path const &library_path ) {
		auto result =
		  static_cast<HINSTANCE>( LoadLibraryW( library_path.c_str( ) ) );
		if( !result ) {
			auto const error_info = GetLastErrorAsString( );
			auto const message =
			  fmt::format( "Could not open library: error no: {} with message: {}",
			               error_info.first,
			               error_info.second );
			throw std::runtime_error( message );
		}
		return result;
	}

	void close_library( HINSTANCE handle ) {
		if( handle ) {
			FreeLibrary( handle );
		}
	}

} // namespace daw::system::impl
#endif
