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

#include "daw/system/load_library.h"
#include "daw/system/plugin_base.h"

#include <daw/daw_string_view.h>

#include <filesystem>
#include <string>
#include <vector>

namespace daw::nodepp::base {
	using plugin_t = std::pair<daw::system::library_handle,
	                           std::unique_ptr<daw::nodepp::plugins::IPlugin>>;

	std::vector<std::filesystem::path>
	get_files_in_folder( daw::string_view folder,
	                     std::vector<std::string> const &extensions );

	std::vector<plugin_t>
	load_libraries_in_folder( daw::string_view plugin_folder );
} // namespace daw::nodepp::base
