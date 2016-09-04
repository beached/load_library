// The MIT License (MIT)
// 
// Copyright (c) 2014-2016 Darrell Wright
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <functional>
#include <cstdint>

#include <daw/daw_exception.h>
namespace daw {

	template<typename T>
	class ReferenceCountedValue final {
		using cleaner_t = std::function<void( T & )>;
		struct data {
			T value;
			cleaner_t cleaner;
			
			template<typename... Args>
			data( Args&&... args ): 
					value{ new T( std::forward<Args>( args )... ) }, 
					cleaner{ } { }

			~data( ) {
				if( nullptr != value ) {
					if( cleaner ) {
						cleaner( value );
					}
				}
			}
		};	// data
		std::shared_ptr<data> m_value;
	public:

		template<typename... Args>
		ReferenceCountedValue( Args&&... args ): 
				m_value{ std::make_shared<data>( std::forward<Args>(args)...)  } { }

		ReferenceCountedValue( ReferenceCountedValue const & other ): 
				m_value{ other.m_value } { } 

		friend void swap( ReferenceCountedValue & lhs, ReferenceCountedValue & rhs ) noexcept {
			using std::swap;
			swap( lhs.m_value, rhs.m_value );
		}

		ReferenceCountedValue& operator=(ReferenceCountedValue const & rhs) {
			if( this != &rhs ) {
				ReferenceCountedValue tmp{ rhs };
				using std::swap;
				swap( *this, rhs );
			}
			return *this;
		}

		ReferenceCountedValue( ReferenceCountedValue && other ) : 
				m_value{ std::move( other.m_value ) } { }

		ReferenceCountedValue& operator=( ReferenceCountedValue && rhs) {
			if( this != &rhs ) {
				ReferenceCountedValue tmp{ std::move( rhs ) };
				using std::swap;
				swap( *this, rhs );
			}
			return *this;
		}

		void set_cleaner( cleaner_t cleaner ) {
			m_value->cleaner = cleaner;
		}

		void clear_cleaner( ) {
			m_value->cleaner = cleaner_t{ }; 
		}

		T & operator( )( ) {
			return m_value->value;
		}

		T const & operator( )( ) const { 
			return m_value->value;
		}

		bool empty( ) const {
			return !m_value;
		}

		~ReferenceCountedValue( ) = default; 

		};	// ReferencedCountedValue
}	// namespace daw

