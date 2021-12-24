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

#include "daw/system/load_library.h"

#include <daw/daw_move.h>

#include <memory>
#include <string>

namespace daw::system::impl {
	OSLibraryHandle::OSLibraryHandle( std::string const &library_path )
	  : m_handle{ impl::load_library( library_path ) } {

		m_handle.set_cleaner( []( handle_t &handle ) { close_library( handle ); } );
	}

#ifdef _WIN32
	OSLibraryHandle::OSLibraryHandle( std::wstring const &library_path )
	  : m_handle{ impl::load_library( library_path ) } {

		m_handle.set_cleaner( []( handle_t &handle ) { close_library( handle ); } );
	}
#endif

	OSLibraryHandle::handle_t &OSLibraryHandle::get( ) {
		return m_handle( );
	}

	OSLibraryHandle::handle_t const &OSLibraryHandle::get( ) const {
		return m_handle( );
	}
} // namespace daw::system::impl

namespace daw::system {
	// 		LibraryHandle::LibraryHandle( LibraryHandle other ): m_handle(
	// DAW_MOVE( other.m_handle ) ) { }
	//
	// 		LibraryHandle LibraryHandle::operator=(LibraryHandle rhs) {
	// 			m_handle = DAW_MOVE( rhs.m_handle );
	// 			return *this;
	// 		}
}
