/*==================================================================*\
  Alignof.hpp
  ------------------------------------------------------------------
  Purpose:
  Implements a C++ type trait using the template mechanism and
  additionally provides a means of querying the value for the type
  by way of a macro with very similar usage semantics to the
  built-in C++ sizeof() operator.
  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#include <cstddef>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace TypeTraits{
	template<typename Type>
	struct AlignmentOf {
#if( !ET_COMPILER_SUPPORTS_CPP11 )
#	if( ET_COMPILER_IS_MSVC )
			ENUM_CLASS( size_t ) { 
				val = __alignof( Type )
			};
#	elif( ET_COMPILER_IS_GCC )
			ENUM_CLASS( size_t ) {
				val =  __alignof__( Type )
			};
#	else
			ENUM_CLASS( size_t ) {
				val = static_cast<size_t>( offsetof( struct{ char c; Type m; }, m ) )
			};
#	endif
#else
			ENUM_CLASS( size_t ) {
				val = alignof( Type )
			};
#endif
	};

}	// namespace TypeTraits
}	// namespace Eldritch2

#if( !ET_COMPILER_SUPPORTS_CPP11 )
#	define alignof( x ) static_cast<size_t>( ::Eldritch2::TypeTraits::AlignmentOf<x>::val )
#endif