/*==================================================================*\
  Nullptr.hpp
  ------------------------------------------------------------------
  Purpose:
  Provides a workaround of sorts for older compilers that do not
  implement the C++11 'nullptr' functionality-- this class/object
  type allows for semantically cleaner assignments to the null
  pointer sentinel value.

  ------------------------------------------------------------------
  (c)2010 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

#if( ET_COMPILER_IS_MSVC )
#	if( _MSC_VER >= 1600 )
#		define ET_NEEDS_NULLPTR_DEFINITION 0
#	endif
#endif

#ifndef ET_NEEDS_NULLPTR_DEFINITION
#	define ET_NEEDS_NULLPTR_DEFINITION 1
#endif

#if( ET_NEEDS_NULLPTR_DEFINITION )
	// nullptr itself is const, so stick the extra keyword here
	const class nullptr_t {
	public:
		template<class T>
		ETForceInlineHint operator T*( ) const { return 0; }

		template<class C, class T>
		ETForceInlineHint operator T C::*( ) const { return 0; }

	private:
		void operator&( ) const;

	} nullptr = {};
#endif // ET_NEEDS_NULLPTR_DEFINITION

}	// namespace Eldritch2

#undef ET_NEEDS_NULLPTR_DEFINITION