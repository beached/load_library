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

#pragma once

#include <memory>
#include <string>

#include <daw/daw_utility.h>

#ifdef _WIN32
#include "load_library_windows.h"
#else
#include "load_library_posix.h"
#endif

namespace daw::system {
	namespace impl {
		using load_library_result_t =
		  daw::remove_cvref_t<decltype( load_library( "" ) )>;
		struct HandleDeleter {
			constexpr void operator( )( load_library_result_t ptr ) const {
				if( ptr ) {
					close_library( ptr );
				}
			}
		};

		class OSLibraryHandle {
			std::shared_ptr<std::remove_pointer_t<load_library_result_t>> m_handle =
			  { };

		public:
			explicit OSLibraryHandle( std::string const &library_path );
#ifdef _WIN32
			explicit OSLibraryHandle( std::wstring const &library_path );
#endif
			[[nodiscard]] load_library_result_t get( ) const;

			[[nodiscard]] explicit operator bool( ) const {
				return static_cast<bool>( m_handle );
			}
		}; // class OSLibraryHandle
	}    // namespace impl

	class LibraryHandle {
		using handle_t = std::remove_pointer_t<
		  daw::remove_cvref_t<decltype( impl::load_library( "" ) )>>;
		std::shared_ptr<handle_t> m_handle = { };

	public:
		template<typename StringType>
		explicit LibraryHandle( StringType const &library_path )
		  : m_handle{ impl::load_library( library_path ), impl::HandleDeleter{} } {}

		template<typename ResultType, typename... Args>
		ResultType call_function( std::string const &function_name,
		                          Args &&...function_args ) const {
			auto function_ptr =
			  impl::get_function_address<ResultType, Args...>( m_handle.get( ),
			                                                   function_name );
			return ( *function_ptr )( std::forward<Args>( function_args )... );
		}

		template<typename ResultType, typename... Args>
		auto get_function( std::string const &function_name )
		  -> daw::function_pointer_t<ResultType, Args...> {
			auto function_ptr =
			  impl::get_function_address<ResultType, Args...>( m_handle.get( ),
			                                                   function_name );
			return function_ptr;
		}
	}; // class LibraryHandle

	template<typename ResultType, typename... Args>
	ResultType call_library_function( std::string const &dll_name,
	                                  std::string const &function_name,
	                                  Args &&...function_args ) {
		auto lib = LibraryHandle( dll_name );
		return lib.call_function<ResultType, Args...>(
		  function_name,
		  DAW_FWD2( Args, function_args )... );
	}

#ifdef _WIN32
	template<typename ResultType, typename... Args>
	ResultType call_library_function( std::wstring const &dll_name,
	                              std::string const &function_name,
	                              Args &&...function_args ) {
		auto lib = LibraryHandle( dll_name );
		return lib.call_function<ResultType, Args...>(
		  function_name,
		  DAW_FWD2( Args, function_args )... );
	}
#endif
} // namespace daw::system
