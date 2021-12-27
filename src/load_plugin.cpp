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

#include "daw/system/load_plugin.h"
#include "daw/system/load_library.h"
#include "daw/system/plugin_base.h"

#include <daw/daw_move.h>
#include <daw/daw_range_algorithm.h>
#include <daw/daw_string_view.h>

#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace daw::nodepp::base {
	using plugin_t = std::pair<daw::system::LibraryHandle,
	                           std::unique_ptr<daw::nodepp::plugins::IPlugin>>;

	std::vector<std::filesystem::path>
	get_files_in_folder( daw::string_view folder,
	                     std::vector<std::string> const &extensions ) {
		namespace fs = std::filesystem;
		std::vector<std::filesystem::path> result;
		auto p = fs::path( folder.data( ) );

		if( fs::exists( p ) and fs::is_directory( p ) ) {
			std::copy_if(
			  fs::directory_iterator( folder.data( ) ),
			  fs::directory_iterator( ),
			  std::back_inserter( result ),
			  [&extensions]( fs::path const &path ) {
				  return fs::is_regular_file( path ) and
				         ( extensions.empty( ) or
				           daw::algorithm::contains( extensions, path.extension( ) ) );
			  } );
		}
		return ::daw::algorithm::sort( result );
	}

	std::vector<plugin_t>
	load_libraries_in_folder( daw::string_view plugin_folder ) {
		static std::vector<std::string> const extensions = { ".npp" };

		std::vector<plugin_t> results;
		for( auto const &plugin_file :
		     get_files_in_folder( plugin_folder, extensions ) ) {
			auto const &filename = plugin_file.relative_path( ).string( );
			try {
				auto handle = ::daw::system::LibraryHandle( filename );
				auto create_func = handle.get_function<daw::nodepp::plugins::IPlugin *>(
				  "create_plugin" );
				auto plugin =
				  std::unique_ptr<daw::nodepp::plugins::IPlugin>( create_func( ) );
				results.emplace_back( DAW_MOVE( handle ), DAW_MOVE( plugin ) );
			} catch( std::runtime_error const &ex ) {
				// We are going to keep on going if we cannot load a plugin
				std::stringstream ss;

				ss << "Error loading plugin: " << filename << " with error\n"
				   << ex.what( ) << '\n';
				std::cerr << ss.str( ) << '\n';
			}
		}

		return results;
	}
} // namespace daw::nodepp::base
